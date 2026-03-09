/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:17:40 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/09 10:53:38 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hh"

lpp::message::message(int type) : _type(static_cast<Type>(type)){}

lpp::message::message() : _type(0){}

lpp::message::Type lpp::message::type() const { return _type; }
