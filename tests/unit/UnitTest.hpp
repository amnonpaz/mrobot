#ifndef _UNITTESTS_H
#define _UNITTESTS_H

#ifndef stringify
#define stringify(s) #s
#else
#warning stringify is already defined
#endif

#include <string>
#include <array>
#include <list>
#include <iostream>
#include <cstdint>

namespace testing {

class UnitTest;

class TestsContainer {
    public:
        TestsContainer() = default; 
        virtual ~TestsContainer() = default; 

        virtual void addTest(UnitTest *test) = 0;
};

class UnitTest {
    public:
        explicit UnitTest(TestsContainer *container, const std::string name) :
            m_result(true), 
            m_name(std::move(name)),
            m_executed(false) {
                container->addTest(this);
        }
        virtual ~UnitTest() = default;

        void run() {
            test();
            m_executed = true;
        }

        const std::string &getName() const { return m_name; }
        bool wasExecuted() const { return m_executed; }
        bool getResult() const { return m_result; }

        static const std::string &resultToString(bool res) {
            static const std::array<std::string, 2> strings{"FAIL","PASS"};

            return strings[res ? 1 : 0];
        }

    protected:
        bool m_result;

    private:
        virtual void test() = 0;

        const std::string m_name;
        bool m_executed;
};

#define EXPECT_EQUAL(exp, res) {            \
        uint32_t line = __LINE__;           \
        bool local_res = (exp) == (res);    \
        if (!local_res) {                   \
            std::cout << "[+] Error on " << __FILE__ << ":" << std::to_string(line) << '\n' << \
                "    Expected: " << stringify(exp) << " == " << stringify(res) << '\n';  \
        }                                   \
        m_result &= local_res;              \
    }

#define UNIT_TEST(S, T)                                 \
    class __##T : public testing::UnitTest {            \
        public:                                         \
            explicit __##T(testing::TestsContainer *C) :         \
                testing::UnitTest{C, stringify(T)} {}   \
        private:                                        \
            void test() override;                       \
    };                                                  \
    __##T T{&S};                                        \
    void __##T::test()

class TestModule : public TestsContainer {
    public:
        explicit TestModule(const std::string name) : m_name(std::move(name)) {}
        virtual ~TestModule() = default;

        bool run() {
            bool res = true;
            std::cout << "=== Test Module " << m_name << " ===\n";
            for (auto *test : m_tests) {
                std::cout << "Executing " << m_name << "::" << test->getName() << '\n';
                test->run();
                std::cout <<  UnitTest::resultToString(test->getResult()) << " " <<
                    m_name << "::" << test->getName() << '\n';
                res &= test->getResult();
            }

            printLog();

            return res;
        }

        void addTest(UnitTest *test) override {
            m_tests.push_back(test);
        }
        
    private:
        void printLog() {
            std::cout << '\n';
            std::cout << "=== " << m_name << " Summary ===\n";
            for (auto *test : m_tests) {
                std::cout << "  - " << test->getName() << ": " << UnitTest::resultToString(test->getResult()) << '\n';
            }
        }
        
        const std::string m_name;

        std::list<UnitTest *> m_tests;
    
};

#define TEST_MODULE(S)                      \
    testing::TestModule S{stringify(S)};    \
    int main() { return S.run() ? 0 : 1; }

} // namespace testing

#endif // _UNITTESTS_H
