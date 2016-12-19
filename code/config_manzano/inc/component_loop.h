/*
 * component_loop.h
 *
 *  Created on: Nov 18, 2016
 *      Author: akearns
 */

#ifndef CODE_CONFIG_MANZANO_INC_COMPONENT_LOOP_H_
#define CODE_CONFIG_MANZANO_INC_COMPONENT_LOOP_H_

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include "json.hpp"
#include "seismic_network.h"
#include "mzn_except.h"

namespace mzn {

class ComponentLoop {

	// provides interface for basic overview of each network component
	// show the current config, valid options, and then process user decision

public:

    using BasicJson = nlohmann::json;

	explicit
	ComponentLoop(BasicJson & parent);
	~ComponentLoop() = default;

	// template pattern for starting up the input loop
	// (as in design pattern, not literal c++ template function)
	char initializer_prompt(BasicJson const & json_sn);

	virtual
	void list_components() const = 0;

	std::vector<std::string> split(std::string const & s,
	                               char const delim) const;

    // TODO: possibly replace these chars with an enum for return values?

	// all methods will use the same command character to go to main menu
	static
	char const RETURN = 'r';
	// .. and to go one level up
	static
	char const PREV = 'p';

	// SUCCESS is returned if a command is successfully callable
	static
	char const SUCCESS = 'a';
	// FAIL if not a return character or success
	static
	char const FAIL = 'n';

protected:

	BasicJson & parent_; // reference to active json sub-object
	TargetAddress ta_; // TODO: set and use parameter

    // used to access private functions through char parameter
    // (derived from user input, but also makes automated testing easier)
    virtual
    void dispatch(BasicJson const & json_sn, char const c) = 0;

	// used to show the valid options that the user can make
	virtual
	void show_options() const = 0;
	// used to handle the user input and call relevant functions
	virtual
	char collect_input() const = 0;
};

} // namespace

#endif /* CODE_CONFIG_MANZANO_INC_COMPONENT_LOOP_H_ */
