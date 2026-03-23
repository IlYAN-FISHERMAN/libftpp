/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:10 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/23 14:41:27 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hh"

static sig_atomic_t sigCode = 0;

static void handleSigint(int){
	sigCode = 1;
}

void lpp::server::_workerLoop(){
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_logger.setFilePath(_logPath + logger::getDate() + '_' + _logFile);
		_logger.setPrintFormat(true);
		_logger.open();
		try{
			if (!_logger.is_open()){
				throw std::runtime_error("Error opening log file.");
			}
			cout.setPrefix("[server] =: ");
			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (_socket < 0)
				throw std::runtime_error("socket failed");
			cout << "started..." << std::endl;
			int opt = 1;
			setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
			_serv_addr.sin_family = AF_INET;
			_serv_addr.sin_addr.s_addr = INADDR_ANY;
			_serv_addr.sin_port = htons(_p_port);
			if (bind(_socket, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0)
				throw std::runtime_error("bind failed");
			else if (::listen(_socket, 5) < 0)
				throw std::runtime_error("listen failed");
			_pollFd.push_back(pollfd{_socket, POLLIN, 0});
			_msg.push_back("");
			cout << "listening..." << std::endl;
			_running.store(true);
		} catch (...){
			_exc = std::current_exception();
			return;
		}
	}
	while (_running.load()){
		if (poll(_pollFd.data(), _pollFd.size(), 100) < 0){
			break;
		}
		if (!_running.load()){
			cout << "stopping server..." << std::endl;
			break ;
		}

		for(size_t i = 0; i < _pollFd.size(); ++i){
			if (_pollFd[i].revents & POLLIN){
				if (_pollFd[i].fd == _socket){
					int acpt = accept(_socket, 0x00, 0x00);
					if (acpt >= 0){
						_pollFd.push_back(pollfd{acpt, POLLIN, 0});
						_msg.push_back("");
						cout << "new client connected" << std::endl;
					}
					continue;
				}
				char buffer[BUFFER_SIZE]{0};
				ssize_t n = read(_pollFd[i].fd, buffer, sizeof(buffer));
				cout << "read " << n << " bytes" << std::endl;
				if (n <= 0){
					cout << "close client" << std::endl;
					close(_pollFd[i].fd);
					_pollFd.erase(_pollFd.begin() + i);
					_msg.erase(_msg.begin() + i);
					i--;
				} else{
					_msg[i].append(buffer, n);
					std::stringstream ss(_msg[i]);
					int code = 0;
					char sep = 0;
					if (ss >> code >> sep){
						message msg(code);
						if (_actions.find(code) != _actions.end() && sep == '|'){
							std::string tmp;
							std::getline(ss >> std::ws, tmp);
							msg << tmp;
							_actions[code](_pollFd[i].fd, msg);
						}
						else{
							lpp::cout << "send error: " << ::send(_pollFd[i].fd, "action not found", 16, 0) << std::endl;
						}
					}
					else{
						::send(_pollFd[i].fd, _msg[i].c_str(), _msg[i].size(), 0);
					}
					_msg[i].clear();
				}
			}
		}
	}
	return ;
}

bool lpp::server::_tryConnection(int clientId){

	return _authorized[clientId]._isConnected;
}

void lpp::server::_executeMessage(std::stringstream &ss, size_t &id){
	int code = 0;
	char sep = 0;
	if (ss >> code >> sep){
		message msg(code);
		if (_actions.find(code) != _actions.end() && sep == '|'){
			std::string tmp;
			std::getline(ss >> std::ws, tmp);
			_logger.log(INFO, "received string: " + tmp);
			msg << tmp;
			_actions[code](_pollFd[id].fd, msg);
		}
		else{
			_logger.log(ERROR, "client[" + std::to_string(_pollFd[id].fd) + "] tried to launch a non-existent action");
			::send(_pollFd[id].fd, "action not found\n", 16, 0);
		}
	} else{
			_logger.log(INFO, "client[" + std::to_string(_pollFd[id].fd) + "] input: \"" + _msg[id] + "\"");
			if (_msg[id] == "\n" || _msg[id].empty())
				::send(_pollFd[id].fd, "", 0, 0);
			else
				::send(_pollFd[id].fd, "message received\n", 18, 0);
	}
}

void lpp::server::_connectUser(int clientId){
	_authorized[clientId] = authentification();
	_authorized[clientId]._isConnected = false;
	sockaddr_in addr{};
	socklen_t len = sizeof(addr);

	getpeername(clientId, (sockaddr*)&addr, &len);
	char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

	_logger.log(DEBUG, "ip: \"" + std::string(ip) + "\"");
	if (std::string(ip) == "127.0.0.1")
		_authorized[clientId]._isConnected = true;
}

