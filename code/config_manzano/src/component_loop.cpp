/*
 * component_loop.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: akearns
 */

#include "component_loop.h"

namespace mzn{

ComponentLoop::ComponentLoop(BasicJson & parent) : parent_{parent} {}

// defines the logic flow for the interface of a component modifier
char ComponentLoop::initializer_prompt(BasicJson const & json_sn) {

	while (true) {
	    if (not parent_.empty()){
	        list_components();
	    }
		show_options();

		char c;
		// collect_input will take in input and call commands as necessary
		c = collect_input();
		if (c == RETURN) {
		    return c; // keeps passing return (goes to root)
		} else if (c == PREV) {
		    return SUCCESS; // only go up one level (parent eats success char)
		} else if (c == FAIL) {
		    std::cout << "That choice was not valid. Select a different choice.";
		    std::cout << std::endl << std::endl;
		    std::cin.clear();
		} else {
		    dispatch(json_sn, c);
		    return SUCCESS;
		}
	}

}

// used to tokenize certain inputs in the network
std::vector<std::string> ComponentLoop::split(std::string const & s,
                                              char const delim) const {
    std::vector<std::string> tokens;

    std::stringstream ss;
    ss.str(s);
    std::string token;
    while ( std::getline(ss, token, delim) ) {
        tokens.push_back(token);
    }

    return tokens;
}

} // <- mzn
