enable_testing()

# GTest
find_package(GTest REQUIRED)

add_executable(lib main_pool.cpp testMain.cc)

# target_link_libraries(lib libftpp GTest::gtest GTest::gtest_main pthread)

add_test(NAME LibftppTests COMMAND libftpp_tests)

include(FetchContent)
