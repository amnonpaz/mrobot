#include "mqtt_client.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <cstring>
#include <signal.h>

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

    std::unique_ptr<mrobot::comm::Client> client = std::make_unique<mrobot::comm::MqttClient>(m_clientName, m_brokerAddress, m_brokerPort);
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
