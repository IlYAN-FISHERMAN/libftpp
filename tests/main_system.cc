/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_system.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:51:12 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/20 18:30:31 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hh"

int testSystem(int ac, char **av){
	try{
		for (int i = 1; i < ac ; i++)
			lpp::system->add_exec(av[i]);
	}catch(std::runtime_error &e){
		lpp::cout << e.what() << std::endl;
	}

	for (int i = 1; i < ac ; i++){
		auto rps = lpp::system->get_exec();
		if (!rps.has_value())
			return 0;
		lpp::cout << *rps << std::endl;
	}

	return 0;
}
