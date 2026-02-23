/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hh                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:44:45 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 16:48:20 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

template<typename TType>
class ThreadSafeQueue{
	private :
	public :
		ThreadSafeQueue();
		~ThreadSafeQueue(){}
		ThreadSafeQueue(const ThreadSafeQueue&);
		ThreadSafeQueue& operator=(const ThreadSafeQueue &);
};
