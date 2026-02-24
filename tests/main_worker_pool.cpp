/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_worker_pool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:51:47 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 17:06:22 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testWorkerPool() {
    WorkerPool pool(4);

    // auto job = []() {
    //     threadSafeCout << "Executing job on thread: " << std::this_thread::get_id() << std::endl;
    // };
    //
    // for (int i = 0; i < 1000; ++i) {
    //     pool.addJob(job);
    // }

    // std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for jobs to finish
	std::shared_ptr<IoStat> io(std::make_shared<IoStat>(1, "eos", 1, 1));

    for (int i = 0; i < 100; ++i)
		pool.addJob(io);

	pool.wait();
	pool.addJob(io);

	threadSafeCout << std::endl << "[END]\n" << *io.get() << std::endl;

    return 0;
}
