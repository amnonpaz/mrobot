#include "gpio.hpp"

#include "UnitTest.hpp"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <unistd.h>

namespace filesystem = std::experimental::filesystem;

TEST_MODULE(GpioTestMoudle)

static const std::string getDummyGpioPath(uint32_t pin) {
    return std::string{std::string(SYSTEM_GPIO_PATH_PREFIX) + "gpio" + std::to_string(pin) + "/"};
}

static bool clearFile(const std::string &filename) {
    std::ofstream ofile;

    ofile.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (!ofile.is_open()) {
        return false;
    }

    ofile << "";
    bool res = ofile.good();
    ofile.close();

    return res;
}

static void clearDummyGpio() {
    filesystem::remove_all(SYSTEM_GPIO_PATH_PREFIX);
}

static bool createDummyGpio(uint32_t pin) {
    clearDummyGpio();

    const std::string gpioDummyPath{getDummyGpioPath(pin)};

    bool res = filesystem::create_directories(gpioDummyPath);
    if (!res) {
        return false;
    }

    res = clearFile(std::string(SYSTEM_GPIO_PATH_PREFIX) + "export");
    if (!res) {
        return false;
    }

    res = clearFile(gpioDummyPath + "direction");
    if (!res) {
        return false;
    }

    return clearFile(gpioDummyPath + "value");
}

static bool readFile(const std::string &filename, std::string &data) {
    std::ifstream ifile;

    ifile.open(filename);
    if (!ifile.is_open()) {
        return false;
    }

    ifile >> data;
    bool res = ifile.eof();
    ifile.close();

    return res;
}

static bool verifyExport(uint32_t pin) {
    const std::string exportEntry{std::string(SYSTEM_GPIO_PATH_PREFIX) + "export"};
    std::string data;

    if (!readFile(exportEntry, data)) {
        std::cout << "Error reading entry file" << '\n';
        return false;
    }

    clearFile(exportEntry);

    return data == std::to_string(pin);
}

static bool verifyDirection(uint32_t pin, mrobot::hw::Gpio::Direction dir) {
    const std::string directionDummyPath{getDummyGpioPath(pin) + "direction"};
    std::string data;

    if (!readFile(directionDummyPath, data)) {
        std::cout << "Error reading direction file" << '\n';
        return false;
    }

    clearFile(directionDummyPath);

    const std::string expected{(dir == mrobot::hw::Gpio::DirectionOut) ? "out" : "in"};

    return data == expected;
}

static bool verifyValue(uint32_t pin, bool expected) {
    const std::string valueDummyPath{getDummyGpioPath(pin) + "value"};
    std::string data;

    ::sync();

    if (!readFile(valueDummyPath, data)) {
        std::cout << "Error reading value file" << '\n';
        return false;
    }

    clearFile(valueDummyPath);

    std::stringstream sstream{data};
    uint32_t value;
    sstream >> value;


    return value == (expected ? 1 : 0);
}

UNIT_TEST(GpioTestMoudle, BasiciInputGpio) {
    const uint32_t pin = 352;

    bool res = createDummyGpio(pin);
    EXPECT_EQUAL(res, true);
    if (!res) {
        return;
    }

    mrobot::hw::Gpio gpio(pin, mrobot::hw::Gpio::DirectionOut);
    EXPECT_EQUAL(gpio.init(), true);

    EXPECT_EQUAL(verifyExport(pin), true);
    EXPECT_EQUAL(verifyDirection(pin, mrobot::hw::Gpio::DirectionOut), true);

    EXPECT_EQUAL(gpio.set(true), true);
    EXPECT_EQUAL(verifyValue(pin, true), true);

    EXPECT_EQUAL(gpio.set(false), true);
    EXPECT_EQUAL(verifyValue(pin, false), true);

    clearDummyGpio();
}
