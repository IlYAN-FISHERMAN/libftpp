/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:05:39 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/17 17:08:43 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hh"
#include "../../utils/color.hh"

std::string lpp::logger::levelToString(lpp::LogLevel lvl){
	switch (lvl) {
	case DEBUG:
		return "[DEBUG]";
	case INFO:
		return "[INFO]";
	case WARNING:
		return "[WARNING]";
	case ERROR:
		return "[ERROR]";
	case CRITICAL:
		return "[CRITICAL]";
	default:
		return "UNKNOWN";
	}
}

lpp::logger::logger(const std::string& filePath) : _filePath(filePath){}

lpp::logger::logger() : _filePath("/var/log/libftpp/server/daemon.log"){}

lpp::logger::~logger(){
	if (logFile.is_open())
		logFile.close();
}

void lpp::logger::log(LogLevel level, const std::string& message){
	time_t now = time(0);
	tm* timeinfo = localtime(&now);
	char timestamp[20];
	strftime(timestamp, sizeof(timestamp),
			 "%Y-%m-%d %H:%M:%S", timeinfo);

	std::ostringstream logEntry;
	logEntry << "[" << timestamp << "] " << levelToString(level) << ": " << message << std::endl;
	if (logFile.is_open()) {
		logFile << logEntry.str();
		logFile.flush();
	}
}

bool lpp::logger::is_open(){return logFile.is_open();}

void lpp::logger::open(){
	logFile.open(_filePath, std::ios::app);
}

void lpp::logger::setFilePath(const std::string name){_filePath = name;}
