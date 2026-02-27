/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hh                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:16:41 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/27 19:21:33 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <functional>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include <cstddef>
#include <netinet/in.h>
#include <poll.h>
#include <csignal>
#include <string.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <unordered_map>

#include "message/message.hh"
#include "client/client.hh"
#include "server/server.hh"
