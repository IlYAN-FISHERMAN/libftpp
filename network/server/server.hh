/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:20:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/25 14:10:45 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "network/client/client.hh"
#include "network/logger/logger.hh"
#include "design_patterns/responsability_chain/responsability_chain.hh"
#include "design_patterns/non_copyable/non_copyable.hh"
#include "time/chronometer/chronometer.hh"
#include <thread>
#include <exception>
#include <cstdio>
#include <sys/file.h>
#include <set>
#include <stdio.h>
#include <curl/curl.h>
#include <filesystem>

#define BUFFER_SIZE 1024

namespace lpp{
	class server : public lpp::IResponsability_chain, public lpp::NonCopyable{
		public :
		 using Callback = std::function<void(long long, const message &)>;
		private:
			class authentification{
				private:
					friend lpp::server;
					bool _isConnected;
					std::string _username;
				public:
					authentification();
					authentification(std::string, bool = false);
					~authentification();
			};
			int 				_socket;
			struct sockaddr_in  _serv_addr;
			std::unordered_map<long long, int> _clients;
			std::unordered_map<message::Type, Callback> _actions;
			std::unordered_map<int, authentification> _authorized;
			std::unordered_map<std::string, std::string> _env;
			size_t _p_port;
			std::exception_ptr _exc;

			lpp::logger _logger;
			lpp::chronometer _chrono;

			std::set<std::string> _whiteList;

			int _lockFd;

			std::string _logFile;
			std::string _lockFile;

			std::string _logPath;
			std::string _lockPath;


			std::string _daemonLogFile;
			std::string _daemonLockFile;
			
			std::string _daemonLogPath;
			std::string _daemonLockPath;

			std::vector<pollfd> _pollFd;
			std::vector<std::string> _msg;

			std::mutex _mutex;
			std::thread	_loop;
			std::atomic<bool> _running;
			bool _isDaemon;

			void _workerLoop();
			void _daemonLoop();
			bool _tryConnection(int);
			void _executeMessage(std::stringstream&, size_t&);
			bool _serverAuthentification(std::stringstream&, int&);
			void _connectUser(int);
			void _getEnv();
			void _parseData();

			bool config()	override;
			bool execute()	override;
			std::string help();
		public:
			server();
			~server();

			void start(const size_t& p_port);
			void disconnect();

			void daemon(const size_t& p_port);
			void killDaemon();

			void defineAction(const message::Type& messageType, const std::function<void(long long clientID, const message& msg)>& action);
			void sendTo(const message& message, long long clientID);

			void setLogFileName(std::string);
			void setLockFileName(std::string);
			std::string getLogFile();
			std::string getLockFile();

			void setDaemonLogFile(std::string);
			void setDaemonLockFile(std::string);
			std::string getDaemonLogFile();
			std::string getDaemonLockFile();

			std::string exec(std::string);
			lpp::logger& getLogger();

			bool isPasswd(std::string);

			void enableUser(int, std::string);
			std::string getIPv4(int);
			void setInWhiteList(std::string);
	};
}
