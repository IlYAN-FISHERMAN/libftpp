/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.hh                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:43:07 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/27 14:28:48 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>

#include <iostream>
#include <regex>

#include "network/vuln/nmap/nmap.hh"
#include "time/chronometer/chronometer.hh"
#include "design_patterns/memento/unique_memento.hh"

namespace lpp {
	class ip{
		private:
			lpp::nmap _map;
			bool _prompt;
			bool _is42;
			bool _https;
			bool _openPort;
			bool _nmapOutput;
			bool _termuxOutput;

			int _loopTime;
			std::chrono::seconds _delay;
			std::vector<std::string> _args;
			std::multimap<std::string, std::string> _ips;
			lpp::unique_chrono _chrono;
			std::string _sys_name;
			lpp::logger _logger;

			[[nodiscard]] std::string _foundUser(std::string &, bool = false);

			bool _isOpenPort(int, bool = false);
			bool _isOpenPort(std::vector<int>, bool = false);

			[[maybe_unused]] std::string _nextLine();

		public:
			ip();
			~ip();
			ip(std::vector<std::string>&);

			ip(const ip &);
			ip& operator=(const ip &);

			void addIp(const std::string &);
			void clearIps();
			void clearIps(const std::string &);

			void clearOptions();
			void addOption(const std::string &);

			void clear();

			void setIterationTime(const int, const std::chrono::seconds);
			void setIsPrompt(const bool);
			bool setLogFile(const std::string &);

			void setIs42(const bool);
			void setNmapOutput(const bool);
			void setPortOutput(const bool);

			[[maybe_unused]] int parse(std::vector<std::string>&);

			[[maybe_unused]] std::vector<std::string> who(const std::vector<std::string> &);

			[[nodiscard]] std::optional<std::pair<std::vector<std::string>, std::vector<std::vector<int>>>> run() noexcept(false);

			[[nodiscard]] static bool isIp(const std::string &, const bool = false);
			[[nodiscard]] static bool isDomaine(const std::string &, const bool = false);
			static void usage();
	};
}
