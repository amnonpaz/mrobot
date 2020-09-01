#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "led.hpp"
#include "conf.hpp"

#include <string>
#include <array>
#include <memory>

namespace mrobot {
    
class Robot final {
    public:
        enum Light {
            LightEyeRight,
            LightEyeLeft,
            LightAntenaRight,
            LightAntenaLeft,
            LightMax
        };

        explicit Robot(std::string confFile) :
            m_confFile(std::move(confFile)) {}
        ~Robot() = default;

        bool init();
        bool setLight(Light light, bool on);

    private:
        bool initLights();

        static const char *to_string(Light light);

        const std::string m_confFile;
        conf::Database m_configuration;
        std::array<std::unique_ptr<hw::Led>, LightMax> m_lights;
};

} // namespace mrobot 

#endif // _ROBOT_H_