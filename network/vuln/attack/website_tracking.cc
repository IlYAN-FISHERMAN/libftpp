/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   website_tracking.cc                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:14:54 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/24 23:53:56 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hh"

int main(int ac, char **av){
	lpp::nmap map;
	std::vector<std::string> ips;

	try {
		if (ac > 2){
			for (int i = 1; i < ac; i++)
				ips.emplace_back(av[i]);

			map.setAsync(true);
			// map.setOptions("-F");
			auto response = map.sniff(ips);

			for (auto &it : response){
				lpp::logger::cout(lpp::WARNING, "[NMAP RESULT]\n");
				lpp::cout << it << std::endl;
			}

		} else if (ac == 2){
			while(true){
				lpp::logger::cout(lpp::WARNING, "[NMAP RESULT]\n");
				auto ports = map.ports(map.sniff(av[1]));
				lpp::cout << "test\n";
				for (auto &it : ports){
					lpp::cout << "found port: " << it << std::endl;
					if (it > 1024 && it < 65535){
						auto target = std::string("open http://") + av[1] + ":" + std::to_string(it);
						lpp::logger::cout(lpp::INFO, "executing: " + target);
						lpp::system::exec(target);
					}
				}

				lpp::logger::cout(lpp::CRITICAL, "[END]");
				std::string input;
				std::getline(std::cin, input);
			}
		}
	} catch(std::runtime_error &e){
		lpp::cout << e.what() << std::endl;
	}
	return 0;
}
