#include "gpio.hpp"

#include <iostream>
#include <array>

namespace mrobot {

namespace hw {

const char *Gpio::s_sysPrefix = "";

Gpio::Gpio(uint32_t pin, Direction dir) :
    m_pin(pin),
    m_dir(dir) {
}

bool Gpio::init() {
    std::cout << "GPIO #" << std::to_string(m_pin) << " was initalized" << '\n';

    return true;
}

bool Gpio::finalize() {
    std::cout << "GPIO #" << std::to_string(m_pin) << " was finalized" << '\n';

    return true;
}

bool Gpio::set(bool on) const {
    static const std::array<const char*, 2> strings{"0","1"};

    if (m_dir != DirectionOut) {
        std::cout << "GPIO #" << std::to_string(m_pin) << " cannot be se";
        return false;
    }

    std::cout << "GPIO #" << std::to_string(m_pin) << " set to " << strings[on ? 1 : 0] << '\n';

    return true;
}

bool Gpio::pinExport() const {
    return true;
}

bool Gpio::pinUnexport() const {
    return true;
}

bool Gpio::setPinDirection() {
    return true;
}

bool Gpio::writeToGpioEntry(const std::string &entry,
                            const std::string &value) {
    (void)(entry);
    (void)(value);

    return true;
}

} // namespace hw

} // namespace mrobot

