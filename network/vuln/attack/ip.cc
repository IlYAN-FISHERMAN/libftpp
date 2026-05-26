/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.cc                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:43:11 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/26 21:19:03 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "ip.hh"
using namespace std::chrono_literals;

lpp::ip::ip() : _prompt(false), _is42(false), _https(false), _openPort(true),
	_nmapOutput(false), _termuxOutput(false),
	_loopTime(0s), _delay(1s), _chrono("ip"), _sys_name(lpp::system::get_system()){
}

lpp::ip::ip(const ip &other) : _prompt(other._prompt),
	_is42(other._is42), _https(other._https), _openPort(true),
	_nmapOutput(false), _termuxOutput(false),
	_loopTime(other._loopTime), _delay(other._delay), _chrono("ip"), _sys_name(lpp::system::get_system()){
}

lpp::ip& lpp::ip::operator=(const ip &other){
	if(this != &other){
		_prompt = other._prompt;
		_is42 = other._is42;
		_https = other._https;
		_loopTime = other._loopTime;
		_delay = other._delay;
	}

	return *this;
}

lpp::ip::ip(std::vector<std::string> &args) : _prompt(false), _is42(false), _https(false),
	_loopTime(0s), _delay(1s), _args(args), _chrono("ip"), _sys_name(lpp::system::get_system()){
	if (args.empty())
		throw std::runtime_error("empty argument");
	if (_args.size() > 1 && _args[1] == "--who"){
		for (size_t i = 2; i < _args.size(); i++){
			std::string user = _foundUser(_args[i], true);
			if (user == "disconnected"){
				lpp::logger::cout(lpp::WARNING, "no one connected at " + std::string(args[i]));
			} else if (user != "unknown")
				lpp::logger::cout(lpp::INFO, "login -> " + user);
		}
	}
}

std::string lpp::ip::_nextLine(){
	while (true){
		std::string input;
		lpp::cout.prompt("[lppnmap]-> ", input);
		if (input == "clear" || input == "c")
			lpp::cout.clear();

		return input;
	}
}

bool lpp::ip::_isOpenPort(int port, bool is42){
	if (port > 1024 && port < 65535){
		if (is42 && (port == 9100 || port == 2049 || port == 3306 || port == 5900))
			return false;
		return true;
	}

	return false;
}

bool lpp::ip::_isOpenPort(std::vector<int> ports, [[maybe_unused]] bool is42){

	for (auto &port : ports){
		if (port > 1024 && port < 65535){
			if (is42 && (port == 9100 || port == 2049 || port == 3306 || port == 5900))
				continue;
			return true;
		}
	}

	return false;
}

std::string lpp::ip::_foundUser(std::string &ip, bool is42){
	std::string user;

	if (is42 && isIp(ip, true)){
		user = lpp::system::exec("curl http://" + ip + ":9100/metrics 2>/dev/null | grep /home | awk -F'[\",/]\' \'{print $15}\' | head -n2");
		if (user.find('\n') != std::string::npos || user.empty()){
			if (user.empty())
				return "disconnected";

			std::string second = user.substr(user.find('\n') + 1, (user.size() - user.find('\n')) - 2);
			user.erase(user.find('\n'), user.size() - user.find('\n'));

			if (user == "credentials" || user.empty()){
				if (is42)
					user = "disconnected";
			}
			else if (second != user){
				user += "|" + second;
				lpp::logger::cout(lpp::CRITICAL, "impossible happened, two user connected at <" + ip + "> [" + user + "]");
			}
		}
	}

	if (user == "credentials" || user.empty())
		user = "unknown";
	return user;
}

lpp::ip::~ip(){}

