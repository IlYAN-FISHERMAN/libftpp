/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 11:38:40 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/17 17:02:44 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hh"

int matt_daemon(int ac, char **av){
	lpp::server server;
    server.defineAction(1, [&server](long long clientID, const lpp::message& msg){
        int value;
        msg >> value;
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

	server.setDaemonLogFileName("matt_daemon.log");
	server.setDaemonLockFileName("matt_daemon.lock");

	try{
		if (ac == 2){
			if (!strcmp(av[1], "-r")){
				server.killDaemon();
				lpp::cout << "restart daemon server" << std::endl;
			}
			else if (!strcmp(av[1], "-k")){
				server.killDaemon();
				return 0;
			}
		}
		server.daemon(4242);
	}catch(std::exception &e){
		lpp::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
