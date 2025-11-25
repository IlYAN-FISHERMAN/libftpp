// #include <gtest/gtest.h>
#include "tester.hh"

/// Only for debugging, interaction mode with command line
// TEST(IoMap, testWithInteraction) {
//      EXPECT_EQ(testInteractiveIoMap(), 0);
// }

/// Only for debugging, interaction mode with command line
// TEST(IoMap, testWithInteraction) {
//      EXPECT_EQ(testIoAggregateMapInteract(), 0);
// }

int main(int ac, char **av) {
        srand(static_cast<unsigned int>(time(NULL)));
        int code = 0;
        (void)ac;
        (void)av;
        (void)code;

        code = testPool();
        std::cout << "code: " << code << std::endl;
    // ::testing::InitGoogleTest(&ac, av);
    // return RUN_ALL_TESTS();
}
