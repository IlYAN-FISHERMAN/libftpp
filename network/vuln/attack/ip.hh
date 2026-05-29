/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.hh                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:43:07 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/29 14:45:33 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
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
			std::multimap<std::string, std::string> _ips;
			lpp::unique_chrono _chrono;
			std::string _sys_name;
			lpp::logger _logger;

			bool _isOpenPort(int, bool = false) const noexcept;

			bool _isOpenPort(std::vector<int>, [[maybe_unused]] bool = false) const noexcept;

			std::string _foundUser(std::string &, bool = false) const noexcept;

			std::string _nextLine() const noexcept;

		public:
			ip();
			~ip();
			explicit ip(std::vector<std::string>&);

			explicit ip(const ip &);
			ip& operator=(const ip &);

			void addIp(const std::string &)noexcept;

			void clearIps();

			void clearIps(const std::string &) noexcept;

			void clearOptions() noexcept;

			void addOption(const std::string &) noexcept;

			void clear() noexcept;

			void setIterationTime(const int, const std::chrono::seconds) noexcept;

			void setIsPrompt(const bool) noexcept;

			bool setLogFile(const std::string &) noexcept;

			void setIs42(const bool) noexcept;

			void setNmapOutput(const bool) noexcept;

			void setPortOutput(const bool) noexcept;

			int parse(std::vector<std::string>&) noexcept(false);

			std::vector<std::string> who(const std::vector<std::string> &) const noexcept(false);

			[[nodiscard("very big data")]] std::optional<std::pair<std::vector<std::string>, std::vector<std::vector<int>>>> run() noexcept(false);

			[[nodiscard]] static bool isIp(const std::string &, const bool = false) noexcept;

			[[nodiscard]] static bool isDomaine(const std::string &, const bool = false) noexcept;
			static void usage();

			std::string get(const std::string = "en0");
	};

}
