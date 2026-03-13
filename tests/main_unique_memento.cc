/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_unique_memento.cc                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 09:48:59 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 10:02:46 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testUniqueMemento(){
	lpp::ivector3 v1(1, 2, 3);
	lpp::ivector3 v2(4, 5, 1);
	
	{
		lpp::cout << v2 << std::endl;
		lpp::unique_memento tmp(v2, v1 + v2);
		lpp::cout << v2 << std::endl;
	}
	lpp::cout << v2 << std::endl;
	return 0;
}
