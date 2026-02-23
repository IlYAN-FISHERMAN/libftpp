/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:56:32 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 19:00:02 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "worker_pool.hh"

WorkerPool::WorkerPool(size_t){}

void WorkerPool::addJob(const std::function<void()>&jobToExecute){}

void WorkerPool::removeTask(const std::string& name){}
