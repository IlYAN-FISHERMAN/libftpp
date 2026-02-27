/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:17:40 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 18:55:06 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hh"

Message::Message(int type) : _type(static_cast<Type>(type)){}

Message::Message() : _type(INT){}

Message::Type Message::type() const { return _type; }
