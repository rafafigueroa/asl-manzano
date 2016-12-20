// Manzano software
#ifndef _MZN_UI_TO_CMD_TYPE_H_
#define _MZN_UI_TO_CMD_TYPE_H_

#include <map>
#include <memory>
#include "commands.h"
#include "user_instruction.h"

namespace mzn {

//! Provides the input/output type (Ci::type, Co::type) from Action/Kind
/*! used in the input_store, output_store and others.

    for example:
    template <Action action, Kind kind,
              class Ci = typename Ci<action, kind>::type >
    Ci do_something_and_return_specific_cmd();

    It will fail at compile time if there is not a specific
    template specialization for that Action/Kind combination.

    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <Action action, Kind kind> struct Ci { using type = void; };
// -------------------------------------------------------------------------- //
template <Action action, Kind kind> struct Co { using type = void; };

// -------------------------------------------------------------------------- //
template <> struct Ci<Action::get, Kind::ping>    {using type = C1Ping4;};
template <> struct Ci<Action::get, Kind::poll>    {using type = C1Pollsn;};
template <> struct Ci<Action::get, Kind::global>  {using type = C1Rqglob;};
template <> struct Ci<Action::get, Kind::center>  {using type = C2Rqamass;};
template <> struct Ci<Action::get, Kind::ctrl>    {using type = C1Rqsc;};
template <> struct Ci<Action::get, Kind::dev>     {using type = C1Rqdev;};
template <> struct Ci<Action::get, Kind::stat>    {using type = C1Rqstat;};
template <> struct Ci<Action::set, Kind::ctrl>    {using type = C1Ctrl;};
template <> struct Ci<Action::start, Kind::cal>   {using type = C1Qcal;};

// -------------------------------------------------------------------------- //
template <> struct Co<Action::get, Kind::ping>    {using type = C1Ping5;};
template <> struct Co<Action::get, Kind::poll>    {using type = C1Mysn;};
template <> struct Co<Action::get, Kind::global>    {using type = C1Glob;};
template <> struct Co<Action::get, Kind::center>  {using type = C2Amass;};
template <> struct Co<Action::get, Kind::ctrl>    {using type = C1Sc;};
template <> struct Co<Action::get, Kind::dev>     {using type = C1Dev;};
template <> struct Co<Action::get, Kind::stat>    {using type = C1Stat;};
template <> struct Co<Action::set, Kind::ctrl>    {using type = C1Cack;};
template <> struct Co<Action::set, Kind::reg>     {using type = C1Cack;};
template <> struct Co<Action::set, Kind::dereg>   {using type = C1Cack;};
template <> struct Co<Action::stop, Kind::cal>    {using type = C1Cack;};
template <> struct Co<Action::start, Kind::cal>   {using type = C1Cack;};

} // end namespace

#endif // _MZN_UI_TO_CMD_TYPE_H_
