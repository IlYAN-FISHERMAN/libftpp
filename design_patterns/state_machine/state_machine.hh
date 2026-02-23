/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.hh                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 10:24:57 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/23 17:32:01 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <functional>
#include <map>

template<typename TState>
class StateMachine{
	private :

			class Transition{
				protected :
					friend StateMachine;
					TState start;
					TState final;
					std::function<void()> __f;

				public :
					Transition(TState s, TState f, const std::function<void()>& lambda) : start(s), final(f), __f(lambda){}
					Transition(TState s, TState f) : start(s), final(f){}
					bool operator==(const Transition& other){
						return (start == other.start && final == other.final);
					}

					void operator()(void){
						__f();
					}
			};

		std::unordered_map<TState, const std::function<void()>>		_states;
		std::vector<Transition>										_transition;
		std::vector<TState>											_existingStates;
		std::vector<TState>::iterator								_index;
	public :
		StateMachine() : _index(_existingStates.begin()){}
		~StateMachine(){}
		StateMachine(const StateMachine&){}
		StateMachine& operator=(const StateMachine&){}

		void addState(const TState& state) noexcept(false){
			if (std::find(_existingStates.begin(), _existingStates.end(), state) != _existingStates.end())
				throw std::invalid_argument("State already added");
			_existingStates.push_back(state);
			_index = _existingStates.begin();
			_states.insert({state, []{}});
		}

		void addAction(const TState& state, const std::function<void()>& lambda) noexcept(false){
			if (std::find(_existingStates.begin(), _existingStates.end(), state) == _existingStates.end())
				throw std::invalid_argument("Handle state not found");
			if (_states.find(state) != _states.end())
				_states.erase(_states.find(state));
			_states.insert({state, lambda});
		}

		void addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda) noexcept(false){
			if (std::find(_existingStates.begin(), _existingStates.end(), startState) == _existingStates.end()
				|| std::find(_existingStates.begin(), _existingStates.end(), finalState) == _existingStates.end())
					throw std::invalid_argument("Handle state not found");
			Transition tmp(startState, finalState, lambda);
			auto it = std::find(_transition.begin(), _transition.end(), tmp);
			if (it != _transition.end())
				_transition.erase(it);
			_transition.push_back(Transition(startState, finalState, lambda));
		}

		void transitionTo(const TState& state) noexcept(false){
			if (std::find(_existingStates.begin(), _existingStates.end(), state) == _existingStates.end()
	   		|| std::find(_existingStates.begin(), _existingStates.end(), state) == _existingStates.end())
				throw std::invalid_argument("Handle state not found");

			if (std::find(_transition.begin(), _transition.end(), Transition(*_index, state)) == _transition.end()){
				if ((*_index.base()) != state)
					addTransition((*_index.base()), state, []{});
				throw std::invalid_argument("Handle transition not found");
			}
			auto it = std::find(_transition.begin(), _transition.end(), Transition(*_index, state));
			(*it.base())();
			_index = std::find(_existingStates.begin(), _existingStates.end(), (*it.base()).final);
		}

		void update(){
			if (_existingStates.size() <= 0)
				throw std::invalid_argument("Handle state not found");
			_states.at(*_index.base())();
	}
};
