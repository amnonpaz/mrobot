#include "led.hpp"

#include <chrono>
using namespace std::chrono_literals;

namespace mrobot {

namespace hw {

Led::~Led() {
    if (m_initialized) {
        bool(finalize());
    }
}

bool Led::init() {
    if (!m_gpio.init()) {
        return false;
    }

    m_initialized = set(OFF);

    return m_initialized;
}

bool Led::finalize() {
    bool res = m_gpio.finalize();
    m_initialized = false;
    return res;
}

bool Led::set(State state) const {
    bool res = false;

    // leds are active low
    switch (state) {
        case OFF:
            res = m_gpio.set(true);
            break;
        case ON:
            res = m_gpio.set(false);
            break;
        default:
            break;
    }

    if (res) {
        m_state = state;
    }

    return res;
}

} // namespace hw

} // namespace mrobot
