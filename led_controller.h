#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "concurrent_util.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

class led_controller {
  private:
    static std::mutex lock;
    static bool is_setup;

  public:
    static void ensure_setup();

    led_controller(int port);
    led_controller() : led_controller(-1) {}

    void bind(int);
    void set_state(bool status);

    void on() { this->set_state(true); }

    void off() { this->set_state(false); }

    void flash(int interval, int delay, int off_interval, int flash_cnt);
    void flash(int interval, int flash_cnt) {
        flash(interval, 0, interval, flash_cnt);
    }

  private:
    int port;

    std::mutex flash_lock;
    std::condition_variable cvar;
    std::atomic_bool is_flashing;
    std::thread *t = nullptr;
};

extern led_controller led0;
extern led_controller led7;

#endif // LED_CONTROLLER_H
