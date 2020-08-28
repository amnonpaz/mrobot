#include "gpio.hpp"

#include <iostream>
#include <array>

namespace mrobot {

namespace hw {

const char *Gpio::s_sysPrefix = SYSTEM_GPIO_PATH_PREFIX;

Gpio::Gpio(uint32_t pin, Direction dir) :
    m_pin(pin),
    m_dir(dir) {
}

bool Gpio::init() {
    if (!pinExport()) {
        std::cout << "Error exporting GPIO#" << std::to_string(m_pin) << '\n';
        return false;
    }

    if (!setPinDirection()) {
        std::cout << "Error setting GPIO#" << std::to_string(m_pin) << " direction"<< '\n';
        return false;
    }

    const std::string valueEntryName{std::string{s_sysPrefix} + "gpio" + std::to_string(m_pin) + "/value"};
    m_valueEntry.open(valueEntryName);
    if (!m_valueEntry.is_open()) {
        std::cout << "Error opening GPIO#" << std::to_string(m_pin) << " value entry"<< '\n';
        if (!pinUnexport()) {
            std::cout << "Error unexporting GPIO#" << std::to_string(m_pin) << '\n';
        }
        return false;
    }

    std::cout << "GPIO #" << std::to_string(m_pin) << " was initalized" << '\n';

    return true;
}

bool Gpio::finalize() {
    m_valueEntry.close();

    if (!pinUnexport()) {
        std::cout << "Error unexporting GPIO#" << std::to_string(m_pin) << '\n';
        return false;
    }

    return true;
}

bool Gpio::set(bool on) const {
    static const std::array<const char*, 2> strings{"0","1"};

    if (m_dir != DirectionOut) {
        return false;
    }

    const char *value = strings[on ? 1 : 0];
    m_valueEntry << value << std::flush;

    return m_valueEntry.good();
}

bool Gpio::pinExport() {
    return writeToGpioEntry("export", std::to_string(m_pin));
}

bool Gpio::pinUnexport() {
    return writeToGpioEntry("unexport", std::to_string(m_pin));
}

bool Gpio::setPinDirection() {
    std::string value{(m_dir == DirectionIn) ? "in" : "out" };
    return writeToGpioEntry("gpio" + std::to_string(m_pin) + "/direction", value);
}

bool Gpio::writeToGpioEntry(const std::string &entry,
                            const std::string &value) {
    std::fstream gpioEntry;

    gpioEntry.open(std::string{s_sysPrefix} + entry);
    if (!gpioEntry.is_open()) {
        std::cout << "Failed writing " << value << " to " << entry << '\n';
        return false;
    }

    gpioEntry << value;
    bool res = gpioEntry.good();
    gpioEntry.close();

    return res;
}

} // namespace hw

} // namespace mrobot

