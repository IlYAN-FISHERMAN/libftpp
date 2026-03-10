/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:19:24 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/10 15:15:06 by ilyanar          ###   LAUSANNE.ch       */
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
        size_t length;
        std::string text;
        msg >> length;
        text.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            char c;
            msg >> c;
            text.push_back(c);
        }
        lpp::cout << "Received a string '" << text << "' of length " << length << " from client " << clientID << std::endl;
		lpp::message tmp(2);
		tmp << "message received";
		server.sendTo(tmp, clientID);
    });

    // Start the server on port 8080
	try{
		server.start(8080);
	} catch(std::runtime_error &e){
		lpp::cout << "server: " << e.what() << std::endl;
		exit(-1);
	}

	try{
		client.connect("127.0.0.1", 8080);
	} catch(std::runtime_error &e){
		lpp::cout << "client: " << e.what() << std::endl;
		return(-1);
	}

	client.defineAction(1, [](const lpp::message& msg){
        int doubledValue;
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

	lpp::cout << "Available operations :" << std::endl;
	lpp::cout << " - [Q]uit|exit : close the program" << std::endl;
	lpp::cout << " - Any other input to continue updating the client" << std::endl;
	lpp::cout << "Usage - (code)|(input), (string)" << std::endl << std::endl;
	lpp::cout.setPrefix("[Client]: ");
	while (true)
	{
		std::string input;
		lpp::cout.prompt("libftpp-> ", input);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input == "quit" || input == "q" || input == "exit")
			break;
		else if(input == "clear" || input == "c"){
			lpp::cout << "\033c" << std::flush;
		} else if (!input.empty()){
			if (input.find('|') == input.npos){
				client.send(input);
			} else {
				int code = 0;
				char sep = 0;
				std::stringstream ss(input);
				input.clear();
				if (ss >> code >> sep >> input){
					if (sep != '|' || !ss.eof()){
						lpp::cout << "Argument error" << std::endl;
						lpp::cout << "error with: " << input << std::endl;
						continue;
					}
					lpp::message msg(code);
					msg << input;
					client.send(msg);
				} else {
					lpp::cout << "Argument error" << std::endl;
					continue;
				}
			}
		}
	}

	lpp::cout.clear();
    return 0;
}

