/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_thread_safe_queue.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:51:33 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 13:18:27 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

void testPush(lpp::thread_safe_queue<int>& p_queue, int p_value) {
	// std::thread::id it = std::this_thread::get_id();
	// std::string id = std::to_string(reinterpret_cast<unsigned long>(&it));
	// lpp::cout.setPrefix(id + ": ");
    p_queue.push_back(p_value);
    lpp::cout << "Pushed value: " << p_value << std::endl;
}

void testPop(lpp::thread_safe_queue<int>& p_queue) {
	// std::thread::id it = std::this_thread::get_id();
	// std::string id = std::to_string(reinterpret_cast<unsigned long>(&it));
	// lpp::cout.setPrefix(id + ": ");
	// std::this_thread::sleep_for(std::chrono::seconds(1));
    try {
		lpp::cout << std::boolalpha << p_queue.empty() << std::endl;
        int value = p_queue.pop_front();
        lpp::cout << "Popped value: " << value << std::endl;
    } catch (const std::runtime_error& e) {
        lpp::cout << e.what() << std::endl;
    }
}

int testthread_safe_queue() {
	lpp::thread_safe_queue<int> myQueue;

    std::thread thread1(testPush, std::ref(myQueue), 10);
    std::thread thread2(testPush, std::ref(myQueue), 20);
    std::thread thread3(testPop, std::ref(myQueue));
    std::thread thread4(testPop, std::ref(myQueue));
    std::thread thread5(testPop, std::ref(myQueue));

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    return 0;
}

