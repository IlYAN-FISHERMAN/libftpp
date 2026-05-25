/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   website_tracking.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:14:54 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/25 14:11:36 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hh"
#include <map>

std::string foundUser(std::string ip){
	std::regex reg(R"(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$)", std::regex::optimize);

	std::smatch match;
	if (std::regex_match(ip, match, reg)){
		if (match[2] == "12" || (match[2] != "11" && match[2] != "13")){
			lpp::logger::cout(lpp::CRITICAL, "only 10.11.X.X and 10.13.X.X accepted");
			throw std::runtime_error("found user ip error");
		}
	}else{
		lpp::logger::cout(lpp::WARNING, "bad ip address format");
		throw std::runtime_error("bad ip regex");
	}

	std::string user = lpp::system::exec("curl http://" + ip + ":9100/metrics 2>/dev/null | grep /home | awk -F'[\",/]\' \'{print $15}\' | head -n1");

	if (user.find('\n') != std::string::npos)
		user.erase(user.find('\n'));

	if (user == "credentials" || user.empty())
		user = "unknown";
	return user;
}


bool isOpenPort(int port, bool is42 = false){
	if (port > 1024 && port < 65535){
		if (is42 && (port == 9100 || port == 2049 || port == 3306 || port == 5900))
			return false;
		return true;
	}

	return false;
}

bool isOpenPort(std::vector<int> ports, bool is42 = false){

	for (auto &port : ports){
		if (port > 1024 && port < 65535){
			if (is42 && (port == 9100 || port == 2049 || port == 3306 || port == 5900))
				continue;
			return true;
		}
	}

	return false;
}

int runNmap(int ac, char **av){

	if (ac == 3 && !strcmp(av[1], "--who")){
		std::string user = foundUser(av[2]);
		if (user == "unknown"){
			lpp::logger::cout(lpp::WARNING, "no one connected at " + std::string(av[2]));
		} else
			lpp::logger::cout(lpp::INFO, "login -> " + user);
		return 0;
	}

	lpp::nmap map;
	bool is42 = false;
	bool https = false;

	std::vector<std::string> args;
	std::map<std::string, std::string> ips;
	std::vector<std::string> tmp_ip;


	for (int i = 1; i < ac; i++)
		args.emplace_back(av[i]);

	for (auto &it : args){
		if (it == "-42"){
			lpp::logger::cout(lpp::INFO, "target 42 users");
			is42 = true;
		}
		else if (it == "--async" || it == "-a"){
			lpp::logger::cout(lpp::INFO, "async enable");
			map.setAsync(true);
		}
		else if (it == "--https" || it == "-s"){
			lpp::logger::cout(lpp::INFO, "target https");
			https = true;
		}
		else if (it == "-A" || it == "-F")
			map.setOptions(it);
		else{
			tmp_ip.emplace_back(it);
		}
	}

	for (auto &it : tmp_ip){
		if (is42)
			ips.insert({foundUser(it), it});
		else
			ips.insert({"unknown", it});
	}

	if (ips.empty()){
		lpp::logger::cout(lpp::WARNING, "No target specified");
		return 0;
	}
	std::string tmp;
	for (auto &it : ips)
		tmp += " [" + it.first + "]<" + it.second + ">";

	lpp::logger::cout(lpp::INFO, "scanning" + tmp);

	std::string sys_name(lpp::system::get_system());
	std::string open_cmd;

	lpp::logger::cout(lpp::INFO, sys_name + " OS detected");
	if (sys_name == "Apple")
		open_cmd = "open ";
	else if (sys_name == "Linux")
		open_cmd = "xdg-open ";
	else{
		lpp::logger::cout(lpp::CRITICAL, "Fuck windows");
		return -1;
	}

	if (https)
		open_cmd += "https://";
	else
		open_cmd += "http://";

	lpp::cout << std::endl;
	while(true){
		for (auto &ip : ips){
			auto ports = map.ports(map.sniff(ip.second));


			if (ip.first == "credentials" || ip.first.empty()){
				lpp::logger::cout(lpp::WARNING, "no one connected at " + ip.second);
				lpp::cout << std::endl;
				continue;
			}
			lpp::logger::cout(lpp::INFO, ip.first + " connected at " + ip.second);

			if (!isOpenPort(ports, is42)){
				lpp::logger::cout(lpp::WARNING, "0 target port found for " + ip.first);
				lpp::cout << std::endl;
				continue;
			}
			for (auto &it : ports){
				if (isOpenPort(it, is42)){
					lpp::logger::cout(lpp::INFO, "port: [" + std::to_string(it) + "] found for <" + ip.second + ">");

					std::string target;
					target = open_cmd + ip.second + ":" + std::to_string(it);
					lpp::logger::cout(lpp::INFO, "executing: " + target);
					lpp::system::exec(target);
				} else{
					continue;
				}
			}
			lpp::cout << std::endl;
		}
		lpp::logger::cout(lpp::INFO, "press any key to scan again...");
		std::string input;
		std::getline(std::cin, input);
	}
	lpp::logger::cout(lpp::CRITICAL, "[END]");
}

int main(int ac, char **av){

	try {
		if (ac == 1){
			lpp::cout << "Usage:\n./libftpp-nmap <options> <ips>" << std::endl;
			return 0;
		} else
			return runNmap(ac, av);
	} catch(std::runtime_error &e){
		lpp::logger::cout(lpp::CRITICAL, e.what());
	}
	return 0;
}
