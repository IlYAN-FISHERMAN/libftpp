/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.hh                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:43:07 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/29 14:37:32 by ilyanar          ###   LAUSANNE.ch       */
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
			[[no_unique_address]] bool _prompt;
			[[no_unique_address]] bool _is42;
			[[no_unique_address]] bool _https;
			[[no_unique_address]] bool _openPort;
			[[no_unique_address]] bool _nmapOutput;
			[[no_unique_address]] bool _termuxOutput;

			int _loopTime;
			std::chrono::seconds _delay;
			std::multimap<std::string, std::string> _ips;
			lpp::unique_chrono _chrono;
			std::string _sys_name;
			lpp::logger _logger;

			[[gnu::hot]]
			bool _isOpenPort(int, bool = false) const noexcept;

			[[gnu::hot]]
			bool _isOpenPort(std::vector<int>, [[maybe_unused]] bool = false) const noexcept;

			[[gnu::hot]]
			[[nodiscard]] std::string _foundUser(std::string &, bool = false) const noexcept;

			[[gnu::cold]]
			[[maybe_unused]] std::string _nextLine() const noexcept;

		public:
			ip();
			~ip();
			explicit ip(std::vector<std::string>&);

			explicit ip(const ip &);
			ip& operator=(const ip &);

			[[gnu::cold]]
			void addIp(const std::string &)noexcept;

			[[gnu::cold]]
			consteval void clearIps();

			[[gnu::cold]]
			void clearIps(const std::string &) noexcept;

			[[gnu::cold]]
			void clearOptions() noexcept;

			[[gnu::cold]]
			void addOption(const std::string &) noexcept;

			[[gnu::cold]]
			void clear() noexcept;

			[[gnu::cold]]
			void setIterationTime(const int, const std::chrono::seconds) noexcept;

			[[gnu::cold]]
			void setIsPrompt(const bool) noexcept;

			[[gnu::cold]]
			bool setLogFile(const std::string &) noexcept;

			[[gnu::cold]]
			void setIs42(const bool) noexcept;

			[[gnu::cold]]
			void setNmapOutput(const bool) noexcept;

			[[gnu::cold]]
			void setPortOutput(const bool) noexcept;

			[[gnu::hot]]
			[[maybe_unused]] int parse(std::vector<std::string>&) noexcept(false);

			[[maybe_unused]] std::vector<std::string> who(const std::vector<std::string> &) const noexcept(false);

			[[gnu::hot]]
			[[nodiscard("very big data")]] std::optional<std::pair<std::vector<std::string>, std::vector<std::vector<int>>>> run() noexcept(false);

			[[gnu::hot]]
			[[nodiscard]] static bool isIp(const std::string &, const bool = false) noexcept;

			[[gnu::hot]]
			[[nodiscard]] static bool isDomaine(const std::string &, const bool = false) noexcept;
			static void usage();

			[[gnu::cold]]
			std::string get(const std::string = "en0");
	};

}
