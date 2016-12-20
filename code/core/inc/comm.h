// ** Comm Object ** //
// Manzano software
#ifndef _MZN_COMM_H
#define _MZN_COMM_H

#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <random>

#include "cmd_file_reader.h"
#include "msg_task_manager.h"

#include "output_store.h"
#include "input_store.h"
#include "stream_output.h"
#include "message_dispatch.h"

// external libraries
#include "md5.h" // jason holland's (usgs) md5 library
#include "date.h" // hinnant's date library

#include "mzn_cmake_config.h"

namespace mzn {


//! Commands are setup and send/recv from this class
/*!
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class Comm {

public:
    //! autocal scheduler takes a ref to md_
    Comm();
    ~Comm() = default;

public:
    //! holds config, status and connections to hardware targets
    SeismicNetwork sn;

    //! takes cmd and send/recv a msg
    MessageDispatch md;

    //! holds the outputs of the send/recv commands
    OutputStore output_store;

    //! has functions to setup input commands
    /*! either creating an acceptable default, using a "shortcut option"
        or using a "live option" which takes a user created command
        @throws if an option is provided and not found in the shortcuts
        or in the custom made commands
        @see InputStore for more info on default/shortcut/live options
      */
    InputStore input_store;

    //! streams output to console
    StreamOutput stream_output;


private:

    //! manages multi-threaded msg task that are waiting to be sent
    MsgTaskManager msg_task_manager_;

    //! uses threads to setup timed send of qcal cmds
    CmdFileReader cmd_file_reader_;

public:
    //! a template specialization for each legal combination is provided
    // can use typename std::enable_if<action != Action::get>::type
    // --------------------------------------------------------------------- //
    template<Action action, Kind kind>
    void run(UserInstruction const & ui, TargetAddress const & ta) {

        std::stringstream ss;
        ss << "Comm::run general case is not defined"
           << "\nAction: " << action << " Kind: " << kind;
        throw std::logic_error{ss.str()};
    }

private:
    //! for use with a q. send a requests, gets a response
    //! @see Digitizer (q)
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    inline
    void q_send_recv(UserInstruction const & ui,
                     TargetAddress const & ta) {

        // send and receive commands
        auto cmd_input =
            input_store.get_input_cmd<action, kind>(ui, ta);

        // empty (default constructed) cmd for receiving
        using Co = typename Co<action, kind>::type;
        Co cmd_output{};

        auto & q = sn.q_ref(ta);

        md.send_recv(q.port_config, cmd_input, cmd_output, true);

        auto constexpr ui_id = UserInstruction::hash(action, kind);
        auto const task_id = ta.hash() + ui_id;

        // Co needs to be move_constructible
        output_store.cmd_map[task_id] =
            std::make_unique<Co>( std::move(cmd_output) );
    }

    /*
    // --------------------------------------------------------------------- //
    template <class Cs, Action action, Kind kind>
    inline
    void cmd_show_options() {

        stream_output.os << "\nshortcut options:";
        stream_output.show_tree( InstructionMap::filter_options<kind>() );

        auto const live_options = input_store.live_options<action, kind>();

        stream_output.os << "\n\nlive options:";
        stream_output.show_num_tree(live_options);

        //TODO: change when the up arrow is added in the user input loop
        unsigned int live_option_index;
        std::string live_option_token;

        // only std::cin makes sense here, no need for indirection
        try {
            std::getline(std::cin, live_option_token);
            live_option_index = std::stoi(live_option_token);

        } catch(std::exception & e) {
            throw WarningException( "Comm",
                                    "cmd_show_options",
                                    "Bad selection provided\n" +
                                    std::string( e.what() ) );
        }

        // user selected "cancel"
        if (live_option_index == live_options.size() + 1) {
            stream_output.os << "\n cancelled";
            return;
        }

        if ( live_option_index == live_options.size() ) {
            stream_output.os << "\n option name: ";
            std::string option;
            std::getline(std::cin, option);
            std::cout << std::endl << "option chosen " << option << std::endl;
            input_store.add_live_cmd<Cs>(option);
            return;
        }

        // user selected an existing live command
    }

    */

    /*
    //! to autocal_scheduler_.autocal_start
    void autocal_start(Digitizer & q, Sensor const & s);
    */
};

} // end namespace
#endif // _MZN_COMM_H_
