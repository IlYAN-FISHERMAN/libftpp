/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_state_machine.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:50:55 by ilyanar           #+#    #+#             */
/*   Updated: 2026/02/24 15:52:38 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

enum class State {
    Idle,
    Running,
    Paused,
    Stopped
};

int myTestStateMachine(){
    StateMachine<State> sm;

    sm.addState(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Paused);
    sm.addState(State::Stopped);

    sm.addAction(State::Idle, [] { std::cout << "System is idle." << std::endl; });
    sm.addAction(State::Running, [] { std::cout << "System is running." << std::endl; });
    sm.addAction(State::Paused, [] { std::cout << "System is paused." << std::endl; });
    // sm.addAction(State::Stopped, [] { std::cout << "System is stopped." << std::endl; });
    // No addAction for State::Stopped, it will use the default empty lambda

    sm.addTransition(State::Idle, State::Running, [] { std::cout << "Transitioning from Idle to Running." << std::endl; });
    sm.addTransition(State::Running, State::Paused, [] { std::cout << "Transitioning from Running to Paused." << std::endl; });
    sm.addTransition(State::Paused, State::Running, [] { std::cout << "Transitioning from Paused to Running." << std::endl; });

    sm.addTransition(State::Idle, State::Stopped, [] { std::cout << "Transitioning from Idle to Stopped." << std::endl; });
    sm.addTransition(State::Running, State::Stopped, [] { std::cout << "Transitioning from Running to Stopped." << std::endl; });
    // sm.addTransition(State::Paused, State::Stopped, [] { std::cout << "Transitioning from Paused to Stopped." << std::endl; });
    // No addTransition for State::Stopped

    sm.update();  // Should print: "System is idle."
    sm.transitionTo(State::Running);  // Should print: "Transitioning from Idle to Running."
    sm.update();  // Should print: "System is running."
    sm.transitionTo(State::Paused);  // Should print: "Transitioning from Running to Paused."
    sm.update();  // Should print: "System is paused."

    // Transitioning to and from the new State::Stopped
    try {
        sm.transitionTo(State::Stopped);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught1: " << e.what() << std::endl;  // Handle state not found
    }

    sm.addTransition(State::Paused, State::Stopped, [] { std::cout << "Transitioning from Paused to Stopped." << std::endl; });

    try {
    	sm.transitionTo(State::Running);  // Should not print anything, default empty lambda is executed
    	sm.transitionTo(State::Stopped);  // Should not print anything, default empty lambda is executed
		sm.addTransition(State::Stopped, State::Running, [] { std::cout << "Transitioning from Stopped to Running." << std::endl; });
    	sm.transitionTo(State::Running);  // Should not print anything, default empty lambda is executed
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught2: " << e.what() << std::endl;  // Handle state not found
    }

    try {
        sm.transitionTo(State::Running);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught3: " << e.what() << std::endl;  // Handle state not found
    }

    sm.update();  // Should print: "System is paused."

    return 0;
}

int testStateMachine() {
    StateMachine<State> sm;

    sm.addState(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Paused);
    sm.addState(State::Stopped);

    sm.addAction(State::Idle, [] { std::cout << "System is idle." << std::endl; });
    sm.addAction(State::Running, [] { std::cout << "System is running." << std::endl; });
    sm.addAction(State::Paused, [] { std::cout << "System is paused." << std::endl; });
    // No addAction for State::Stopped, it will use the default empty lambda

    sm.addTransition(State::Idle, State::Running, [] { std::cout << "Transitioning from Idle to Running." << std::endl; });
    sm.addTransition(State::Running, State::Paused, [] { std::cout << "Transitioning from Running to Paused." << std::endl; });
    sm.addTransition(State::Paused, State::Running, [] { std::cout << "Transitioning from Paused to Running." << std::endl; });
    // No addTransition for State::Stopped

    sm.update();  // Should print: "System is idle."
    sm.transitionTo(State::Running);  // Should print: "Transitioning from Idle to Running."
    sm.update();  // Should print: "System is running."
    sm.transitionTo(State::Paused);  // Should print: "Transitioning from Running to Paused."
    sm.update();  // Should print: "System is paused."

    // Transitioning to and from the new State::Stopped
    try {
        sm.transitionTo(State::Stopped);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }

    try {
    	sm.transitionTo(State::Stopped);  // Should not print anything, default empty lambda is executed
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }

    try {
        sm.transitionTo(State::Running);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }

    return 0;
}

