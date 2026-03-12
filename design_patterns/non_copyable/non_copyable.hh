/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_copyable.hh                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:36:27 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:37:16 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace lpp{
	class NonCopyable{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;

	public:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}