std::optional<std::pair<std::vector<std::string>, std::vector<std::vector<int>>>>  lpp::ip::run(){

	if (_ips.empty()){
		lpp::logger::cout(lpp::WARNING, "No target specified");
		return std::nullopt;
	}

	std::pair<std::vector<std::string>, std::vector<std::vector<int>>> rtn;

	std::string open_cmd;
	lpp::logger::cout(lpp::INFO, _sys_name + " OS detected");
	if (_termuxOutput)
		open_cmd = "termux-open ";
	else if (_sys_name == "Apple")
		open_cmd = "open ";
	else if (_sys_name == "Linux")
		open_cmd = "xdg-open ";
	else{
		lpp::logger::cout(lpp::CRITICAL, "Fuck windows");
		return std::nullopt;
	}

	if (_https)
		open_cmd += "https://";
	else
		open_cmd += "http://";

	std::string tmp;
	for (auto &it : _ips)
		tmp += "[" + it.first + "]<" + it.second + ">\n";

	lpp::logger::cout(lpp::INFO, "scanning\n" + tmp);
	while(true){
		for (auto &ip : _ips){
			if (ip.first == "disconnected" || ip.first.empty()){
				lpp::logger::cout(lpp::WARNING, "no one connected at " + ip.second);
				continue;
			}

			auto sniff = _map.sniff(ip.second);
			rtn.first.emplace_back(sniff);
			if (_nmapOutput)
				lpp::cout << sniff << std::endl;

			auto ports = _map.ports(sniff);
			rtn.second.emplace_back(ports);
			if (_openPort){
				lpp::logger::cout(lpp::INFO, ip.first + " connected at " + ip.second);
				if (!_isOpenPort(ports, _is42)){
					lpp::logger::cout(lpp::WARNING, "0 target port found for user <" + ip.first + ">");
					continue;
				}
				for (auto &it : ports){
					if (_isOpenPort(it, _is42)){
						lpp::logger::cout(lpp::INFO, "port: [" + std::to_string(it) + "] found for <" + ip.second + ">");

						std::string target;
						target = open_cmd + ip.second + ":" + std::to_string(it);
						lpp::logger::cout(lpp::INFO, "executing: " + target);
						lpp::system::exec(target);
					} else{
						continue;
					}
				}
			}
		}
		
		if (_prompt){
			lpp::logger::cout(lpp::INFO, "press any key to scan again...");
			_nextLine();
		} else if (_loopTime.count() > 0){
			std::this_thread::sleep_for(1s);
			_loopTime--;
		} else
			break;
	}

	lpp::logger::cout(lpp::INFO, "end ip");
	
	return rtn;
}

bool lpp::ip::isIp(const std::string &ip, [[maybe_unused]] bool is42){
	std::regex reg(R"(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$)", std::regex::optimize);

	std::smatch match;
	if (std::regex_match(ip, match, reg)){
		if (is42 && (match[2] == "12" || (match[2] != "11" && match[2] != "13"))){
			lpp::logger::cout(lpp::CRITICAL, "only 10.11.X.X and 10.13.X.X accepted");
			return false;
		}
	}else{
		lpp::logger::cout(lpp::WARNING, "regex: <" + ip + "> is not an ip");
		return false;
	}

	return true;
}

void lpp::ip::usage(){
	std::stringstream os;
	os << "Usage:" << std::endl;
	os << "  " << "lppnmap" << " [options] [ip] [ip...]" << std::endl;
	os << std::endl;

	os << "META OPTIONS" << std::endl;
	os << "   -h, --help\tshow list of command-line options" << std::endl;
	os << std::endl;

	os << "OPTIONS" << std::endl;
	os << "            -42,\t\ttarget ips is only for 42 students" << std::endl;
	os << "            --who <ip>,\t\tgive the name of the student connected at the local ip" << std::endl;
	os << "            -a, --async\t\tenable async command, all the ips is scanned in the same time" << std::endl;
	os << "            -h, --https\tclear all logs from all instance of the matt-daemon servers" << std::endl;
	os << "            <nmap options>,\t\tyou can add all the option of nmap" << std::endl;
	os << std::endl;

	os << "ACTIONS" << std::endl;
	os << std::endl;
	os << "The matt-daemon server has predefined possible actions. By default, three of them exist, with the following format:" << std::endl;
	os << std::endl;
	os << "   code [1] server authentification system: \t\"1|username=<user> password=<server_password>\"" << std::endl;
	os << "   code [2] email sender: \t\t\t\"2|To=<example@mail.com> Body=<text_to_send>\"" << std::endl;
	os << "   code [3] remote shell of the server: \t\"3|<command>\"" << std::endl;
	os << std::endl;
	os << "When running the server, you must define at least the SERVER_PASSWORD variable in a .env file located in the build folder.\nSee the env.example file for more details." << std::endl;
	os << std::endl;

	std::cout << os.str();
}

