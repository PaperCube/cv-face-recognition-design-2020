#include "led_controller.h"
#include <iostream>
#include <type_traits>
#include <wiringPi.h>

bool led_controller::is_setup = false;
std::mutex led_controller::lock;

led_controller led0(0);
led_controller led7(7);

void led_controller::ensure_setup() {
    mutex_lock_guard lock(led_controller::lock);
    if (is_setup) {
        return;
    }
    wiringPiSetup();
    is_setup = true;
}

led_controller::led_controller(int port) { bind(port); }

void led_controller::bind(int port) {
    if (port < 0)
        return;
    this->port = port;
    ensure_setup();
    pinMode(port, OUTPUT);
    set_state(false);
}

void led_controller::set_state(bool status) {
    if (port < 0)
        return;
    digitalWrite(port, status ? HIGH : LOW);
}

void led_controller::flash(int interval, int delay, int off_interval,
                           int flash_cnt) {
    using std::clog;
    using std::endl;
    using namespace std::chrono;
    if (off_interval < 0)
        off_interval = interval;
    if (flash_cnt < 0)
        flash_cnt = ~0U >> 1;

    mutex_unique_lock lock(flash_lock);
    if (is_flashing && t) {
        is_flashing = false;
        cvar.notify_all();
        std::clog << "LED is flashing. Notifying all..." << std::endl;
    }

    lock.unlock();

    if (t && t->joinable())
        t->join();
    std::clog << "Thread exited" << std::endl;
    delete t;

    lock.lock();

    is_flashing = true;
    std::clog << "this = " << this << std::endl;
    t = new std::thread([&, interval, delay, off_interval]() {
        auto should_stop = [this]() -> bool { return !is_flashing; };
        bool flag = should_stop();
        mutex_unique_lock lck(flash_lock);
        cvar.wait_for(lck, delay * 1ms, should_stop);
        if (should_stop())
            return;

        for (int i = 0; i < flash_cnt && !should_stop(); i++) {
            this->set_state(true);
            cvar.wait_for(lck, interval * 1ms, should_stop);
            if (should_stop())
                break;

            if (i != flash_cnt - 1) {
                this->set_state(false);
                cvar.wait_for(lck, off_interval * 1ms, should_stop);

                if (should_stop())
                    break;
            }
        }
        this->set_state(false);
        is_flashing = false;
        std::clog << "Exiting thread" << std::endl;
    });
    lock.unlock();
}
