#include "robot.hpp"

#include <iostream>
#include <string.h>
#include <algorithm>
#include <signal.h>

static constexpr size_t s_maxConfFilenameLen = 256;

static mrobot::Robot *pRobot = nullptr;

static void terminate(int signum)
{
    (void)(signum);

    if (pRobot != nullptr) {
        pRobot->stop();
    }
}

static void registerSigtermHandler()
{
    struct sigaction action;

    ::memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = terminate;
    sigaction(SIGINT, &action, NULL);
}

int main(int argc, const char *argv[])
{
    std::cout << "Maayan's Robot v" << VERSION << std::endl;

    if (argc < 2) {
        std::cout << "Please provide configuration file name" << '\n';
        return 1;
    }

    const std::string confFilename{argv[1], std::min(::strlen(argv[1]), s_maxConfFilenameLen)};

    mrobot::Robot robot(confFilename);
    pRobot = &robot;

    if (!robot.initialize()) {
        std::cout << "Failed initializing robot" << '\n';
        return 2;
    }

    registerSigtermHandler();
    if (!robot.run()) {
        std::cout << "Failed running robot" << '\n';
        return 2;
    }

    std::cout << "Exiting" << '\n';

    robot.finalize();

    return 0;
}
