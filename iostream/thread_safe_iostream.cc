/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:00 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 14:32:48 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hh"

thread_local std::stringstream threadSafeCout::_msg;
thread_local std::string threadSafeCout::_prefix;

std::string threadSafeCout::prefix() const{return _prefix;}

void threadSafeCout::setPrefix(const std::string &str){_prefix = str;}
