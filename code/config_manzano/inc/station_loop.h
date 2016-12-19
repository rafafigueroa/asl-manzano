/*
 * network_loop.h
 *
 *  Created on: Nov 23, 2016
 *      Author: akearns
 */

#ifndef CODE_CONFIG_MANZANO_INC_NETWORK_LOOP_H_
#define CODE_CONFIG_MANZANO_INC_NETWORK_LOOP_H_

#include <vector>
#include "component_loop.h"

namespace mzn {

class StationLoop : public ComponentLoop {

// this class looks at the network level, to add and select stations

public:

    explicit
    StationLoop(BasicJson & parent) : ComponentLoop{parent} {}

    // parent is referenced by active_ object, which is what gets modified
    void add( ); // TODO: determine correct parameters

    // TODO: need add station and add processor functions

    bool remove(std::string const & index);

    // editing of existing subcomponent
    // should pass back a station_loop object
    // stations are accessed in the json by name, so index should be a string
    void modify(BasicJson const & json_sn,
            std::string const & index); // TODO: return type StationLoop

    // lists the stations available
    void list_components() const override;


protected:

    std::string index_prompt() const;

    // calls specific modifier functions based on user's input
    void dispatch(BasicJson const & json_sn, char const c) override;

    void show_options() const override;

    char collect_input() const override;

    void modify_prompt(BasicJson const & json_sn);

    // used to insert component into the parent JSON (input prompts)
    void add_prompt();

    // used to remove the current element from the json structure
    void remove_prompt();

};

} /* namespace mzn */

#endif /* CODE_CONFIG_MANZANO_INC_NETWORK_LOOP_H_ */
