/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:16:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/25 13:08:14 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "nmap.hh"

lpp::nmap::nmap() : _async(false), _chrono("nmap sniffing"){
	_logger.setIsStdout(true);
	_logger.log(lpp::INFO, "create nmap class");
	if (lpp::system::exec("nmap --version").find("not found") != std::string::npos)
		throw std::runtime_error("nmap not installed");
}

lpp::nmap::~nmap(){}

void lpp::nmap::setAsync(bool async){ _async = async;}

void lpp::nmap::deleteOptions(){_options.clear();}

void lpp::nmap::setOptions(std::string options){
	if (!_options.empty())
		_options += " " + options;
	else
		_options = options;
}



std::vector<int> lpp::nmap::ports(const std::string &outputNmap) const{
    std::vector<int> ports;
	std::regex regexPort(R"(^([0-9]+)/(?:tcp|udp)\s+open)", std::regex::optimize);
    std::string ligne;
    std::size_t pos = 0;
    std::size_t prev = 0;
 
    while ((pos = outputNmap.find('\n', prev)) != std::string::npos) {
        ligne = outputNmap.substr(prev, pos - prev);
        prev = pos + 1;

        std::smatch match;
        if (std::regex_search(ligne, match, regexPort)) {
			ports.push_back(std::atoi(match[1].str().c_str()));
        }
    }

    return ports;
}

std::vector<std::string> lpp::nmap::sniff(std::vector<std::string> &ip){
	lpp::logger::cout(lpp::INFO, "start sniffing");
	std::vector<std::string> cmds;

	if (_async){
		lpp::logger::cout(lpp::WARNING, "multithread sniffing: This can use significante ressources");
		std::string cmd;
		for (auto &it : ip){
			cmd = "nmap " + _options + " " + it;
			_sys.add_exec(cmd);
		}

		for (auto it = _sys.get_exec(); it.has_value(); it = _sys.get_exec()){
			if (it.has_value())
				cmds.emplace_back(it.value());
		}
	}
	else{
	_logger.log(lpp::INFO, "single thread sniffing");
		for (auto &it : ip)
			cmds.emplace_back(lpp::system::exec("nmap " + _options + " " + it));
	}

	return cmds;
}

std::string lpp::nmap::sniff(const std::string ip){
	lpp::logger::cout(lpp::INFO, "start sniffing");

	std::string cmd = lpp::system::exec("nmap " + _options + " " + ip);

	lpp::logger::cout(lpp::INFO, "sniffing end");
	return cmd;
}
