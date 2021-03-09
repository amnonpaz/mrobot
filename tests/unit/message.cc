#include "message.hpp"

#include "UnitTest.hpp"

#include <iostream>

TEST_MODULE(MessageTestModule)

using unique_message_ptr = std::unique_ptr<mrobot::messaging::Message>;

enum TestMessageType {
    TestMessageTypeFirst, 
    TestMessageTypeSecond, 
    TestMessageTypeThird, 
    TestMessageTypeMax
};

class MessageFirst : public mrobot::messaging::Message {
    public:
        MessageFirst() = default;
        virtual ~MessageFirst() = default;

        bool deserialize(const unsigned char *payload, ::size_t size) override {
            (void)(payload);
            (void)(size);
            return true;
        }
};

class MessageFirstHandler : public mrobot::messaging::Handler {
    public:
        MessageFirstHandler() = default;
        virtual ~MessageFirstHandler() = default;

        void handleMessage(const unique_message_ptr &message) override {
            (void)(message);
        }
};

class MessageSecond : public mrobot::messaging::Message {
    public:
        MessageSecond() = default;
        virtual ~MessageSecond() = default;

        bool deserialize(const unsigned char *payload, ::size_t size) override {
            (void)(payload);
            (void)(size);
            return true;
        }
};

class MessageSecondHandler : public mrobot::messaging::Handler {
    public:
        MessageSecondHandler() = default;
        virtual ~MessageSecondHandler() = default;

        void handleMessage(const unique_message_ptr &message) override {
            (void)(message);
        }
};

class MessageThird : public mrobot::messaging::Message {
    public:
        MessageThird() = default;
        virtual ~MessageThird() = default;

        bool deserialize(const unsigned char *payload, ::size_t size) override {
            (void)(payload);
            (void)(size);
            return true;
        }
};

class MessageThirdHandler : public mrobot::messaging::Handler {
    public:
        MessageThirdHandler() = default;
        virtual ~MessageThirdHandler() = default;

        void handleMessage(const unique_message_ptr &message) override {
            (void)(message);
        }
};


class TestMessagesFactory : public mrobot::messaging::Factory {
    public:
        TestMessagesFactory() = default; 
        ~TestMessagesFactory() = default; 

        unique_message_ptr create(uint32_t messageId,
                                  const unsigned char *payload,
                                  ::size_t size) const override
        {
            (void)(payload);
            (void)(size);

            unique_message_ptr pMessage{nullptr};

            switch (messageId) {
                case TestMessageTypeFirst: 
                    pMessage = std::make_unique<MessageFirst>();
                    break;
                case TestMessageTypeSecond: 
                    pMessage = std::make_unique<MessageSecond>();
                    break;
                case TestMessageTypeThird:
                    pMessage = std::make_unique<MessageThird>();
                    break;
                default:
                    break;
            }

            return pMessage;
        }

};

class TestRouter : public mrobot::messaging::Router<TestMessageTypeMax> {
    public:
        TestRouter() : Router(&m_testFactory) {
            registerHandler(TestMessageTypeFirst, &m_firstHandler);
            registerHandler(TestMessageTypeSecond, &m_secondHandler);
            registerHandler(TestMessageTypeThird, &m_thirdHandler);
        }

        virtual ~TestRouter() = default;

    private:
        TestMessagesFactory m_testFactory;
        MessageFirstHandler m_firstHandler;
        MessageSecondHandler m_secondHandler;
        MessageThirdHandler m_thirdHandler;
};


UNIT_TEST(MessageTestModule, BasicTest) {
}
