/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_responsability_chain.cc                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 12:34:22 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:26:21 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testResponsabilityChain(){
	lpp::responsability_chain chain;
	chain.setNext(std::make_shared<lpp::server>());
	auto c = std::make_shared<lpp::client>();
	chain.setNext(c);
	chain.setNext(std::make_shared<lpp::prompt>(*c.get()));

	try{
		do{
			chain.run();
			std::string retry;
			lpp::cout.prompt("run again?[y]: ", retry);
			if (retry == "Y" || retry == "y")
				chain.run();
			else 
				break;
		}
		while (true);
	}catch (std::runtime_error &e){
		lpp::cout << e.what() << std::endl;
	}

	return 0;
}
