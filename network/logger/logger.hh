/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:05:39 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/18 15:07:02 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../design_patterns/non_copyable/non_copyable.hh"

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

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
			std::ofstream logFile;
			std::string _filePath;
			std::string levelToString(LogLevel level);
			bool _deleteFile;
			bool _printFormat;

		public:
			logger();
			logger(const std::string& filePath, bool deleteFile = false, bool printformat = true);
			~logger();

			void log(LogLevel level, const std::string& message);
			bool is_open();
			void open();

			void setFilePath(const std::string);
			void setDeleteFile(bool);
			void setPrintFormat(bool);
	};
}
