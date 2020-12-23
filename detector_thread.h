#ifndef DETECTOR_THREAD_H
#define DETECTOR_THREAD_H

#include "concurrent_util.h"
#include "dnnfacedetect.h"
#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <type_traits>

extern const char *model_binary;
extern const char *model_configuration;

class detector_thread {
  public:
    using display_listener_t = std::function<void(const QImage &)>;

    enum State { NOT_STARTED, RUNNING, PAUSED, CLOSED };
    enum invocation_result {
        OK,
        ILLEGAL_STATE,
        CAMERA_ERROR,
        DNN_INITIALIZATION_FAILURE
    };

    std::atomic<State> state;

    detector_thread();
    virtual ~detector_thread();

    void set_display_listener(display_listener_t);

    invocation_result start(int);
    void close();
    void pause();
    void resume();

  private:
    using result_vector = std::vector<cv::Rect>;

    display_listener_t display_listener;

    cv::VideoCapture capture;
    dnnfacedetect *face_detection = nullptr;

    std::mutex update_lock;

    std::thread *t = nullptr;
    std::thread *detection_worker = nullptr;

    cv::Mat frame;
    result_vector results;

    void loop();
    void work();
};

#endif // DETECTOR_THREAD_H
