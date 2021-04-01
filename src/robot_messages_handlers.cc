#include "robot_messages_handlers.hpp"
#include "robot.hpp"

namespace mrobot {

namespace message_handler {

void LightSet::handleMessage(const std::unique_ptr<messaging::IncomingMessage> &message) {
    const auto *lightSetMessage = dynamic_cast<const MessageLightSet *>(message.get()); 

    m_owner->setLight(Robot::Light(lightSetMessage->getId()), lightSetMessage->getState() != 0); 
}

} // namespace message_handler

} // namespace mrobot  
