#ifndef DINO_TESTS_TEST_FRAMEWORK_H_
#define DINO_TESTS_TEST_FRAMEWORK_H_

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace dino::testing {

struct TestCase {
    std::string name;
    std::function<void()> func;
};

class TestRegistry {
public:
    static TestRegistry& instance() {
        static TestRegistry reg;
        return reg;
    }

    void add_test(std::string name, std::function<void()> func) {
        tests_.push_back(TestCase{std::move(name), std::move(func)});
    }

    int run_all() {
        int passed = 0;
        int failed = 0;
        std::cout << "[==========] Running " << tests_.size() << " test cases.\n";
        for (const auto& test : tests_) {
            std::cout << "[ RUN      ] " << test.name << '\n';
            try {
                test.func();
                std::cout << "[       OK ] " << test.name << '\n';
                passed++;
            } catch (const std::exception& ex) {
                std::cout << "[  FAILED  ] " << test.name << ": " << ex.what() << '\n';
                failed++;
            } catch (...) {
                std::cout << "[  FAILED  ] " << test.name << ": Unknown exception\n";
                failed++;
            }
        }
        std::cout << "[==========] " << passed << " passed, " << failed << " failed.\n";
        return failed == 0 ? 0 : 1;
    }

private:
    std::vector<TestCase> tests_;
};

struct TestRegistrar {
    TestRegistrar(std::string name, std::function<void()> func) {
        TestRegistry::instance().add_test(std::move(name), std::move(func));
    }
};

class TestFailure : public std::exception {
public:
    explicit TestFailure(std::string msg) : msg_(std::move(msg)) {}
    [[nodiscard]] const char* what() const noexcept override { return msg_.c_str(); }
private:
    std::string msg_;
};

}  // namespace dino::testing

#define DINO_TEST_CASE(Name) \
    static void Name(); \
    static ::dino::testing::TestRegistrar registrar_##Name(#Name, Name); \
    static void Name()

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            throw ::dino::testing::TestFailure(std::string("Assertion failed: ") + #cond + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
        } \
    } while (false)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_EQ(a, b) \
    do { \
        if (!((a) == (b))) { \
            throw ::dino::testing::TestFailure(std::string("Assertion failed: ") + #a + " == " + #b + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
        } \
    } while (false)

#define ASSERT_NE(a, b) \
    do { \
        if (!((a) != (b))) { \
            throw ::dino::testing::TestFailure(std::string("Assertion failed: ") + #a + " != " + #b + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
        } \
    } while (false)

#endif  // DINO_TESTS_TEST_FRAMEWORK_H_
