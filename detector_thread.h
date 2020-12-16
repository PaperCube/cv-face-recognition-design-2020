#ifndef DETECTOR_THREAD_H
#define DETECTOR_THREAD_H

#include "concurrent_util.h"
#include "dnnfacedetect.h"
#include <atomic>
#include <functional>
#include <thread>
#include <type_traits>

extern const char *model_binary;
extern const char *model_configuration;

class detector_thread {
  public:
    using display_listener_t = function<void(const QImage &)>;

    enum State { NOT_STARTED, RUNNING, PAUSED, CLOSED };
    enum invocation_result { OK, ILLEGAL_STATE, CAMERA_ERROR, DNN_INITILIAZTION_FAILURE };

    detector_thread();
    virtual ~detector_thread();

    void set_display_listener(display_listener_t);

    invocation_result start(int);
    void close();
    void pause();
    void resume();

  private:
    display_listener_t display_listener;

    VideoCapture capture;
    dnnfacedetect *face_detection = nullptr;

    thread *t = nullptr;
    Mat frame;
    atomic<State> state;

    void run();
};

#endif // DETECTOR_THREAD_H
