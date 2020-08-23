#ifndef _GPIO_H
#define _GPIO_H

#include <string>
#include <fstream>
#include <cstdint>


#ifndef SYSTEM_GPIO_PATH_PREFIX
#define SYSTEM_GPIO_PATH_PREFIX "/sys/class/gpio/"
#endif

namespace mrobot {

namespace hw {

class Gpio {
    public:
        enum Direction {
            DirectionIn,
            DirectionOut,
        };

        Gpio(uint32_t pin, Gpio::Direction dir);
        ~Gpio() = default;

        [[nodiscard]] bool init();
        [[nodiscard]] bool finalize();
        [[nodiscard]] bool set(bool on);

    private:
        [[nodiscard]] bool pinExport();
        [[nodiscard]] bool pinUnexport();
        [[nodiscard]] bool setPinDirection();
        [[nodiscard]] bool writeToGpioEntry(const std::string &entry,
                                            const std::string &value);

        uint32_t m_pin;
        Direction m_dir;
        std::fstream m_valueEntry;

        static const char *s_sysPrefix;
};

} // namespace hw

} // namespace mrobot

#endif // _GPIO_H
