/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_shell.cc                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:52:00 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/24 22:31:00 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "system_shell.hh"

lpp::system::shell::shell() : _worker(10){}

lpp::system::shell::~shell(){
	_worker.wait();
}

lpp::system::system(){
	_shell = std::make_unique<lpp::system::shell>();
}

lpp::system::~system(){
	_shell->_worker.wait();
}

std::string lpp::system::exec(const std::string &command){
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

	if (!pipe)
		throw std::runtime_error("popen() failed!");

	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
		result += buffer.data();

	return result;
}

void lpp::system::shell::cmd(){
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

void lpp::system::shell::execute(){
	cmd();
}

void lpp::system::add_exec(const std::string &cmd) const{
	_shell->_in.push_back(cmd);
	_shell->_worker.addJob(_shell);
}

std::optional<std::string> lpp::system::get_exec() const{
	_shell->_worker.wait();
	if (_shell->_out.empty())
		return std::nullopt;

	return _shell->_out.pop_front();
}
