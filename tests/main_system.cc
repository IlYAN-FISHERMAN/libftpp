/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_system.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:51:12 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/20 20:29:15 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hh"

int testSystem(int ac, char **av){
	try{
		lpp::system sys;
		for (int i = 1; i < ac ; i++)
			sys.add_exec(av[i]);
		for (int i = 1; i < ac ; i++){
			auto rps = sys.get_exec();
			if (!rps.has_value())
				return 0;
			lpp::cout << *rps << std::flush;
		}
	}catch(std::system_error &e){
		lpp::cout << e.what() << std::endl;
	}

	return 0;
}
