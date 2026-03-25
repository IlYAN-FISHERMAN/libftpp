/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:21:10 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/25 14:13:20 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hh"

static sig_atomic_t sigCode = 0;

static void handleSigint(int){
	sigCode = 1;
}

static const char* payload[] = {
    "To: recipient@gmail.com\r\n",
    "From: you@gmail.com\r\n",
    "Subject: Test mail\r\n",
    "\r\n",
    "Hello from C++!\r\n",
    NULL
};

struct upload_status {
    int lines_read;
};

std::string lpp::server::help(){
	std::stringstream help;

	help << "ACTIONS" << std::endl;
	help << std::endl;
	help << "The matt-daemon server has predefined possible actions. By default, three of them exist, with the following format:" << std::endl;
	help << std::endl;
	help << "   code [1] server authentification system: \t\"1|username=<user> password=<server_password>\"" << std::endl;
	help << "   code [2] email sender: \t\t\t\"2|To=<example@mail.com> Body=<text_to_send>\"" << std::endl;
	help << "   code [3] remote shell of the server: \t\"3|<command>\"" << std::endl;
	help << std::endl;
	help << "When running the server, you must define at least the SERVER_PASSWORD variable in a .env file located in the build folder.\nSee the env.example file for more details." << std::endl;
	help << std::endl;

	return help.str();
}

size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp) {
    upload_status* upload = (upload_status*)userp;
    const char* data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
        return 0;

    data = payload[upload->lines_read];

    if (data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload->lines_read++;
        return len;
    }
    return 0;
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

lpp::server::~server(){
	disconnect();
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


	if (ss.str() == "clear"){
		::send(_pollFd[id].fd, "\33c", 3, 0);
	}
	else if (ss.str() == "help"){
		std::string help = this->help();
		::send(_pollFd[id].fd, help.c_str(), help.size(), 0);
	}
	else if (ss >> code >> sep){
		message msg(code);
		if (_actions.find(code) != _actions.end() && sep == '|'){
			std::string tmp;
			std::getline(ss >> std::ws, tmp);
			_logger.log(INFO, _authorized[_pollFd[id].fd]._username + " run action type " + std::to_string(code));
			msg << tmp;
			_actions[code](_pollFd[id].fd, msg);
		}
		else{
			_logger.log(ERROR, _authorized[_pollFd[id].fd]._username + " tried to launch a non-existent action");
			::send(_pollFd[id].fd, "action not found\n", 16, 0);
		}
	} else{
			_logger.log(INFO, _authorized[_pollFd[id].fd]._username + " input: \"" + _msg[id] + "\"");
			if (_msg[id] == "\n" || _msg[id].empty())
				::send(_pollFd[id].fd, "", 0, 0);
			else
				::send(_pollFd[id].fd, "message received\n", 18, 0);
	}
}

bool lpp::server::_serverAuthentification(std::stringstream &ss, int &id){
	int code = 0;
	char sep = 0;

	_logger.log(lpp::LogLevel::INFO, _authorized[id]._username + " try to log in");
	if (_whiteList.find(getIPv4(id)) != _whiteList.end()){
		if ((ss >> code >> sep) && code == 1){
			message msg(code);
			if (_actions.find(code) != _actions.end() && sep == '|'){
				_logger.log(INFO, "received authentification request from " + _authorized[id]._username);
				std::string tmp;
				std::getline(ss >> std::ws, tmp);
				msg << tmp;
				_actions[code](id, msg);
				return _authorized[id]._isConnected;
			}
		}
	} else
		_logger.log(WARNING, _authorized[id]._username + "[" + getIPv4(id) + "] not in the white list");

	::send(id, "Connection refused\n", 20, 0);
	return false;
}

std::string lpp::server::getIPv4(int clientId){
	sockaddr_in addr{};
	socklen_t len = sizeof(addr);
	getpeername(clientId, (sockaddr*)&addr, &len);
	char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

	return std::string(ip);
}

void lpp::server::_connectUser(int clientId){
	_authorized[clientId] = authentification("guest[" + std::to_string(clientId) + ']');
	_logger.log(INFO, "new client, " + _authorized[clientId]._username + " connected");

	std::string ipv4(getIPv4(clientId));
	if (_whiteList.find(ipv4) != _whiteList.end())
		_logger.log(WARNING, _authorized[clientId]._username + "[" + ipv4 + "] white list authorisation");
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
						_connectUser(acpt);
					}
					continue;
				}
				char buffer[BUFFER_SIZE]{0};
				ssize_t n = read(_pollFd[i].fd, buffer, sizeof(buffer));
				_logger.log(INFO, "server read " + std::to_string(n) + " bytes");
				if (n <= 0){
					_logger.log(INFO, "close " + _authorized[_pollFd[i].fd]._username + " connection");
					_authorized.erase(_pollFd[i].fd);
					close(_pollFd[i].fd);
					_pollFd.erase(_pollFd.begin() + i);
					_msg.erase(_msg.begin() + i);
					i--;
				} else{
					_msg[i].append(buffer, n);
					if (_msg[i].find('|') == std::string::npos)
						std::erase(_msg[i], '\n');
					std::stringstream ss(_msg[i]);
					if (_msg[i] == "quit" || _msg[i] == "exit"){
						_logger.log(INFO, _authorized[_pollFd[i].fd]._username + " request quit");
						close(_pollFd[i].fd);
						_pollFd.erase(_pollFd.begin() + i);
						_msg.erase(_msg.begin() + i);
						i--;
					} else if (_tryConnection(_pollFd[i].fd)){
						_executeMessage(ss, i);
					} else if (!_serverAuthentification(ss, _pollFd[i].fd))
						_logger.log(INFO, _authorized[_pollFd[i].fd]._username + " connection refused");
					else
						_logger.log(INFO, _authorized[_pollFd[i].fd]._username + " input: \"" + _msg[i] + "\"");
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

	_logger.log(WARNING, "shutdown daemon");
	return ;
}

