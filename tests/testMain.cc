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

int main(void) {
	srand(static_cast<unsigned int>(time(NULL)));
	int code = 0;

	// code = testPool();
	code = myTestPool();
	std::cout << "code: " << code << std::endl;
	return code;
}
