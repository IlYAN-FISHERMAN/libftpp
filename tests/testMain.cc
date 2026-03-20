/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/20 14:38:09 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int main(int ac, char **av) {
	srand(static_cast<unsigned int>(time(NULL)));
	int code = 0;
	(void)ac;
	(void)av;

	// code = testPool();
	// code = myTestPool();
	// code = deleteTestPool();
	// code = benchPoolTest();
	// code = testDataBuffer();
//
	// code = testMemento();
	// code = myTestMemento();
	// code = testObserver();
	// code = myTestObserver();
	// code = testSingleton();
	// code = testStateMachine();
	// code = myTestStateMachine();
//
	// code = testThreadSafeIostream();
	// code = myTestThreadSafeIostream();
//
	// code = testThreadSafeQueue();
	// code = testThread();
	// code = testWorkerPool();
	// code = testPersistentWorker();

	// code = testServer();
	//
	// code = testIVector2();
	// code = testIVector3();
	// code = testRandom_2D_coordinate_generator();
	// code = testPerlin_noise_2D();
	//
	// code = testArenaAllocator();
	// code = testResponsabilityChain();
	// code = testNonCopyable();
	// code = testUniqueMemento();
	// code = testTimer();

	code = matt_daemon(ac, av);
	lpp::cout << "return code: " << code << std::endl;
	return code;
}
