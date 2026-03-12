/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.hh                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 16:29:16 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:28:10 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/server.hh"
#include "../client/client.hh"

namespace lpp{
	class prompt : public lpp::IResponsability_chain{
		private:
			lpp::client &_client;
			std::string _prefix;
			bool execute();
			bool config();
		public:
			prompt(lpp::client &);
			~prompt();
			void run();
			void setPrefix(std::string);
	};
}
