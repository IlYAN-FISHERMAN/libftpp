/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hh                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:55:14 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/19 20:46:15 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Memento{
		public :
			class Snapshot{
			};
	protected :
		virtual void _saveToSnapshot(Memento::Snapshot &) const;
		virtual void _loadFromSnapshot(Memento::Snapshot &);

	public :
		void load(const Memento::Snapshot& state);
		Snapshot save();
};
