#include "detector_thread.h"
#include "image_converter.h"
#include "led_controller.h"

const char *model_binary = "/opt/model/opencv_face_detector_uint8.pb";
const char *model_configuration = "/opt/model/opencv_face_detector.pbtxt";

detector_thread::detector_thread(){}
detector_thread::~detector_thread() {}

void detector_thread::set_display_listener(display_listener_t l) {
    display_listener = l;
}

void detector_thread::pause(){

}

void detector_thread::resume(){

}

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

void detector_thread::run() {
    while (state != CLOSED) {
        capture >> frame;
        static cv::Mat resized_picture;
        cv::resize(frame, resized_picture, Size(640, 480), 0, 0, INTER_LINEAR);
        if (!frame.empty()) {
            auto checkR = face_detection->detect(frame);
            if (checkR.markR) {
                led7.off();
                led0.on();
            }
            auto img = cvMat_to_QImage(checkR.frameR);
            display_listener(img);
        }
    }
}

detector_thread::invocation_result
detector_thread::start(int camera_index) {
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
    t = new thread(std::mem_fn(&detector_thread::run), this);
    return OK;
}
