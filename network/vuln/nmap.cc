/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:16:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/22 17:33:49 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "nmap.hh"

lpp::nmap::nmap() : _async(false), _chrono("nmap sniffing"){
	_logger.setIsStdout(true);
	_logger.log(lpp::INFO, "create nmap class");
}

lpp::nmap::~nmap(){}

void lpp::nmap::setAsync(bool async){ _async = async;}

void lpp::nmap::setOptions(std::string options){ _options = options;}

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
			cmds.emplace_back(lpp::system::cmd("nmap " + _options + " " + it));
	}

	return cmds;
}
