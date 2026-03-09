/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 13:50:01 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 13:12:50 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testClient() {
	lpp::client client;

	client.defineAction(1, [](const lpp::message& msg){
        int doubledValue;
        msg >> doubledValue;
        lpp::cout << "Received a doubled value: " << doubledValue << std::endl;
    });

    // Connect to the server
    client.connect("localhost", 8080);

    // Send a message of type 1 (int)
    lpp::message message1(1);
    message1 << 42;
    client.send(message1);

    // Send a message of type 2 (size_t followed by characters)
    lpp::message message2(2);
    std::string str = "Hello";
    message2 << str.size();
    for (char c : str) {
        message2 << c;
    }
    client.send(message2);

	bool quit = false;

	while (!quit)
	{
		lpp::cout << "Client updated." << std::endl;
		lpp::cout << "Available operations :" << std::endl;
		lpp::cout << " - [Q]uit : close the program" << std::endl;
		lpp::cout << " - Any other input to continue updating the client" << std::endl;

		std::string input;
		std::getline(std::cin, input);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input == "quit" || (input.length() == 1 && input[0] == 'q')) {
		    quit = true;
		}
	}

    // Disconnect from the server
    client.disconnect();

    return 0;
}
