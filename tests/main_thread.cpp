/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_thread.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:51:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 15:52:46 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

void myFunction1() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function1, iteration " << i << std::endl;
    }
}

void myFunction2() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function2, iteration " << i << std::endl;
    }
}

int testThread() {
    Thread thread1("Thread1: ", myFunction1);
    Thread thread2("Thread2: ", myFunction2);

    thread1.start();
    thread2.start();

	threadSafeCout << "simple test" << std::endl;

    thread1.stop();
    thread2.stop();

    return 0;
}
