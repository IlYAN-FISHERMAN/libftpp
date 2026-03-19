/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 11:38:40 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/19 12:03:13 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hh"

void printUsage(){
	std::cout << "Usage:" << std::endl;
	std::cout << "  " << "matt-daemon" << " [options]" << std::endl << std::endl;

	std::cout << "META OPTIONS" << std::endl;
	std::cout << "   -h, --help\tshow list of command-line options" << std::endl;
	std::cout << std::endl;

	std::cout << "OPTIONS" << std::endl;
	std::cout << "            -r,\t\trestart matt-daemon server" << std::endl;
	std::cout << "            -k,\t\tkill current matt-daemon server" << std::endl;
	std::cout << "   --clear-log,\tclear all logs from all instance of the matt-daemon servers" << std::endl;
	std::cout << std::endl;
}

int matt_daemon(int ac, char **av){
	lpp::server server;

	server.setDaemonLogFileName("matt_daemon.log");
	server.setDaemonLockFileName("matt_daemon.lock");
	server.setDaemonExecFileName("matt_daemon_exec.log");

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

    server.defineAction(3, [&server](long long clientID, const lpp::message& msg){
		
		server.getLogger().log(lpp::INFO, "received command: " + msg.str());
		lpp::message rtn(3);
		rtn << server.exec(msg.str());
		server.sendTo(rtn, clientID);
    });

	try{
		for (auto it = 1; it < ac; it++){
			if (!strcmp(av[it], "--help") || !strcmp(av[it], "-h")){
				printUsage();
				return 0;
			}
			else if (!strcmp(av[it], "-r")){
				server.killDaemon();
				server.daemon(4242);
				lpp::cout << "restart daemon server" << std::endl;
			}
			else if (!strcmp(av[it], "-k")){
				if (ac > 2){
					std::cerr << "too much options with -k" << std::endl;
				}
				server.killDaemon();
				return 0;
			}
			else if (!strcmp(av[it], "--clear-log")){
				std::filesystem::directory_entry dir("/var/log/libftpp/server");
				// dir.
			}
			else{
				std::cout << "Error: matt-daemon: option " << av[it] << " not found" << std::endl <<  std::endl;
				printUsage();
				return -1;
			}
		}
	}catch(std::exception &e){
		lpp::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
