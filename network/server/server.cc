/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:10 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/19 12:04:01 by ilyanar          ###   LAUSANNE.ch       */
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
		try{
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


void lpp::server::_daemonLoop(){
	_logger.open();
	_logger.setPrintFormat(true);
	if (!_logger.is_open()){
		lpp::logger("/dev/stderr").log(ERROR, "Error opening log file.");
		return ;
	}
	_logger.log(DEBUG, "created child");
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket < 0)
			throw std::runtime_error("socket failed");
		_logger.log(INFO, "started...");
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
						_logger.log(INFO, "new client[" + std::to_string(_pollFd[i].fd) + "] connected");
					}
					continue;
				}
				char buffer[BUFFER_SIZE]{0};
				ssize_t n = read(_pollFd[i].fd, buffer, sizeof(buffer));
				_logger.log(INFO, "server read " + std::to_string(n) + " bytes");
				if (n <= 0){
					_logger.log(INFO, "close client[" + std::to_string(_pollFd[i].fd) + "]");
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
							_logger.log(ERROR, "client[" + std::to_string(_pollFd[i].fd - 1) + "] tried to launch a non-existent action");
							::send(_pollFd[i].fd, "action not found\n", 16, 0);
						}
					}
					else{
						if (_msg[i] == "quit" || _msg[i] == "quit\n"){
							close(_pollFd[i].fd);
							_pollFd.erase(_pollFd.begin() + i);
							_msg.erase(_msg.begin() + i);
							i--;
							_logger.log(INFO, "client[" + std::to_string(_pollFd[i].fd - 1) + "] request quit");
						} else{
							_logger.log(INFO, "client[" + std::to_string(_pollFd[i].fd - 1) + "] input: " + _msg[i]);
							::send(_pollFd[i].fd, "message received", 17, 0);

						}
					}
					_msg[i].clear();
				}
			}
		}
	}
	_logger.log(INFO, "stopping server... code[" + std::to_string(sigCode) + "]");

	for (auto pol : _pollFd)
		close(pol.fd);
	_logger.log(INFO, "clear poll structure");

	if (_lockFd != -1){
		flock(_lockFd, LOCK_UN);
		close(_lockFd);
		unlink(_lockFile.c_str());
		_logger.log(INFO, "unlink " + _lockFile);
	}
	return ;
}

void lpp::server::daemon(const size_t& p_port){
	_lockFd = open(_lockFile.c_str(), O_CREAT | O_RDWR, 0644);
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
			lpp::cout.setPrefix("daemon: ");
			lpp::cout << "kill current parent" << std::endl;
			lpp::cout << "run daemon server" << std::endl;
			ftruncate(_lockFd, 0);
			dprintf(_lockFd, "%d\n", pid);
			lpp::cout << "write child PID in " + _lockFile << std::endl;
			return ;
		}
		if (!pid){
			_running.store(true);
			_daemonLoop();
		}
	}
	else
		lpp::cout << "server already running" << std::endl;

	lpp::cout.setPrefix("");
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
	if (_running.load()){
		_running.store(false);
		if(_loop.joinable())
			_loop.join();
	}
	if(_loop.joinable())
		_loop.join();
	if (_socket != -1)
		close(_socket);
}

lpp::server::~server(){
	disconnect();
}

lpp::server::server() : _exc(nullptr), _fileName("daemon.log"), _lockFd(-1),
	_lockFile("/var/lock/libftpp/server/matt_daemon/daemon.lock"),
	_logPath("/var/log/libftpp/server/matt_daemon/"),
	_lockPath("/var/lock/libftpp/server/matt_daemon/"){
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
		start(8080);
	return true;
}

void lpp::server::killDaemon(){
	std::lock_guard<std::mutex> m(_mutex);
	std::ifstream lock(_lockFile);
	std::string tmp;

	if (!lock.is_open()){
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

lpp::logger& lpp::server::getLogger(){return _logger;}

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
	_logger.log(INFO, "delete " + _execFile);
    return result;
}

void lpp::server::setDaemonLogFileName(std::string name){
	_fileName = _logPath + logger::getDate() + '_' + name;
	_logger.setFilePath(_fileName);
}

void lpp::server::setDaemonLockFileName(std::string name){_lockFile = _lockPath + name;}
void lpp::server::setDaemonExecFileName(std::string name){_execFile = _logPath + name;}

std::string lpp::server::getDaemonLogFileName(){ return _logger.getFilePath();}
std::string lpp::server::getDaemonLockFileName(){ return _lockFile;}
std::string lpp::server::getDaemonExecFileName(){ return _execFile;}
