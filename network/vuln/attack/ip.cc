/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.cc                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:43:11 by ilyanar           #+#    #+#             */
/*   Updated: 2026/06/08 11:50:38 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "ip.hh"
using namespace std::chrono_literals;

void lpp::ip::usage(){
    std::cout <<
	R"(Usage: lppnmap [options] <targets>
Options:
  -42              Enable target 42 student
  -a, --async      Enable async mode
  -h, --https      Target HTTPS services
  -p               Prompt mode (cannot be used with -t)
  -t, --time N     Run scan N times
  -d, --delay N    Delay between scans (seconds)
  --file PATH      Log output to file
  --who ARG        Display student-login at the ip
  -m               Output Nmap format
  --port           Show open ports only (can be used with -m)
  -ip              Add your own ip to the ips target
  --termux         Enable this if you use termux
  --help, -h       Show this help message

Targets:
  IP addresses or domains

EXAMPLES:
	lppnmap -42 c2r5s1 (need -42 if you target student domaine)
	lppnmap --https --async -t 10 -d 5 --file logins.txt 10.11.10.2 (target the 10.11.10.2 ip, only https, with async scan.
	                                                                 Scan 10 time with 5 second delay between them and print the output log at logins.txt)
)" << std::endl;
}

lpp::ip::ip() : _prompt(false), _is42(false), _https(false), _openPort(true),
	_nmapOutput(false), _termuxOutput(false),
	_loopTime(1), _delay(1s), _chrono("ip"), _sys_name(lpp::system::get_system()){
	_logger.setIsStdout(true);
}

lpp::ip::ip(const ip &other) : _prompt(other._prompt),
	_is42(other._is42), _https(other._https), _openPort(true),
	_nmapOutput(false), _termuxOutput(false),
	_loopTime(other._loopTime), _delay(other._delay), _chrono("ip"), _sys_name(lpp::system::get_system()){
	_logger.setIsStdout(false);
}

lpp::ip& lpp::ip::operator=(const ip &other){
	if(this != &other){
		_prompt = other._prompt;
		_is42 = other._is42;
		_https = other._https;
		_loopTime = other._loopTime;
		_delay = other._delay;
		_logger = other._logger;
	}

	return *this;
}

bool lpp::ip::setLogFile(const std::string &path) noexcept{
	_logger.setIsStdout(false);
	_logger.setFilePath(path);
	_logger.open();
	return _logger.is_open();
}

lpp::ip::ip(std::vector<std::string> &args) : _prompt(false), _is42(false), _https(false),
	_loopTime(1), _delay(1s), _chrono("ip"), _sys_name(lpp::system::get_system()){
	if (args.empty())
		throw std::runtime_error("empty argument");
	if (args.size() > 1 && args[1] == "--who"){
		for (size_t i = 2; i < args.size(); i++){
			std::string user = _foundUser(args[i], true);
			if (user == "disconnected"){
				_logger.log(lpp::WARNING, "no one connected at " + std::string(args[i]));
			} else if (user != "unknown")
				_logger.log(lpp::INFO, "login -> " + user);
		}
	}

	_logger.setFilePath("test.txt");
	_logger.open();
	if (!_logger.is_open())
		throw std::runtime_error("logger open fail");
}

std::string lpp::ip::_nextLine() const noexcept{
	while (true){
		std::string input;
		lpp::cout.prompt("[lppnmap]-> ", input);
		if (input == "clear" || input == "c")
			lpp::cout.clear();

		return input;
	}
}

bool lpp::ip::_isOpenPort(int port, bool is42) const noexcept{
	if ((port > 1024 && port < 65535) || (port == 80 || port == 443)){
		if (is42 && (port == 9100 || port == 2049 || port == 3306 || port == 5900))
			return false;
		return true;
	}

	return false;
}

bool lpp::ip::_isOpenPort(std::vector<int> ports, [[maybe_unused]] bool is42) const noexcept{

	for (auto &port : ports){
		if ((port > 1024 && port < 65535) || (port == 80 || port == 443)){
			if (is42 && (port == 9100 || port == 2049 || port == 3306 || port == 5900))
				continue;
			return true;
		}
	}

	return false;
}

std::string lpp::ip::_foundUser(std::string &ip, bool is42) const noexcept{
	std::string user;

	if (is42 && (isIp(ip, true) || isDomaine(ip, true))){
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
				_logger.log(lpp::CRITICAL, "impossible happened, two user connected at <" + ip + "> [" + user + "]");
			}
		}
	}

	if (user == "credentials" || user.empty())
		user = "unknown";
	return user;
}

lpp::ip::~ip(){}

std::optional<std::pair<std::vector<std::string>, std::vector<std::vector<int>>>>  lpp::ip::run() noexcept(false){

	if (_ips.empty()){
		_logger.log(lpp::WARNING, "No target specified");
		return std::nullopt;
	}

	std::pair<std::vector<std::string>, std::vector<std::vector<int>>> rtn;

	static constinit std::string open_cmd;
	_logger.log(lpp::INFO, _sys_name + " OS detected");
	if (open_cmd.empty()){
		if (_termuxOutput)
			open_cmd = "termux-open ";
		else if (_sys_name == "Apple")
			open_cmd = "open ";
		else if (_sys_name == "Linux")
			open_cmd = "xdg-open ";
		else{
			_logger.log(lpp::CRITICAL, "Fuck windows");
			return std::nullopt;
		}
		open_cmd += (_https ? "https://" : "http://");
	}


	std::string tmp;
	for (auto &it : _ips)
		tmp += " [" + it.first + "]<" + it.second + "> ";

	_logger.log(lpp::INFO, "scanning " + tmp);
	for (auto &ip : _ips){
		for(auto nb = _loopTime; nb > 0; nb--){
			if (ip.first == "disconnected" || ip.first.empty()){
				_logger.log(lpp::WARNING, "no one connected at " + ip.second);
				continue;
			}

			auto sniff = _map.sniff(ip.second);
			rtn.first.emplace_back(sniff);
			if (_nmapOutput)
				_logger.log(INFO, sniff);

			auto ports = _map.ports(sniff);
			rtn.second.emplace_back(ports);
			if (_openPort){
				_logger.log(lpp::INFO, ip.first + " connected at " + ip.second);
				if (!_isOpenPort(ports, _is42))
					_logger.log(lpp::WARNING, "0 target port found for user [" + ip.first + "]<" + ip.second + ">");
				else{
					for (auto &it : ports){
						if (_isOpenPort(it, _is42)){
							_logger.log(lpp::INFO, "port: [" + std::to_string(it) + "] found for <" + ip.second + ">");

							std::string target;
							target = open_cmd + ip.second + ":" + std::to_string(it);
							_logger.log(lpp::INFO, "executing: " + target);
							lpp::system::exec(target);
						}
					}
				}
			}
			if (_prompt){
				_logger.log(lpp::INFO, "press any key to scan again...");
				[[maybe_unused]] auto reply = _nextLine();
			}
			if (nb > 1 && _delay.count() > 0)
				std::this_thread::sleep_for(_delay);
		}
	}
	

	_logger.log(lpp::INFO, "end ip");
	return rtn;
}

bool lpp::ip::isDomaine(const std::string &d, bool is42) noexcept{
	static const std::regex pattern(R"(^(?:[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?\.)+[a-zA-Z]{2,}\.?$)");
	static const std::regex users(R"(^c\d+r\d+s\d+$)");

	if (is42 && std::regex_match(d, users))
		return true;
    if (std::regex_match(d, pattern))
		return true;

	return false;
}


bool lpp::ip::isIp(const std::string &ip, bool is42) noexcept{
	static const std::regex reg(
		R"(^(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.)"
		R"((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.)"
		R"((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.)"
		R"((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)$)"
	);

	static const std::regex cidrRegex(
		R"(^(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.)"
		R"((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.)"
		R"((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.)"
		R"((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)"
		R"(\/([0-9]|[12]\d|3[0-2]))$)"
	);

	std::smatch match;
	if (!std::regex_match(ip, match, reg) && !std::regex_match(ip, match, cidrRegex))
		return false;
	if (is42 && (match[2] == "12" || (match[2] != "11" && match[2] != "13")))
		return false;

	return true;
}

void lpp::ip::addIp(const std::string &ip) noexcept{
	if (isIp(ip) || isDomaine(ip))
		_ips.insert({"unknown", ip});
}

void lpp::ip::addOption(const std::string &opt) noexcept {_map.addOptions(opt);}

void lpp::ip::clearIps(){_ips.clear();}

void lpp::ip::setNmapOutput(const bool nmap) noexcept {_nmapOutput = nmap;}

void lpp::ip::setPortOutput(const bool port) noexcept {_openPort = port;}

void lpp::ip::clearIps(const std::string &name) noexcept {_ips.erase(name);}

void lpp::ip::clearOptions() noexcept {_map.clearOptions();}

void lpp::ip::clear() noexcept {
	_ips.clear();
	_prompt = false;
	_loopTime = 1;
	_delay = 1s;
	_https = false;
}

void lpp::ip::setIterationTime(const int s, const std::chrono::seconds delay) noexcept {
	_loopTime = s;
	_delay = delay;
}

void lpp::ip::setIsPrompt(const bool prompt) noexcept {_prompt = prompt;}

void lpp::ip::setIs42(const bool is42) noexcept { _is42 = is42;}

std::string lpp::ip::get(const std::string interface){

	std::string ip;
	if (_sys_name == "Apple")
		ip = lpp::system::exec("ipconfig getifaddr " + interface);
	else if (_sys_name == "Linux")
		ip = lpp::system::exec(R"(hostname -i | awk '{print $2}')");

	std::cout << "\"" << ip << "\"" << std::endl;
	return ip;
}

int lpp::ip::parse(std::vector<std::string> &args) noexcept(false){
	for (auto it = args.begin(); it < args.end(); it++){
		if (*it == "-42") [[unlikely]] {
			_logger.log(lpp::INFO, "target 42 users");
			_is42 = true;
		} else if (*it == "--async" || *it == "-a") [[unlikely]] {
			_logger.log(lpp::INFO, "async enable");
			_map.setAsync(true);
		} else if (*it == "--https" || *it == "-h"){
			_logger.log(lpp::INFO, "target https");
			_https = true;
		} else if (*it == "-p"){
			if (_loopTime == 0){
				_logger.log(lpp::ERROR, "cant't specifie -p and -t");
				return 1;
			}
			_prompt = true;
		}else if (*it == "--file"){
			it++;
			if (it == args.end()){
				_logger.log(lpp::ERROR, "log file option empty value");
				return 1;
			}
			_logger.close();
			_logger.setFilePath(*it);
			_logger.open();
			if (!_logger.is_open())
				throw std::runtime_error("logger file open fail");

		}else if (*it == "--who"){
			it++;
			if (it == args.end())
				throw std::runtime_error("empty argument");
			std::vector<std::string> tmp(it, args.end());
			who(tmp);
			return 1;
		} else if (*it == "-t" || *it == "--time"){
			if (_prompt){
				_logger.log(lpp::ERROR, "cant't specifie -p and -t");
			}
			it++;
			if (it == args.end()){
				_logger.log(lpp::ERROR, "time option empty value");
				return 1;
			}
			std::stringstream os(*it);
			int nbr = 0;
			os >> nbr;
			if (!os.eof() || nbr == 0){
				_logger.log(lpp::ERROR, "time option bad value");
				return 1;
			}
			_loopTime = nbr;
			_logger.log(lpp::INFO, "scan running " + std::to_string(_loopTime) + " times");
		} else if (*it == "-d" || *it == "--delay"){
			it++;
			if (it == args.end()){
				_logger.log(lpp::ERROR, "delay option empty value");
				return 1;
			}
			std::stringstream os(*it);
			int nbr = 0;
			os >> nbr;
			if (!os.eof() || nbr == 0){
				_logger.log(lpp::ERROR, "delay option bad value");
				return 1;
			}
			_delay = std::chrono::seconds(nbr);
			_logger.log(lpp::INFO, "delay set to " + std::to_string(_delay.count()) + "s");
		} else if (*it == "-m"){
			_nmapOutput = true;
			_openPort = false;
		} else if (*it == "--port"){
			_openPort = true;
		} else if (*it == "--termux"){
			_termuxOutput = true;
		} else if (*it == "-ip"){
			addIp(ip::get());
		}
		else if (isIp(*it, _is42) || isDomaine(*it, _is42)) [[likely]]{
			_logger.log(INFO, "found ip/domaine at " + *it);
			_ips.insert({_foundUser(*it, _is42), *it});
		}
		else{
			_logger.log(INFO, "add option to nmap: " + *it);
			_map.addOptions(*it);
		}
	}

	return 0;
}

std::vector<std::string> lpp::ip::who(const std::vector<std::string> &args) const noexcept(false){
	std::vector<std::string> rtn;

	if (args.empty())
		throw std::runtime_error("empty argument");
	for (auto it = _loopTime; it > 0; it--){
		for (auto it : args){
			std::string user = _foundUser(it, true);
			rtn.emplace_back(user);
			if (user == "disconnected"){
				_logger.log(lpp::WARNING, "no one connected at " + it);
			} else if (user != "unknown")
				_logger.log(lpp::INFO, "[" + it + "]login -> " + user);
		}
		if (it > 1)
			std::this_thread::sleep_for(_delay);
	}

	return rtn;
}
