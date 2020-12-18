#include "detector_thread.h"
#include "image_converter.h"
#include "led_controller.h"

const char *model_binary = "/opt/model/opencv_face_detector_uint8.pb";
const char *model_configuration = "/opt/model/opencv_face_detector.pbtxt";

using namespace std;
using namespace cv;

detector_thread::detector_thread() {
    state = NOT_STARTED;
}
detector_thread::~detector_thread() {}

void detector_thread::set_display_listener(display_listener_t l) {
    display_listener = l;
}

void detector_thread::pause() {}

void detector_thread::resume() {}

void detector_thread::close() {
    if (state == CLOSED)
        return;
    state = CLOSED;
    if (t && t->joinable())
        t->join();
    capture.release();

    delete face_detection;
    face_detection = nullptr;
}

void detector_thread::work() {
    while (state != CLOSED) {
        if(state == PAUSED) continue;
        static Mat tmp_frame;
        {
            lock_guard<mutex> lock(update_lock);
            tmp_frame = frame;
        }
        if (!tmp_frame.empty()) {
            result_vector result = face_detection->detect(frame);
            {
                lock_guard<mutex> lock(update_lock);
                results = result;
            }
        }
    }
}

void detector_thread::loop() {
    while (state != CLOSED) {
        if(state == PAUSED) continue;
        static Mat cur_frame;
        static Mat resized_frame;

        capture >> cur_frame;

        result_vector last_result;

        {
            lock_guard<mutex> lock(update_lock);
            frame = cur_frame;
            last_result = results;
        }

        if (!cur_frame.empty()) {
            if (!last_result.empty()) {
                led7.off();
                led0.on();
                render(cur_frame, last_result);
            } else {
                led7.on();
                led0.off();
            }

            cv::resize(frame, resized_frame, Size(640, 480), 0, 0,
                       INTER_LINEAR);

            display_listener(cvMat_to_QImage(resized_frame));
        }
    }
}

detector_thread::invocation_result detector_thread::start(int camera_index) {
    if (state != NOT_STARTED)
        return ILLEGAL_STATE;

    if (!capture.open(camera_index)) {
        return CAMERA_ERROR;
    }

    face_detection = new dnnfacedetect(model_binary, model_configuration);
    if (!face_detection->initdnnNet()) {
        return DNN_INITILIAZTION_FAILURE;
    }

    state = RUNNING;
    t = new thread(std::mem_fn(&detector_thread::loop), this);
    detection_worker = new thread(std::mem_fn(&detector_thread::work), this);
    return OK;
}
