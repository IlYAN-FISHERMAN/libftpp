/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:52:00 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/20 18:42:31 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.hh"

lpp::shell::shell() : _worker(10){}

lpp::shell::~shell(){}

void lpp::shell::cmd(){
	
	std::string command(_in.pop_front());
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

	if (!pipe)
		throw std::runtime_error("popen() failed!");

	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
		result += buffer.data();

	_out.push_back(result);
}

void lpp::shell::execute(){
	cmd();
}

void lpp::shell::add_exec(const std::string &cmd){
	_in.push_back(cmd);
	_worker.addJob(lpp::system);
}

std::optional<std::string> lpp::shell::get_exec(){
	std::lock_guard<std::mutex> lock(_mutex);
	auto t = _worker.running();
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lpp::cout << "test" << std::endl;
		if (_worker.running() < t || !_out.empty()){
			if (_out.empty())
				return std::nullopt;
			return _out.pop_front();
		}

	}
	return std::nullopt;
	// _worker.wait();
	// if (_out.empty())
	// 	return std::nullopt;
	//
	// return _out.pop_front();
}
