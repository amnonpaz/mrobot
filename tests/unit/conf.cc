#ifndef DUMMY_CONF_PATH
#error DUMMY_CONF_PATH not defined
#endif

#include "conf.hpp"

#include "UnitTest.hpp"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

TEST_MODULE(ConfTestMoudle)

static void clearDummyPath() {
    filesystem::remove_all(DUMMY_CONF_PATH);
}

static bool createDummyPath() {
    return filesystem::create_directories(DUMMY_CONF_PATH);
}

static bool writeConfFile(const std::string &filename,
                          const std::string data) {
    std::ofstream ofile;

    ofile.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (!ofile.is_open()) {
        return false;
    }

    ofile << data;
    bool res = ofile.good();
    ofile.close();

    return res;
    
}

UNIT_TEST(ConfTestMoudle, BasicPassableConf) {
    static const std::string filename{std::string(DUMMY_CONF_PATH) + "conf.txt"};
    static const std::string data{
        "first = 1st\n"
        "  second   =   2nd   \n"
        "#third = 3rd\n"
        "   #third = 3rd\n"
        "   # third = 3rd\n"
        "fourth = 4th"};

    EXPECT_EQUAL(createDummyPath(), true);

    writeConfFile(filename, data);

    mrobot::conf::Database config;

    EXPECT_EQUAL(config.addFile(filename), true);

    EXPECT_EQUAL(config.get("first"), "1st");
    EXPECT_EQUAL(config.get("second"), "2nd");
    EXPECT_STRING_EMPTY(config.get("third"));
    EXPECT_EQUAL(config.get("fourth"), "4th");

    clearDummyPath();
}

UNIT_TEST(ConfTestMoudle, NoDelimiter) {
    static const std::string filename{std::string(DUMMY_CONF_PATH) + "conf.txt"};
    static const std::string data{
        "first = 1st\n"
        "  second      2nd   \n"
        "#third = 3rd\n"
        "   #third = 3rd\n"
        "   # third = 3rd\n"
        "fourth=4th"};

    EXPECT_EQUAL(createDummyPath(), true);

    writeConfFile(filename, data);

    mrobot::conf::Database config;

    EXPECT_EQUAL(config.addFile(filename), false);

    clearDummyPath();
}

UNIT_TEST(ConfTestMoudle, NoKey) {
    static const std::string filename{std::string(DUMMY_CONF_PATH) + "conf.txt"};
    static const std::string data{
        "first = 1st\n"
        "  second  =    2nd   \n"
        "#third = 3rd\n"
        "   #third = 3rd\n"
        "   # third = 3rd\n"
        "= 4th"};

    EXPECT_EQUAL(createDummyPath(), true);

    writeConfFile(filename, data);

    mrobot::conf::Database config;

    EXPECT_EQUAL(config.addFile(filename), false);

    clearDummyPath();
}

UNIT_TEST(ConfTestMoudle, NoValue) {
    static const std::string filename{std::string(DUMMY_CONF_PATH) + "conf.txt"};
    static const std::string data{
        "first =\n"
        "  second  =    2nd   \n"
        "#third = 3rd\n"
        "   #third = 3rd\n"
        "   # third = 3rd\n"
        "forth= 4th"};

    EXPECT_EQUAL(createDummyPath(), true);

    writeConfFile(filename, data);

    mrobot::conf::Database config;

    EXPECT_EQUAL(config.addFile(filename), false);

    clearDummyPath();
}

UNIT_TEST(ConfTestMoudle, RepeatingKey) {
    static const std::string filename{std::string(DUMMY_CONF_PATH) + "conf.txt"};
    static const std::string data{
        "first =\n"
        "  second  =    2nd   \n"
        "second=2nd\n"
        "#third = 3rd\n"
        "   #third = 3rd\n"
        "   # third = 3rd\n"
        "forth= 4th"};

    EXPECT_EQUAL(createDummyPath(), true);

    writeConfFile(filename, data);

    mrobot::conf::Database config;

    EXPECT_EQUAL(config.addFile(filename), false);

    clearDummyPath();
}
