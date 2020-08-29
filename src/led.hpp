#ifndef _LED_H
#define _LED_H

#include "gpio.hpp"

namespace mrobot {

namespace hw {

class Led final {
    public:
        enum State {
            OFF,
            ON
        };

        explicit Led(uint32_t gpioPin) :
            m_gpio(gpioPin, hw::Gpio::DirectionOut),
            m_state(OFF),
            m_initialized(false) {}
        ~Led();

        [[nodiscard]] bool init();
        [[nodiscard]] bool finalize();

        bool set(State state) const;
        State get() const { return m_state; };

    private:
        hw::Gpio m_gpio;
        mutable State m_state;
        bool m_initialized;
};

} // namespace hw

} // namespace mrobot

#endif // _LED_H
