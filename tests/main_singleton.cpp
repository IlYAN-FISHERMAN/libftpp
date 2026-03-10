/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_singleton.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:50:44 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/10 15:37:17 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

class MyClass {
public:
	MyClass(int value)
	{
		std::cout << "MyClass constructor, with value [" << value << "]" << std::endl;
	}

    void printMessage() {
        std::cout << "Hello from MyClass" << std::endl;
    }
};

int testSingleton() {
	lpp::unique_chrono chrono("singleton");
    try
    {
        // This should throw an exception as instance is not yet created
        lpp::singleton<MyClass>::instance();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl; // Output: "Exception: Instance not yet created"
    }

    lpp::singleton<MyClass>::instantiate(42); // Setting up the instance

    lpp::singleton<MyClass>::instance()->printMessage(); // Output: "Hello from MyClass"

    try
    {
        // This should throw an exception as instance is already created
        lpp::singleton<MyClass>::instantiate(100);
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl; // Output: "Exception: Instance already created"
    }

    return 0;
}
