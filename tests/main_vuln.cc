/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_vuln.cc                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:14:54 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/24 21:39:27 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hh"

int testVuln(int ac, char **av){
	lpp::nmap map;
	std::vector<std::string> ips;

	for (int i = 1; i < ac; i++)
		ips.emplace_back(av[i]);

	map.setAsync(true);
	// map.setOptions("-F");

	auto response = map.sniff(ips);

	for (auto &it : response){
		lpp::logger::cout(lpp::WARNING, "[NMAP RESULT]");
		lpp::cout << it << std::endl;
	}

	return 0;
}
