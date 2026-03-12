/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 16:30:47 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/12 17:32:20 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt.hh"

lpp::prompt::prompt(lpp::client &client) : _client(client), _prefix("[libftpp prompt]-> "){}

lpp::prompt::~prompt(){}

void lpp::prompt::run(){
	
	if (!_client.isRunning())
		throw std::logic_error("client not running");

	lpp::cout << "Available operations :" << std::endl;
	lpp::cout << " - [Q]uit|exit : close the program" << std::endl;
	lpp::cout << " - Any other input to continue updating the _client" << std::endl;
	lpp::cout << "Usage - (code)|(input), (string)" << std::endl << std::endl;
	lpp::cout.setPrefix("[libftpp prompt]: ");

	while (true)
	{
		std::string input;
		lpp::cout.prompt(_prefix, input, true);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input == "quit" || input == "q" || input == "exit")
			break;
		else if(input == "clear" || input == "c"){
			lpp::cout << "\033c" << std::flush;
		} else if (!input.empty()){
			if (input.find('|') == input.npos){
				_client.send(input);
			} else {
				int code = 0;
				char sep = 0;
				std::stringstream ss(input);
				input.clear();
				if (ss >> code >> sep >> input){
					if (sep != '|' || !ss.eof()){
						lpp::cout << "Argument error" << std::endl;
						lpp::cout << "error with: " << input << std::endl;
						continue;
					}
					lpp::message msg(code);
					msg << input;
					_client.send(msg);
				} else {
					lpp::cout << "Argument error" << std::endl;
					continue;
				}
			}
		}
	}

	lpp::cout.clear();
}

bool lpp::prompt::execute(){
	run();
	return true;
}

bool lpp::prompt::config(){return true;}

void lpp::prompt::setPrefix(std::string prefix){_prefix = prefix;}
