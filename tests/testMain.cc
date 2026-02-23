/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 17:30:33 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

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
	// code = myTestPool();
	// code = deleteTestPool();
	// code = benchPoolTest();

	// code = testDataBuffer();
	
	// code = testMemento();
	// code = myTestMemento();

	// code = testObserver();
	// code = myTestObserver();

	// code = testSingleton();

	// code = testStateMachine();
	// code = myTestStateMachine();
	
	// code = testThreadSafeIostream();
	// code = myTestThreadSafeIostream();

	// code = testThreadSafeQueue();

	code = testThread();

	std::cout << "code: " << code << std::endl;
	return code;
}
