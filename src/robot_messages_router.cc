#include "robot_messages_router.hpp"

namespace mrobot {

std::unique_ptr<messaging::Message> RobotMessagesRouter::factory(uint32_t messageId,
                                                                 const unsigned char *payload,
                                                                 ::size_t size) const {
    (void)(messageId);
    (void)(payload);
    (void)(size);

    std::unique_ptr<messaging::Message> message = nullptr;

    // TODO: implement

    return message;
}

uint32_t RobotMessagesRouter::topicToMessageId(const std::string &topic) const {
    (void)(topic);

    // TODO: implement

    return 0;
}

} // namespace mrobot