void lpp::ip::addIp(std::string &ip){_ips.insert({"unknown", ip});}

void lpp::ip::addOption(std::string &opt){_map.addOptions(opt);}

void lpp::ip::clearIps(){_ips.clear();}

void lpp::ip::setNmapOutput(bool nmap){_nmapOutput = nmap;}

void lpp::ip::setPortOutput(bool port){_openPort = port;}

void lpp::ip::clearIps(std::string &name){_ips.erase(name);}

void lpp::ip::clearOptions(){_map.clearOptions();}

void lpp::ip::clear(){
	_ips.clear();
	_prompt = false;
	_loopTime = 0s;
	_delay = 1s;
	_https = false;
	_args.clear();
}

void lpp::ip::setIterationTime(std::chrono::seconds s, std::chrono::seconds delay){
	if (_prompt)
		throw std::logic_error("can't define prompt and iteration time");
	_loopTime = s;
	_delay = delay;
}

void lpp::ip::setIsPrompt(bool prompt){_prompt = prompt;}

void lpp::ip::setIs42(bool is42){ _is42 = is42;}

int lpp::ip::parse(std::vector<std::string> &args){
	if (args.empty())
		throw std::runtime_error("empty argument");

	if (args.size() > 1 && args[0] == "--who"){
		args.erase(args.begin());
		who(args);
		return 1;
	}

	for (auto it = args.begin(); it < args.end(); it++){
		if (*it == "-42") [[unlikely]] {
			lpp::logger::cout(lpp::INFO, "target 42 users");
			_is42 = true;
		}
		else if (*it == "--async" || *it == "-a") [[unlikely]] {
			lpp::logger::cout(lpp::INFO, "async enable");
			_map.setAsync(true);
		}
		else if (*it == "--https" || *it == "-h"){
			lpp::logger::cout(lpp::INFO, "target https");
			_https = true;
		}
		else if (*it == "-p"){
			if (_loopTime.count() == 0){
				lpp::logger::cout(lpp::ERROR, "cant't specifie -p and -t");
				return 1;
			}
			_prompt = true;
		}
		else if (*it == "-t" || *it == "--time"){
			if (_prompt){
				lpp::logger::cout(lpp::ERROR, "cant't specifie -p and -t");
			}
			it++;
			if (it == args.end()){
				lpp::logger::cout(lpp::ERROR, "time option empty value");
				return 1;
			}
			std::stringstream os(*it);
			int nbr = 0;
			os >> nbr;
			if (!os.eof() || nbr == 0){
				lpp::logger::cout(lpp::ERROR, "time option bad value");
				return 1;
			}
			_loopTime = std::chrono::seconds(nbr);
			lpp::logger::cout(lpp::INFO, "scan running " + std::to_string(_loopTime.count()) + " times");
		} else if (*it == "-m"){
			_nmapOutput = true;
			_openPort = false;
		} else if (*it == "--port"){
			_openPort = true;
		} else if (*it == "--termux"){
			_termuxOutput = true;
		}
		else if (isIp(*it)) [[likely]]
			_ips.insert({_foundUser(*it, _is42), *it});
		else
			_map.addOptions(*it);
	}

	return 0;
}

std::vector<std::string> lpp::ip::who(std::vector<std::string> &args){
	std::vector<std::string> rtn;

	if (args.empty())
		throw std::runtime_error("empty argument");
	for (auto it : args){
		std::string user = _foundUser(it, true);
		rtn.emplace_back(user);
		if (user == "disconnected"){
			lpp::logger::cout(lpp::WARNING, "no one connected at " + it);
		} else if (user != "unknown")
			lpp::logger::cout(lpp::INFO, "login -> " + user);
	}

	return rtn;
}
