/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.hh                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:45:55 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/20 18:27:08 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <array>
#include <memory>
#include <stdio.h>
#include "threading/worker_pool/worker_pool.hh"
#include <optional>

namespace lpp{
	class shell : public lpp::worker_pool::IJobs, public lpp::NonCopyable{
		private:

			lpp::worker_pool _worker;
			lpp::thread_safe_queue<std::string> _in;
			lpp::thread_safe_queue<std::string> _out;
			mutable std::mutex _mutex;

			virtual void execute() override;

			void cmd();
		public:
			shell();
			~shell();
			void add_exec(const std::string &);
			std::optional<std::string> get_exec();
	};

	inline std::shared_ptr<lpp::shell> system = std::make_shared<lpp::shell>();
}