void lpp::server::_daemonLoop(){
	_logger.setFilePath(_daemonLogPath + logger::getDate() + '_' + _daemonLogFile);
	_logger.setPrintFormat(true);
	_logger.open();
	if (!_logger.is_open()){
		lpp::logger info;
		info.setIsStdout(true);
		info.log(ERROR, "Error opening log file.");
		return ;
	}
	_logger.log(INFO, "begin daemon loop");
	_chrono.start();
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket < 0)
		throw std::runtime_error("socket failed");
	_logger.log(INFO, "start server configuration");

	int opt = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_addr.s_addr = INADDR_ANY;
	_serv_addr.sin_port = htons(_p_port);

	if (bind(_socket, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0)
		throw std::runtime_error("daemon bind failed");
	else if (::listen(_socket, 5) < 0)
		throw std::runtime_error("daemon listen failed");

	_pollFd.push_back(pollfd{_socket, POLLIN, 0});
	_msg.push_back("");
	_logger.log(INFO, "listening...");
	_running.store(true);

	std::signal(SIGPIPE, SIG_IGN);
	std::signal(SIGINT, handleSigint);

	while (!sigCode){
		if (poll(_pollFd.data(), _pollFd.size(), 0) < 0){
			break;
		}
		if (sigCode)
			break;
		for(size_t i = 0; i < _pollFd.size(); ++i){
			if (_pollFd[i].revents & POLLIN){
				if (_pollFd[i].fd == _socket){
					int acpt = accept(_socket, 0x00, 0x00);
					if (acpt >= 0){
						_pollFd.push_back(pollfd{acpt, POLLIN, 0});
						_msg.push_back("");
						_logger.log(INFO, "new client[" + std::to_string(acpt) + "] connected");
						_connectUser(acpt);
					}
					continue;
				}
				char buffer[BUFFER_SIZE]{0};
				ssize_t n = read(_pollFd[i].fd, buffer, sizeof(buffer));
				_logger.log(INFO, "server read " + std::to_string(n) + " bytes");
				if (n <= 0){
					_logger.log(INFO, "close client[" + std::to_string(_pollFd[i].fd) + "]");
					close(_pollFd[i].fd);
					_authorized.erase(_pollFd[i].fd);
					_pollFd.erase(_pollFd.begin() + i);
					_msg.erase(_msg.begin() + i);
					i--;
				} else{
					_msg[i].append(buffer, n);
					std::stringstream ss(_msg[i]);
					_logger.log(INFO, "try connect client[" + std::to_string(_pollFd[i].fd) + "]: " + std::to_string(_tryConnection(_pollFd[i].fd)));
					if (_msg[i] == "quit" || _msg[i] == "quit\n"){
						_logger.log(INFO, "client[" + std::to_string(_pollFd[i].fd) + "] request quit");
						close(_pollFd[i].fd);
						_pollFd.erase(_pollFd.begin() + i);
						_msg.erase(_msg.begin() + i);
						i--;
					} else if (_tryConnection(_pollFd[i].fd)){
						_executeMessage(ss, i);
					} else{
						::send(_pollFd[i].fd, "Connection refused\n", 20, 0);
					}
					_msg[i].clear();
				}
			}
		}
	}
	_logger.log(INFO, "stopping server... code[" + std::to_string(sigCode) + "]");
	_logger.log(INFO, "lpp::server running " + std::to_string(_chrono.stop().count()) + " seconds");

	for (auto pol : _pollFd)
		close(pol.fd);
	_logger.log(INFO, "clear poll structure");

	if (_lockFd != -1){
		flock(_lockFd, LOCK_UN);
		close(_lockFd);
		std::filesystem::remove((_daemonLockPath + _daemonLockFile).c_str());
		_logger.log(INFO, "filesystem remove " + _daemonLockPath + _daemonLockFile);
	}

	_logger.log(WARNING, "shutdown daemon ");
	return ;
}

void lpp::server::daemon(const size_t& p_port){
	_lockFd = open((_daemonLockPath + _daemonLockFile).c_str(), O_CREAT | O_RDWR, 0644);
    if (_lockFd == -1) {
        throw std::runtime_error("failed to open lock file");
    }

    if (flock(_lockFd, LOCK_EX | LOCK_NB) == -1) {
        close(_lockFd);
        throw std::runtime_error("daemon already running (lock file busy)");
    }

	if (!_running.load()){
		_p_port = p_port;
		int pid = fork();
		if (pid < 0){
			throw std::runtime_error("server: fork failed");
		}
		else if (pid > 0){
			lpp::cout << "kill current parent" << std::endl;
			lpp::cout << "run daemon server" << std::endl;
			ftruncate(_lockFd, 0);
			dprintf(_lockFd, "%d\n", pid);
			lpp::cout << "write child PID in " + _daemonLockPath + _daemonLockFile << std::endl;
			return ;
		}
		if (!pid){
			_running.store(true);
			_daemonLoop();
		}
	}
	else
		lpp::cout << "server already running" << std::endl;

}

