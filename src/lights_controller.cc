#include "lights_controller.hpp"

#include <functional>
#include <chrono>

using namespace std::chrono_literals;

namespace mrobot {

namespace control {

Lights::~Lights() {
    stop();
}

void Lights::add(const hw::Led *led) {
    std::lock_guard<std::mutex> _lk(m_lock);

    m_leds.push_back(led);
}

void Lights::work() {
    std::unique_lock<std::mutex> _lk(m_lock);
    
    m_isWorking = true;

    do {
        switch (m_scheme) {
            case SchemeTypeBlink:
                blink();
                break;
            default:
                break;
        }
    } while (m_cv.wait_for(_lk, m_period*1ms, [&]() -> bool { return m_isWorking; }));
}

void Lights::start() {
    std::lock_guard<std::mutex> _lk(m_lock);

    if (m_isWorking) {
        return;
    }

    m_worker = std::thread([&](){ work(); });
}

void Lights::stop() {
    std::lock_guard<std::mutex> _lk(m_lock);

    if (!m_isWorking) {
        return;
    }

    m_isWorking = false;
    m_cv.notify_all();
    m_worker.join();
}

void Lights::blink() {
    for (auto led : m_leds) {
        if (led->get() == hw::Led::ON) {
            led->set(hw::Led::OFF);
        } else {
            led->set(hw::Led::ON);
        }

        if (m_between > 0) {
            std::this_thread::sleep_for(1ms*m_between);
        }
    }
}

} // namespace hw

} // namespace mrobot
