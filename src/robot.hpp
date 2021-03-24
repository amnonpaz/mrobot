#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "led.hpp"
#include "conf.hpp"
#include "mqtt_client.hpp"
#include "robot_messages_router.hpp"

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
        bool initComm();

        static const char *to_string(Light light);

        const std::string m_confFile;
        conf::Database m_configuration;
        std::array<std::unique_ptr<hw::Led>, LightMax> m_lights;

        std::unique_ptr<comm::MqttClient> m_comm;
        const RobotMessagesRouter m_messagesRouter;

};

} // namespace mrobot 

#endif // _ROBOT_H_
