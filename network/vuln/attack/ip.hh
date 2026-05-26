/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.hh                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:43:07 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/26 21:19:33 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>

#include <iostream>
#include <regex>

#include "network/vuln/nmap/nmap.hh"
#include "time/chronometer/chronometer.hh"

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

			std::chrono::seconds _loopTime;
			std::chrono::seconds _delay;
			std::vector<std::string> _args;
			std::multimap<std::string, std::string> _ips;
			lpp::unique_chrono _chrono;
			std::string _sys_name;

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

			void addIp(std::string &);
			void clearIps();
			void clearIps(std::string &);

			void clearOptions();
			void addOption(std::string &);

			void clear();

			void setIterationTime(std::chrono::seconds iter, std::chrono::seconds delay);
			void setIsPrompt(bool);

			void setIs42(bool);
			void setNmapOutput(bool);
			void setPortOutput(bool);

			[[maybe_unused]] int parse(std::vector<std::string>&);

			[[maybe_unused]] std::vector<std::string> who(std::vector<std::string> &);

			[[nodiscard]] std::optional<std::pair<std::vector<std::string>, std::vector<std::vector<int>>>> run() noexcept(false);

			[[nodiscard]] static bool isIp(const std::string &, bool = false);
			static void usage();
	};
}
