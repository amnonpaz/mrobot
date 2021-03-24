#include "robot_messages_router.hpp"

namespace mrobot {

std::unique_ptr<messaging::Message> RobotMessagesRouter::factory(uint32_t messageId) const {
    (void)(messageId);

    std::unique_ptr<messaging::Message> message = nullptr;

    // TODO: implement

    return message;
}

uint32_t RobotMessagesRouter::topicToMessageId(const std::string &topic) const {
    (void)(topic);

    // TODO: implement

    return 0;
}

const std::string &RobotMessagesRouter::messageIdToTopic(uint32_t messageId) const {
    (void)(messageId);
    static const std::string dummy;

    return dummy;
}

} // namespace mrobot
