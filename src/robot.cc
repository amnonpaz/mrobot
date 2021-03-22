#include "robot.hpp"

#include <iostream>
#include <sstream>

#include "mqtt_client.hpp"

namespace mrobot {

bool Robot::init()
{
    if (!m_configuration.addFile(m_confFile)) {
        return false;
    }

    if (!initLights()) {
        std::cout << "Error initializing lights" << '\n';
        return false;
    }

    if (!initComm()) {
        std::cout << "Error initializing communication" << '\n';
        return false;
    }

    return true;
}

bool Robot::setLight(Light light, bool on)
{
    return m_lights[light]->set(on ? hw::Led::ON : hw::Led::OFF);
}

bool Robot::initLights() {
    bool res = true;

    for (uint32_t idx = 0; idx < uint32_t(LightMax); ++idx) {
        const std::string lightName{to_string(Light(idx))};
        const std::string &value = m_configuration.get(lightName);
        if (value.empty()) {
            std::cout << "Error: No configuration for " << lightName << '\n';
            res = false;
            break;
        }

        std::stringstream sin(value);
        uint32_t gpioNum;
        sin >> gpioNum;

        m_lights[idx] = std::make_unique<hw::Led>(gpioNum);
        if (!m_lights[idx]->init()) {
            std::cout << "Error initializing " << lightName << '\n';
            res = false;
            break;
        }
    }

    return res;
}

bool Robot::initComm() {
    bool res = true;

    auto clientName = m_configuration.get("mqtt_client_name");
    auto brokerURL = m_configuration.get("mqtt_broker_url");
    auto brokerPort = m_configuration.get("mqtt_broker_port");

    if (clientName.empty()) {
        std::cout << "No configuration for mqtt_client_name" << '\n';
        res = false;
    }

    if (brokerURL.empty()) {
        std::cout << "No configuration for mqtt_broker_url" << '\n';
        res = false;
    }

    if (brokerPort.empty()) {
        std::cout << "No configuration for mqtt_broker_port" << '\n';
        res = false;
    }

    if (!res) {
        return false;
    }

    uint32_t brokerPortUint;
    std::stringstream sin(brokerPort);
    sin >> brokerPortUint;

    m_comm = std::make_unique<comm::MqttClient>(clientName,
                                                brokerURL,
                                                brokerPortUint,
                                                &m_messagesRouter);

    return m_comm != nullptr;
}

const char *Robot::to_string(Light light) {
    static const std::array<const char *, LightMax + 1> strings = {
        "eye_right",
        "eye_left",
        "antena_right",
        "antena_left",
        "invlid_light"
    };

    if (light > LightMax) {
        light = LightMax;
    }

    return strings[light];
}

} // namespace mrobot
