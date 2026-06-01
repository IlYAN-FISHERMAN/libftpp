/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:05:39 by ilyanar           #+#    #+#             */
/*   Updated: 2026/06/01 22:41:24 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "design_patterns/non_copyable/non_copyable.hh"
#include "iostream/thread_safe_iostream.hh"
#include <ctime>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <filesystem>

namespace lpp{
	enum LogLevel {
		INFO,
		DEBUG,
		WARNING,
		ERROR,
		CRITICAL
	};

	class logger : lpp::NonCopyable{
		private:
			mutable std::ofstream _logFile;
			std::string _filePath;
			static std::string levelToString(LogLevel level);
			bool _printFormat;
			bool _isStdout;
			bool _deleteFile;

		public:
			logger();
			logger(const logger&);
			logger& operator=(const logger&);
			logger(const std::string& filePath, bool deleteFile = false, bool printformat = true, bool isStdout = false);
			~logger();

			void log(const LogLevel level, const std::string& message) const noexcept;
			bool is_open();
			void open();
			void close();

			void setFilePath(const std::string);
			void setDeleteFile(bool);
			void setPrintFormat(bool);
			void setIsStdout(bool);

			std::string getFilePath();
			bool getDeleteFile();
			bool getPrintFormat();

			static std::string getDate();
			static std::string getLog(LogLevel level);
			static void cout(LogLevel level, const std::string& message, bool endl = true);
	};
}
