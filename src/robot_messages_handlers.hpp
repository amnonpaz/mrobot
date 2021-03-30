#ifndef ROBOT_MESSAGES_HANDLERS_H_
#define ROBOT_MESSAGES_HANDLERS_H_

#include "message.hpp"

namespace mrobot {

class Robot;

namespace message_handler {

class Base : public messaging::Handler {
    public:
        explicit Base(Robot *pRobot) :
            m_owner(pRobot) {}
        ~Base() = default;

    protected:
        Robot *m_owner;

};

class LightSet : public Base {
    public:
        explicit LightSet(Robot *pRobot) :
            Base(pRobot) {}
        ~LightSet() = default;

        void handleMessage(const std::unique_ptr<messaging::Message> &message) override;
};

} // namespace message_handler

} // namespace mrobot  

#endif // ROBOT_MESSAGES_HANDLERS_H_
