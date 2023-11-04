#ifndef TEST_MACRO_H_
#define TEST_MACRO_H_
#define TEST(name, test_func) \
    printf("\x1B[1;34mRunning test: %s\x1B[0m\n", name); \
    test_func(); \
    printf("\x1B[1;32mTest %s passed\x1B[0m\n", name);
#endif // !TEST_MACRO_H_