void lpp::server::_getEnv(){
	std::ifstream env(".env", std::ios::out);

	if (!env.is_open()){
		throw std::runtime_error("Need .env file with at least SERVER_PASSWORD variable defined");
	}
	
	while(!env.eof()){
		std::string str;
		std::string key;
		std::string value;

		std::getline(env, str, '\n');
		if (str.empty())
			continue;
		if (str.find('=') == std::string::npos)
			throw std::logic_error(".env file: bad format");
		key = str.substr(0, str.find('='));
		value = str.substr(str.find('=') + 1);
		if (key.empty() || value.empty())
			throw std::logic_error(".env file: empty field");
		if (key == "WHITE_LIST")
			_whiteList.insert(value);
		else
			_env[key] = value;
		key.clear();
		value.clear();
	}

	if (_env.find("SERVER_PASSWORD") == _env.end())
		throw std::logic_error("Need .env file with at least SERVER_PASSWORD variable defined");
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
			defineAction(1, [this](long long clientID, const lpp::message& msg){
				std::string user;
				std::string password;
				std::stringstream reply;
				std::string tmp;

				msg >> tmp;
				if (tmp.find('=') == std::string::npos){
					reply << "bad format";
				} else{
					if (tmp.substr(0, tmp.find('=')) == "username"){
						user = tmp.substr(tmp.find('=') + 1);
						msg >> tmp;
						if (tmp.find('=') == std::string::npos || !msg.eof())
							reply << "bad format";
						else if (tmp.substr(0, tmp.find('=')) == "password"){
							password = tmp.substr(tmp.find('=') + 1);
							if (user.empty() || password.empty())
								reply << "empty field";
							else if (isPasswd(password)){
								_logger.log(lpp::LogLevel::INFO,  user + " logged successfully");
								enableUser(clientID, user);
								reply << "connected";
							} else
								reply << "authentification failed";
						}
						else
							reply << "bad format";
					} else{
						reply << "bad format";
					}
				}

				reply << std::endl;
				std::string str(reply.str());
				::send(clientID, str.data(), str.size(), 0);
			});

			defineAction(2, [this](long long clientID, const lpp::message& msg){
					CURLcode res = CURLE_OK;
					CURL *curl = curl_easy_init();

					std::string reply;
					std::string To;
					std::string Body;
					std::string tmp;
					std::stringstream ss(msg.str());

					ss >> tmp;
					if (tmp.find('=') == std::string::npos){
						reply = "bad format";
					} else{
						if (tmp.substr(0, tmp.find('=')) == "To"){
							To = tmp.substr(tmp.find('=') + 1);
							ss >> tmp;
							if (tmp.find('=') == std::string::npos || tmp.substr(0, tmp.find('=')) != "Body")
								reply = "need body field";
							else{
								Body = tmp.substr(tmp.find('=') + 1);
								tmp.clear();
								while(!ss.eof()){
									std::getline(ss, tmp);
									if (ss.eof())
										Body += tmp;
									else
										Body += tmp + "\n";
								}
								if (To.empty())
									reply = "empty mail destination";
								else if(curl){
									curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
									curl_easy_setopt(curl, CURLOPT_URL, _env["CURL_SMTP_SERVER"].c_str());
									curl_easy_setopt(curl, CURLOPT_MAIL_FROM, _env["CURL_EMAIL_PROVIDER"].c_str());
									curl_easy_setopt(curl, CURLOPT_USERNAME, _env["CURL_EMAIL_PROVIDER"].c_str());
									curl_easy_setopt(curl, CURLOPT_PASSWORD, _env["CURL_EMAIL_PASSWORD"].c_str());

									struct curl_slist *recipients = nullptr;
									recipients = curl_slist_append(recipients, To.c_str());
									curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
									curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
									curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

									res = curl_easy_perform(curl);

									curl_easy_cleanup(curl);
									curl_slist_free_all(recipients);
									curl_global_cleanup();
								  }else
									::send(clientID, "curl setup failed", 19, 0);
								if (res != CURLE_OK)
									reply = "fail to send email";
								else
									reply = "email send succefully";
							}
						} else
							reply = "bad format";
					}

					reply += '\n';
					::send(clientID, reply.c_str(), reply.size(), 0);
			});
			defineAction(3, [this](long long clientID, const lpp::message& msg){
				std::string rtn(exec(msg.str()));
				::send(clientID, rtn.c_str(), rtn.size(), 0);
			});

			_getEnv();
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

lpp::server::authentification::authentification(std::string name, bool autorized) : _isConnected(autorized), _username(name){}

lpp::server::authentification::~authentification(){}

bool lpp::server::isPasswd(std::string passwd){
	return _env["SERVER_PASSWORD"] == passwd;
}

void lpp::server::enableUser(int clientId, std::string username){ _authorized[clientId] = authentification(username, true);}

void lpp::server::setInWhiteList(std::string ip){_whiteList.insert(ip);}
