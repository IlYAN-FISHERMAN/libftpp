/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   website_tracking.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:14:54 by ilyanar           #+#    #+#             */
/*   Updated: 2026/06/16 16:36:36 by ilyanar          ###   LAUSANNE.ch       */
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
		if ((std::find(tmp.begin(), tmp.end(), "--help") != tmp.end() || std::find(tmp.begin(), tmp.end(), "-h") != tmp.end())){
			lpp::ip::usage();
			return 0;
		}

		lpp::ip ip;
		if (!ip.parse(tmp)){
			if (ip.run()){
				;
				// auto &[ips, ports] = it.value();
				// for (auto &ip : ips){
				// 	lpp::cout  << ip << std::endl;
				// }
				// for (auto &port : ports){
				// 	for (auto &p : port)
				// 		lpp::cout << " " << p;
				// 	lpp::cout << std::endl;
				// }
			}
		}
	} catch(std::runtime_error &e){
		lpp::logger::cout(lpp::CRITICAL, e.what());
	}
	return 0;
}
