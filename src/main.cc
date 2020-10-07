#include "robot.hpp"

#include <iostream>
#include <string.h>
#include <algorithm>

#define MAX_CONF_FILENAME_LEN 256UL

int main(int argc, const char *argv[])
{
    std::cout << "Mayyan's Robot v" << VERSION << std::endl;

    if (argc < 2) {
        std::cout << "Please provide configuration file name" << '\n';
        return 1;
    }

    const std::string confFilename{argv[1], std::min(::strlen(argv[1]), MAX_CONF_FILENAME_LEN)};

    mrobot::Robot robot(confFilename);

    robot.init();

    return 0;
}
