#ifndef TEST_HH
#define TEST_HH

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

namespace TEST {

    static constexpr const int LENGTH = 80;

    static constexpr const char *COLOR_RESET = "\x1B[0m";
    static constexpr const char *COLOR_RED = "\x1B[31m";
    static constexpr const char *COLOR_GREEN = "\x1B[32m";

#define cprintf(color, ...)        \
    {                              \
        printf("%s", color);       \
        printf(__VA_ARGS__);       \
        printf("%s", COLOR_RESET); \
    }

    static void print_centered(const char *text) {
        float len = (LENGTH - 2 - strlen(text)) / 2.0f;
        for (int i = 0; i < floorf(len); i++) printf("=");
        printf(" %s ", text);
        for (int i = 0; i < ceilf(len); i++) printf("=");
        printf("\n");
    }

    static void print_separator() {
        for (int i = 0; i < LENGTH; i++) printf("=");
        printf("\n");
    }

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

    static std::string to_lower_case(std::string string) {
        std::transform(string.begin(), string.end(), string.begin(), [](auto ch) { return std::tolower(ch); });
        return string;
    }

    static bool should_run(const std::string &test_suite_name) {
#ifndef TEST
        (void) test_suite_name;
        (void) to_lower_case;
        return true;
#else
        return to_lower_case(test_suite_name) == to_lower_case(STRINGIFY(TEST));
#endif
    }

#define TEST_SUITE(TEST_SUITE_NAME, ...)                    \
    namespace {                                             \
        TEST::Runner runner([]() {                          \
            if (!TEST::should_run(TEST_SUITE_NAME)) return; \
            TEST::TestSuite __test_suite(TEST_SUITE_NAME);  \
            {__VA_ARGS__};                                  \
        });                                                 \
    }

    class failed_assert : public std::runtime_error {
    public:
        failed_assert() : std::runtime_error("") {}
    };

#define TEST_CASE(TEST_CASE_NAME, ...)                                                           \
    {                                                                                            \
        TEST::TestCase __test_case = __test_suite.test_case(TEST_CASE_NAME, __FILE__, __LINE__); \
        try {                                                                                    \
            {__VA_ARGS__};                                                                       \
            if (__test_case.expects_num != 1) __test_suite.abort_expects();                      \
        } catch (const TEST::failed_assert &) {                                                  \
            printf("failed assert\n");                                                           \
        } catch (const std::exception &ex) {                                                     \
            printf("%s\n", ex.what());                                                           \
            __test_suite.abort_exception();                                                      \
        }                                                                                        \
    }

#define ASSERT(CONDITION)                                \
    if ((CONDITION) == false) {                          \
        __test_suite.abort_assert(#CONDITION, __LINE__); \
        throw TEST::failed_assert();                     \
    }

#define EXPECT(CONDITION)                         \
    __test_case.expects_num++;                    \
    if ((CONDITION) == true) __test_suite.pass(); \
    else __test_suite.fail();

    struct TestCase {
        const char *test_name, *file;
        int line, expects_num;
    };

    class TestSuite {
    public:
        TestSuite(const char *test_suite) : passed(0), failed(0), aborted(0) { print_centered(test_suite); }
        ~TestSuite() {
            print_centered("Summary");
            cprintf(COLOR_GREEN, "Passed: %d\n", passed);
            cprintf(COLOR_RED, "Failed: %d\n", failed);
            cprintf(COLOR_RED, "Aborted: %d\n", aborted);
            print_separator();
        }

        inline TestCase &test_case(const char *test_name, const char *file, int line) {
            current_test = {test_name, file, line, 0};
            return current_test;
        }

        inline void pass() {
            passed++;
            cprintf(COLOR_GREEN, "[PASSED] %s\n", current_test.test_name);
        }

        inline void fail() {
            failed++;
            cprintf(COLOR_RED, "[FAILED] %s\n", current_test.test_name);
        }

        inline void abort_assert(const char *condition, int line) {
            abort();
            cprintf(COLOR_RED, "failed assert(%s) at line %d\n", condition, line);
        }

        inline void abort_expects() {
            abort();
            passed -= current_test.expects_num;
            cprintf(COLOR_RED, "test must have only one expect, but has %d\n", current_test.expects_num);
        }

        inline void abort_exception() {
            abort();
            cprintf(COLOR_RED, "test contains an uncaught exception\n");
        }

    private:
        int passed, failed, aborted;
        TestCase current_test;

        inline void abort() {
            aborted++;
            cprintf(COLOR_RED, "[ABORTED] %s(%s#%d): ", current_test.test_name, current_test.file, current_test.line);
        }
    };

    class Runner {
    public:
        template <typename Fn> Runner(Fn callback) { callback(); }
    };

#undef cprintf
#undef _STRINGIFY
#undef STRINGIFY

}  // namespace TEST

#endif  // TEST_HH
