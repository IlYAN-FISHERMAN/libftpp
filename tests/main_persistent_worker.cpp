/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_persistent_worker.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 16:07:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 13:14:03 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testpersistent_worker() {
   lpp:: persistent_worker worker;
	std::shared_ptr<IoStat> io = std::make_shared<IoStat>(1, "eos", 1, 1);

    auto task1 = []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        lpp::cout << "Executing Task 1" << std::endl;
    };

    auto task2 = []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        lpp::cout << "Executing Task 2" << std::endl;
    };

    worker.addTask("Task1", task1);
    worker.addTask("Task1", []() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
        lpp::cout << "Executing Task 1.1" << std::endl;
    });

    worker.addTask("Task2", task2);
	worker.addTask("Task1", []() {
        lpp::cout << "i\'m doing something" << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));

    worker.removeTask("Task1");


    std::this_thread::sleep_for(std::chrono::seconds(2));

    worker.addTask("io", io);
	lpp::cout << "containe io: " << std::boolalpha << worker.containe("io") << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
	worker.removeTask("io");
	lpp::cout << "containe io: " << std::boolalpha << worker.containe("io") << std::endl;

	io->execute();
	lpp::cout << *io << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
