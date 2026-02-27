/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:19:24 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 18:51:48 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testServer() {
    Server server;

    // Define an action for messages of type 1 (int)
    server.defineAction(Message::Type::INT, [&server](long long& clientID, const Message& msg){
        int value;
        msg >> value;
        threadSafeCout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 3 with double the value
        Message replyMsg(3);
        replyMsg << (value * 2);
        server.sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    server.defineAction(Message::Type::SIZE_T, [](long long& clientID, const Message& msg){
        size_t length;
        std::string text;
        msg >> length;
        text.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            char c;
            msg >> c;
            text.push_back(c);
        }
        threadSafeCout << "Received a string '" << text << "' of length " << length << " from client " << clientID << std::endl;
    });

    // Start the server on port 8080
    server.start(8080);

   	bool quit = false;

    Client client;

	client.defineAction(Message::Type::DOUBLE, [](const Message& msg){
        int doubledValue;
        msg >> doubledValue;
        threadSafeCout << "Received a doubled value: " << doubledValue << std::endl;
    });

    // Connect to the server
    client.connect("localhost", 8080);

    // Send a message of type 1 (int)
    Message message1(1);
    message1 << 42;
    client.send(message1);

    // Send a message of type 2 (size_t followed by characters)
    Message message2(2);
    std::string str = "Hello";
    message2 << str.size();
    for (char c : str) {
        message2 << c;
    }
    client.send(message2);

	while (!quit)
	{
		client.update();

		threadSafeCout << "Client updated." << std::endl;
		threadSafeCout << "Available operations :" << std::endl;
		threadSafeCout << " - [Q]uit : close the program" << std::endl;
		threadSafeCout << " - Any other input to continue updating the client" << std::endl;

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

	client.connect("localhost", 8080);
	while (!quit)
	{
		client.update();

		threadSafeCout << "Server updated." << std::endl;
		threadSafeCout << "Available operations :" << std::endl;
		threadSafeCout << " - [Q]uit : close the program" << std::endl;
		threadSafeCout << " - Any other input to continue updating the server" << std::endl;

		std::string input;
		threadSafeCout.prompt("", input);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input == "quit" || (input.length() == 1 && input[0] == 'q')) {
		    quit = true;
		}
	}
	
    return 0;
}

