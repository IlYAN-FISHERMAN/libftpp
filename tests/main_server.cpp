/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:19:24 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/18 17:17:31 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testServer() {
    lpp::server server;
    lpp::client client;
	lpp::unique_chrono chrono("server");

    // Define an action for messages of type 1 (int)
    server.defineAction(1, [&server](long long clientID, const lpp::message& msg){
        int value;
        msg >> value;
        lpp::cout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 1 with double the value
        lpp::message replyMsg(1);
        replyMsg << (value * 2);
        server.sendTo(replyMsg, clientID);
    });

    server.defineAction(10, [&server](long long clientID, const lpp::message& msg){
		std::string value;
        lpp::message replyMsg(10);

        msg >> value;
		if (value != "ping")
			replyMsg << "ping?";
		else{
			replyMsg << "pong";
		}

        server.sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    server.defineAction(2, [&server](long long clientID, const lpp::message& msg){
        size_t length = 0;
        std::string text;
        msg >> length;
        text.resize(length);
		for (size_t i = 0; i < length; ++i)
			msg >> text[i];

        lpp::cout << "Received a string '" << text << "' of length " << length << " from client " << clientID << std::endl;
		lpp::message tmp(2);
		tmp << "message received";
		server.sendTo(tmp, clientID);
    });

	server.defineAction(31, [](long long clientId, const lpp::message& msg){
		lpp::cout << "test with client[" << clientId << "]" << std::endl;
		(void)msg;
	});

    // Start the server on port 8080
	try{
		server.start(8080);
	} catch(std::exception &e){
		lpp::cout << "server error: " << e.what() << std::endl;
		return(-1);
	}

	try{
		client.connect("127.0.0.1", 8080);
	} catch(std::exception &e){
		lpp::cout << "client error: " << e.what() << std::endl;
		return(-1);
	}

	client.defineAction(1, [](const lpp::message& msg){
        int doubledValue = 0;
		msg >> doubledValue;
        lpp::cout << "Received a doubled value: " << doubledValue << std::endl;
    });

    client.defineAction(2, [](const lpp::message& msg){
		lpp::cout << "server say \"" << msg.str() << "\"" << std::endl;
    });

	client.defineAction(4, [](const lpp::message& msg){
		std::string str;
        msg >> str;
        lpp::cout << "client received : " << str << std::endl;
    });

	client.defineAction(10, [](const lpp::message& msg){
		std::string str;
        msg >> str;
		lpp::cout << str << std::endl;
		if (str == "pong")
			lpp::cout << "server work!" << std::endl;
    });

	lpp::prompt prompt(client);
	prompt.run();

    return 0;
}

