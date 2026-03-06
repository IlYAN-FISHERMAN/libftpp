/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:19:24 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/06 17:02:38 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testServer() {
    Server server;
    Client client;

    // Define an action for messages of type 1 (int)
    server.defineAction(1, [&server](long long clientID, const Message& msg){
        int value;
        msg >> value;
        threadSafeCout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 1 with double the value
        Message replyMsg(1);
        replyMsg << (value * 2);
        server.sendTo(replyMsg, clientID);
    });

    server.defineAction(10, [&server](long long clientID, const Message& msg){
		std::string value;
        Message replyMsg(10);

        msg >> value;
		if (value != "ping")
			replyMsg << "ping?";
		else{
			replyMsg << "pong";
		}

        server.sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    server.defineAction(2, [&server](long long clientID, const Message& msg){
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
		Message tmp(2);
		tmp << "message received";
		server.sendTo(tmp, clientID);
    });

    // Start the server on port 8080
	try{
		server.start(8080);
	} catch(std::runtime_error &e){
		threadSafeCout << "server: " << e.what() << std::endl;
		exit(-1);
	}

	try{
		client.connect("127.0.0.1", 8080);
	} catch(std::runtime_error &e){
		threadSafeCout << "client: " << e.what() << std::endl;
		return(-1);
	}

	client.defineAction(1, [](const Message& msg){
        int doubledValue;
		msg >> doubledValue;
        threadSafeCout << "Received a doubled value: " << doubledValue << std::endl;
    });

    client.defineAction(2, [](const Message& msg){
		threadSafeCout << "server say \"" << msg.str() << "\"" << std::endl;
    });

	client.defineAction(4, [](const Message& msg){
		std::string str;
        msg >> str;
        threadSafeCout << "client received : " << str << std::endl;
    });

	client.defineAction(10, [](const Message& msg){
		std::string str;
        msg >> str;
		threadSafeCout << str << std::endl;
		if (str == "pong")
			threadSafeCout << "server work!" << std::endl;
    });

	threadSafeCout << "Available operations :" << std::endl;
	threadSafeCout << " - [Q]uit|exit : close the program" << std::endl;
	threadSafeCout << " - Any other input to continue updating the client" << std::endl;
	threadSafeCout << "Usage - (code)|(input), (string)" << std::endl << std::endl;
	threadSafeCout.setPrefix("[Client]: ");
	while (true)
	{
		std::string input;
		threadSafeCout.prompt("libftpp-> ", input);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input == "quit" || input == "q" || input == "exit")
			break;
		else if(input == "clear" || input == "c"){
			threadSafeCout << "\033c" << std::flush;
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
						threadSafeCout << "Argument error" << std::endl;
						threadSafeCout << "error with: " << input << std::endl;
						continue;
					}
					Message msg(code);
					msg << input;
					client.send(msg);
				} else {
					threadSafeCout << "Argument error" << std::endl;
					continue;
				}
			}
		}
	}

    return 0;
}

