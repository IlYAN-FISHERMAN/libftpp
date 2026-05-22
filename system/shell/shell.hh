/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.hh                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:45:55 by ilyanar           #+#    #+#             */
/*   Updated: 2026/05/20 20:24:23 by ilyanar          ###   LAUSANNE.ch       */
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
	class system{
		private:
			class shell : public lpp::worker_pool::IJobs, public lpp::NonCopyable{
				private:
					friend system;
					lpp::worker_pool _worker;
					lpp::thread_safe_queue<std::string> _in;
					lpp::thread_safe_queue<std::string> _out;
					mutable std::mutex _mutex;

					virtual void execute() override;
					void cmd();
				public:
					shell();
					~shell();
			};
			std::shared_ptr<shell> _shell;
		public:
			system();
			~system();

			void add_exec(const std::string &);
			std::optional<std::string> get_exec();
			static std::string cmd(const std::string &);
	};
}
