#include "mqtt_client.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <cstring>
#include <signal.h>


class MessageEcho final : public mrobot::messaging::Message {
    public:
        MessageEcho() = default;
        virtual ~MessageEcho() = default;

        bool deserialize(const unsigned char *payload, ::size_t size) override {
            try {
                m_data.reserve(size);
            } catch (...) {
                std::cout << "Failed allocating echo message" << '\n';
                return false;
            }

            ::memcpy(m_data.data(), payload, truncSize(size));
            return true;
        }

        std::vector<unsigned char> getData() const {
            return m_data;
        }

    private:
        std::vector<unsigned char> m_data;
        static const ::size_t maxSize = 128;
        static constexpr ::size_t truncSize(::size_t size) {
            return (size < maxSize) ? size : maxSize;
        }
};

class TestMqttRouter final : public mrobot::comm::MqttRouter {
    public:
        TestMqttRouter() : MqttRouter(MessageTypeMax) {}
        ~TestMqttRouter() = default;

    private:
        std::unique_ptr<mrobot::messaging::Message> factory(uint32_t messageId) const override {
            std::unique_ptr<mrobot::messaging::Message> message = nullptr;

            switch (messageId) {
                case MessageTypeEcho:
                    message = std::make_unique<MessageEcho>();
                    break;
                default:
                    std::cout << "Unknown message ID " << messageId << '\n';
                    break;
            }

            return message;
        }

        uint32_t topicToMessageId(const std::string &topic) const override {
            uint32_t messageId = MessageTypeMax;

            if (topic == "echo") {
                messageId = MessageTypeEcho;
            }

            return messageId;
        }

        enum MessageTypes {
            MessageTypeEcho,
            MessageTypeMax
        };
};

class Test {
    public:
        Test(std::string clientName, std::string brokerAddress, uint16_t brokerPort);
        ~Test() = default;

        bool start() const ;
        void stop();

    private:
        const std::string m_clientName;
        const std::string m_brokerAddress;
        const uint16_t m_brokerPort;
        const TestMqttRouter m_router;
        bool m_running;
};

Test::Test(std::string clientName, std::string brokerAddress, uint16_t brokerPort) :
    m_clientName(std::move(clientName)),
    m_brokerAddress(std::move(brokerAddress)),
    m_brokerPort(brokerPort),
    m_running(true) {
}

bool Test::start() const {
    std::cout << "Test MqTT client starting" << '\n';

    auto client = std::make_unique<mrobot::comm::MqttClient>(m_clientName,
                                                             m_brokerAddress,
                                                             m_brokerPort,
                                                             &m_router);
    if (!client) {
        std::cout << "Failed allocating new client" << '\n';
        return false;
    }

    if (!client->initialize()) {
        std::cout << "Failed initializing client" << '\n';
        return false;
    }

    if (!client->connect()) {
        std::cout << "Failed connecting to broker" << '\n';
        client->finalize();
        return false;
    }

    std::cout << "Running..." << '\n';

    while (m_running) {
    }

    client->finalize();

    return true;
}

void Test::stop() {
    m_running = false;
}

static Test *pTest = nullptr;

static void terminate(int signum)
{
    (void)(signum);

    if (pTest != nullptr) {
        pTest->stop();
    }
}

static void register_sigterm_handler()
{
    struct sigaction action;

    ::memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = terminate;
    sigaction(SIGINT, &action, NULL);
}

template <typename T>
static T to_string_priv(std::string s) {
    T r;
    std::stringstream ss;
    ss << s;
    ss >> r;
    return r;
}


int main(int argc, const char* argv[])
{
    if (argc < 4) {
        std::cout << "Please provide three input arguments: " << '\n';
        std::cout << argv[0] << " <client name> <broker url> <broker port>" << '\n';
        return 1;
    }

    Test test{argv[1], argv[2], to_string_priv<uint16_t>(std::string(argv[3]))};
    pTest = &test;

    register_sigterm_handler();
    test.start();

    return 0;
}
