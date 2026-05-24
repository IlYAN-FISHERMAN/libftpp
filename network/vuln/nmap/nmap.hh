/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.hh                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:16:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/24 23:04:00 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "system/system.hh"
#include "time/time.hh"
#include "network/logger/logger.hh"
#include <vector>
#include <regex>

namespace lpp{
	class nmap{
		private:
			bool _async;
			std::string _options;
			lpp::system _sys;
			lpp::unique_chrono _chrono;
			lpp::logger _logger;

		public:
			nmap();
			~nmap();
			std::vector<std::string> sniff(std::vector<std::string> &ip);
			void setAsync(bool);
			void setOptions(std::string);

			std::vector<int> ports(const std::string &) const;
			std::string sniff(const std::string ip);
	};
}
