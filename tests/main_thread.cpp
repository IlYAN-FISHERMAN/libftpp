/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_thread.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:51:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 13:15:17 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

void myFunction1() {
    for (int i = 0; i < 5; ++i) {
        lpp::cout << "Hello from Function1, iteration " << i << std::endl;
    }
}

void myFunction2() {
    for (int i = 0; i < 5; ++i) {
        lpp::cout << "Hello from Function2, iteration " << i << std::endl;
    }
}

int testThread() {
    lpp::thread thread1("Thread1: ", myFunction1);
    lpp::thread thread2("Thread2: ", myFunction2);

    thread1.start();
    thread2.start();

	lpp::cout << "simple test" << std::endl;

    thread1.stop();
    thread2.stop();

    return 0;
}
