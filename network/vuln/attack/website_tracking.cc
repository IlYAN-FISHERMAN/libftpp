/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   website_tracking.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:14:54 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/26 20:15:21 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "ip.hh"

int main(int ac, char **av){

	try {
		if (ac == 1){
			lpp::ip::usage();
			return 0;
		}

		std::vector<std::string> tmp(av + 1, av + ac);

		lpp::ip ip;

		if (!ip.parse(tmp))
			ip.run();

	} catch(std::runtime_error &e){
		lpp::logger::cout(lpp::CRITICAL, e.what());
	}
	return 0;
}
