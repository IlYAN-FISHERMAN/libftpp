/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_persistent_worker.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 16:07:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/26 20:24:07 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testPersistentWorker() {
    PersistentWorker worker;
	std::shared_ptr<IoStat> io = std::make_shared<IoStat>(1, "eos", 1, 1);

    auto task1 = []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        threadSafeCout << "Executing Task 1" << std::endl;
    };

    auto task2 = []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        threadSafeCout << "Executing Task 2" << std::endl;
    };

    worker.addTask("Task1", task1);
    worker.addTask("Task1", []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        threadSafeCout << "Executing Task 1.1" << std::endl;
    });

    worker.addTask("Task2", task2);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    worker.removeTask("Task1");


    std::this_thread::sleep_for(std::chrono::seconds(2));

    worker.addTask("io", io);

    std::this_thread::sleep_for(std::chrono::seconds(2));
	worker.removeTask("io");

	io->execute();
	threadSafeCout << *io << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
