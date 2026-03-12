/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsability_chain.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 12:29:51 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:19:58 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "responsability_chain.hh"

void lpp::responsability_chain::setNext(std::shared_ptr<lpp::IResponsability_chain> next){
	_chain.push_back(next);
}

lpp::responsability_chain::responsability_chain(){}

lpp::responsability_chain::~responsability_chain(){ _chain.clear();}

void lpp::responsability_chain::run(){
	for (auto &it : _chain){
		if (!it->config()){
			lpp::cout << "config error" << std::endl;
			return ;
		}
	}

	for (auto &node : _chain){
		if (!node.get()->execute()){
			lpp::cout << "execute error" << std::endl;
			return;
		}
	}
}
