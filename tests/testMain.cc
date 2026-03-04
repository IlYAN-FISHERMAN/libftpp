/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:07:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/04 11:17:51 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

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

	// code = testThread();

	// code = testWorkerPool();

	code = testPersistentWorker();

	// code = testClient();

	// code = testServer();

	std::cout << "code: " << code << std::endl;
	return code;
}
