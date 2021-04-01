#include "robot_messages_router.hpp"

#include <iostream>
#include <cstring>

namespace mrobot {

bool MessageLightSet::deserialize(const unsigned char *payload, ::size_t size) {
    static constexpr ::size_t dataSize = sizeof(size);

    if (size < dataSize) {
        std::cout << "MessageLightSet Error: Payload too small [" << size << "]" << '\n';
        return false;
    }

    if (size > dataSize) {
        std::cout << "MessageLightSet Error: Payload too large [" << size << "]" << '\n';
        return false;
    }

    ::memcpy(&m_data, payload, dataSize);

    return true;
}

std::unique_ptr<messaging::IncomingMessage> RobotImcomingMessagesRouter::factory(uint32_t messageId) const {
    (void)(messageId);

    std::unique_ptr<messaging::IncomingMessage> message = nullptr;

    switch (MessageType(messageId)) {
        case MessageTypeLightSet:
            message = std::make_unique<MessageLightSet>();
            break;
        default:
            std::cout << "Unknown message ID " << messageId << '\n';
            break;
    }

    return message;
}

uint32_t RobotImcomingMessagesRouter::topicToMessageId(const std::string &topic) const {
    uint32_t messageId = MessageTypeMax;

    for (messageId = 0; messageId < MessageTypeMax; ++messageId) {
        if (topic == messageIdToTopic(messageId)) {
            break;
        }
    }

    return messageId;
}

const std::string &RobotImcomingMessagesRouter::messageIdToTopic(uint32_t messageId) const {
    static const std::array<std::string, MessageTypeMax> strings = {
        "/led/set" // MessageTypeLightSet
    };

    return strings[messageId];
}

} // namespace mrobot
