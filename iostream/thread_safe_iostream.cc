/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:46:00 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 14:11:19 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hh"

std::string& threadSafeCout::prefix() {
	thread_local std::string p;
	return p;
}

static inline std::mutex mutex;

std::stringstream&  threadSafeCout::msg() {
	thread_local std::stringstream b;
	return b;
}
