#include "led.hpp"

#include <chrono>
using namespace std::chrono_literals;

namespace mrobot {

namespace hw {

bool Led::init() {
    if (!m_gpio.init()) {
        return false;
    }

    return set(OFF);
}

bool Led::finalize() {
    return m_gpio.init();
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
