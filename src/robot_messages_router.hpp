#ifndef ROBOT_MESSAGES_ROUTER_H_
#define ROBOT_MESSAGES_ROUTER_H_

#include "mqtt_client.hpp"

namespace mrobot {

class RobotMessagesRouter final : public comm::MqttRouter {
    public:
        RobotMessagesRouter() : comm::MqttRouter(0) {}
        virtual ~RobotMessagesRouter() = default;

    protected:
        std::unique_ptr<messaging::Message> factory(uint32_t messageId) const override;
        uint32_t topicToMessageId(const std::string &topic) const override;
};

} // namespace mrobot

#endif // ROBOT_MESSAGES_ROUTER_H_
