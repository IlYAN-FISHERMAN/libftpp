/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_worker_pool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:51:47 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/26 13:45:31 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testWorkerPool() {
    WorkerPool pool(25);

    auto job = []() {
		volatile long long a = 0, b = 1;
		for (int i = 0; i < 50'000'000; ++i) {
			long long c = a + b;
			a = b;
			b = c;
		}
    };

	// auto mixedJob = [] {
	// 	volatile long long x = 0;
	// 	for (int i = 0; i < 10'000'000; ++i) x += i;
	// 		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	// };

	// auto cpuJob = []{
 //    volatile double v = 0;
 //    for (int i = 0; i < 200'000'000; i++)
 //        v += std::sin(i);  // CPU lourd
	// };

	// auto ioJob = [] {
 //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// };

    for (int i = 0; i < 1000; ++i) {
        pool.addJob(job);
    }

	threadSafeCout << "bc jobs running: " << pool.running() << std::endl;
	pool.wait();
	threadSafeCout << "bc jobs running: " << pool.running() << std::endl;

	// pool.wait();

	std::shared_ptr<IoStat> io(std::make_shared<IoStat>(1, "eos", 1, 1));
    //
    for (int i = 0; i < 100; ++i)
		pool.addJob(io);
	threadSafeCout << "bc jobs running: " << pool.running() << std::endl;

	// pool.wait();
	// pool.addJob(io);
	// pool.addJob(io);

	threadSafeCout << std::endl << "[END]\n" << *io.get() << std::endl;
	//
    return 0;
}
