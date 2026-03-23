/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 11:38:40 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/23 21:04:54 by ilyanar          ###   LAUSANNE.ch       */
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
	std::ifstream passwd(".env", std::ios::out);

	if (!passwd.is_open()){
		lpp::cout << "Need .env file with passwd" << std::endl;
		return -1;
	}
	std::string tmp;
	passwd >> tmp;
	server.setPasswd(tmp);
	server.setDaemonLogFile("matt_daemon.log");
	server.setDaemonLockFile("matt_daemon.lock");

	lpp::cout.setPrefix("[matt_daemon]: ");
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

	try{
		if (ac > 1){
			for (auto it = 1; it < ac; it++){
				if (!strcmp(av[it], "--help") || !strcmp(av[it], "-h")){
					printUsage();
					return 0;
				}
				else if (!strcmp(av[it], "-r")){
					server.killDaemon();
					lpp::cout << "restart daemon server" << std::endl;
					server.daemon(4242);
				}
				else if (!strcmp(av[it], "-k")){
					server.killDaemon();
				}
				else if (!strcmp(av[it], "--clear-log")){
					std::string path("/var/log/libftpp/server/lpp_daemon/");
					if (std::filesystem::exists(path) && std::filesystem::is_directory(path)){
						size_t fileNbr = 0;
						for (auto &it : std::filesystem::directory_iterator(path))
							std::filesystem::remove_all(it.path()) && fileNbr++;
						lpp::cout << fileNbr << " log files removed from " << path << std::endl;
					}
				}
				else{
					lpp::cout << "Error: matt-daemon: option " << av[it] << " not found\n" << std::endl;
					printUsage();
					return -1;
				}
			}
		}
		else
			server.daemon(4242);
	}catch(std::exception &e){
		lpp::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
