/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:00 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/21 21:19:24 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hh"

void threadSafeCout::setPrefix(const std::string& prefix){
	std::lock_guard<std::mutex> lock(_mutex);
	_prefix = prefix;
}