void lpp::server::start(const size_t& p_port){
	if (!_running.load()){
		_p_port = p_port;
		_loop = std::thread(&server::_workerLoop, this);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (!_running.load() && _exc){
		disconnect();
		std::rethrow_exception(_exc);
	}
}

void lpp::server::disconnect(){
	_running.store(false);
	if(_loop.joinable())
		_loop.join();
	if (_socket != -1)
		close(_socket);
}

lpp::server::~server(){
	disconnect();
}

lpp::server::server() : _p_port(8080), _exc(nullptr), _lockFd(-1),
	_logFile("server.log"),
	_lockFile("server.lock"),
	_logPath("/var/log/libftpp/server/lpp_server/"),
	_lockPath("/var/lock/libftpp/server/lpp_server/"),
	_daemonLogFile("daemon.log"),
	_daemonLockFile("daemon.lock"),
	_daemonLogPath("/var/log/libftpp/server/lpp_daemon/"),
	_daemonLockPath("/var/lock/libftpp/server/lpp_daemon/")
{
	_running.store(false);
}

void lpp::server::defineAction(const message::Type& messageType, const std::function<void(long long clientID, const message& msg)>& action){
	if (_running.load())
		return;
	std::lock_guard<std::mutex> lock(_mutex);
	_actions[messageType] = action;
}

void lpp::server::sendTo(const message& message, long long clientID){
	std::string data = (std::to_string(message.type()) + '|' + message.str() + '\n');
	_logger.log(INFO, "message type: " + std::to_string(message.type()));
	_logger.log(INFO, "send to client[" + std::to_string(clientID+1) + "]");
	if (::send(clientID, data.c_str(), data.size(), 0) < 0)
		_logger.log(ERROR, "failed to send data to client[" + std::to_string(clientID) + "]");
}

bool lpp::server::config(){
	// Default action configuration
    defineAction(1, [&](long long clientID, const lpp::message& msg){
        int value;
        msg >> value;
        lpp::cout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 1 with double the value
        lpp::message replyMsg(1);
        replyMsg << (value * 2);
        sendTo(replyMsg, clientID);
    });

    defineAction(10, [&](long long clientID, const lpp::message& msg){
		std::string value;
        lpp::message replyMsg(10);

        msg >> value;
		if (value != "ping")
			replyMsg << "ping?";
		else{
			replyMsg << "pong";
		}

        sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    defineAction(2, [&](long long clientID, const lpp::message& msg){
        size_t length;
        std::string text;
        msg >> length;
        text.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            char c;
            msg >> c;
            text.push_back(c);
        }
        lpp::cout << "Received a string '" << text << "' of length " << length << " from client " << clientID << std::endl;
		lpp::message tmp(2);
		tmp << "message received";
		sendTo(tmp, clientID);
    });

	return true;
}

bool lpp::server::execute(){
	if (!_running.load())
		start(_p_port);
	return true;
}

void lpp::server::killDaemon(){
	std::lock_guard<std::mutex> m(_mutex);
	std::ifstream lock(_daemonLockPath + _daemonLockFile);
	std::string tmp;

	if (!lock.is_open()){
		lpp::cout << "no instance running" << std::endl;
		return ;
	}
	pid_t pid = 0;
	if (!(lock >> pid))
		throw std::runtime_error("Faile to read lock file");
	if (kill(pid, SIGINT) == -1){
		throw std::runtime_error("Failed to send SIGINT to daemon");
	}
	lpp::cout << "kill daemon process [" << pid << "] with SIGINT" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

std::string lpp::server::exec(std::string cmd){
	std::string result("\n");
    char buffer[128];

	_logger.log(INFO, "exec command: \""+ cmd + "\"");
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "error";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

void lpp::server::setLogFileName(std::string name){_logFile = name;}
void lpp::server::setLockFileName(std::string name){_lockFile = name;}
std::string lpp::server::getLogFile(){ return _logFile;}
std::string lpp::server::getLockFile(){ return _lockFile;}


void lpp::server::setDaemonLogFile(std::string name){_daemonLogFile = name;}
void lpp::server::setDaemonLockFile(std::string name){_daemonLockFile = name;}
std::string lpp::server::getDaemonLogFile(){ return _daemonLogFile;}
std::string lpp::server::getDaemonLockFile(){ return _daemonLockFile;}


lpp::logger& lpp::server::getLogger(){return _logger;}

lpp::server::authentification::authentification() : _isConnected(false), _username("guest"){}

lpp::server::authentification::~authentification(){}
