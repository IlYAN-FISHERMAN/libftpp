/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:05:39 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/22 17:29:36 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hh"
#include "../../utils/color.hh"
#include <unistd.h>

std::string lpp::logger::levelToString(lpp::LogLevel lvl){
	switch (lvl) {
	case DEBUG:
		return C_CYAN "[DEBUG]" C_RESET;
	case INFO:
		return C_GREEN "[INFO]" C_RESET;
	case WARNING:
		return C_YELLOW "[WARNING]" C_RESET;
	case ERROR:
		return C_BRED "[ERROR]" C_RESET;
	case CRITICAL:
		return C_RED "[CRITICAL]" C_RESET;
	default:
		return "UNKNOWN";
	}
}

lpp::logger::logger(const std::string& filePath, bool deleteFile, bool printFormat, bool isStdout) : _filePath(filePath), _deleteFile(deleteFile), _printFormat(printFormat), _isStdout(isStdout){}

lpp::logger::logger() : _filePath(), _printFormat(true), _isStdout(false){}

lpp::logger::~logger(){
	if (logFile.is_open()){
		logFile.close();
		if (_deleteFile)
			std::filesystem::remove(_filePath);
	}
}

std::string lpp::logger::getDate(){
	time_t now = time(0);
	tm* timeinfo = localtime(&now);

	std::ostringstream logEntry;
	char timestamp[20];
	strftime(timestamp, sizeof(timestamp),
		 "%Y-%m-%d %H:%M:%S", timeinfo);

	return timestamp;
}

std::string lpp::logger::getLog(lpp::LogLevel level){
	return "[" + getDate() + "] " + levelToString(level) + ": ";
}

void lpp::logger::log(LogLevel level, const std::string& message){
	std::ostringstream logEntry;
	if (_printFormat){
		logEntry << getLog(level) << message << std::endl;
	}
	else
		logEntry << message;
	if (!_isStdout){
		if (logFile.is_open()) {
			logFile << logEntry.str();
			logFile.flush();
		}
	} else
		lpp::cout << logEntry.str() << std::flush;
}

void lpp::logger::cout(LogLevel level, const std::string& message){
	lpp::cout << getLog(level) << message << std::endl;
}

bool lpp::logger::is_open(){return logFile.is_open();}

void lpp::logger::open(){
	logFile.open(_filePath, std::ios::trunc);
}

void lpp::logger::setFilePath(const std::string name){_filePath = name;}
void lpp::logger::setDeleteFile(bool deleteFile){_deleteFile = deleteFile;}
void lpp::logger::setPrintFormat(bool printFormat){_printFormat = printFormat;}
void lpp::logger::setIsStdout(bool isStdout){_isStdout = isStdout;}

std::string lpp::logger::getFilePath(){return _filePath;}
bool lpp::logger::getDeleteFile(){return _deleteFile;}
bool lpp::logger::getPrintFormat(){return _printFormat;}
