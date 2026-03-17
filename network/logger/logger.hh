/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:05:39 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/17 17:19:38 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../design_patterns/non_copyable/non_copyable.hh"

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

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

		public:
			logger();
			logger(const std::string& filePath);
			~logger();

			void log(LogLevel level, const std::string& message);
			bool is_open();
			void open();

			void setFilePath(const std::string);
	};
}
