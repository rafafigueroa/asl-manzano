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
#include "stream_plotter.h"
#include "message_dispatch.h"
#include "string_utilities.h"
#include "system_calls.h"

// external libraries
#include "md5.h" // jason holland's (usgs) md5 library
#include "date.h" // hinnant's date library

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

    //! holds this computer ip as seen by digitizer registrations
    std::atomic<uint32_t> ip_address_number;

private:

    //! manages multi-threaded msg task that are waiting to be sent
    MsgTaskManager msg_task_manager_;

    //! uses threads to setup timed send of qcal cmds
    CmdFileReader cmd_file_reader_;

public:

    //! primary template
    //! a template specialization for each legal combination is provided
    //! can use typename std::enable_if<action != Action::get>::type
    // --------------------------------------------------------------------- //
    template <Action action, Kind kind>
    void run(TargetAddress const & ta,
             OptionInput const & oi = OptionInput{}) {

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
    void q_send_recv(TargetAddress const & ta,
                     OptionInput const & oi = OptionInput{}) {

        // send and receive commands
        auto cmd_input = input_store.get_input_cmd<action, kind>(ta, oi);

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


    // --------------------------------------------------------------------- //
    bool q_is_reg(Digitizer & q) {
        C2Regresp cmd_regresp;
        return q_is_reg(q, cmd_regresp);
    }

    // --------------------------------------------------------------------- //
    bool q_is_reg(Digitizer & q, C2Regresp & cmd_regresp) {

        if (ip_address_number == 0) {
            cmd_regresp.registered(false);
            return false;
        }

        C2Regchk cmd_regchk;
        cmd_regchk.ip_address(ip_address_number);

        // turns out you can't ask if registered after de-registering
        // maybe because it had a specific purpose related to the balers
        // it should be used internally sparingly, in auto_ instructions only
        // when it doesn't work, the digitizer sends a C1Cerr message which
        // provides the same function, although using error handling

        try {

            md.send_recv(q.port_config, cmd_regchk, cmd_regresp, false);

        } catch (WarningException const & e) {

            std::string const e_msg = e.what();

            if (e_msg.find("you_are_not_registered") != std::string::npos) {
                // C1Cerr with you_are_not_registered code
                cmd_regresp.registered(false);
                q.port_config.registered = false;
                return false;

            } else throw e;
        }

        auto const reg_status = cmd_regresp.registered();
        q.port_config.registered = reg_status;
        return reg_status;
    }

    // --------------------------------------------------------------------- //
    uint32_t q_current_seq_number(Digitizer & q) {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.global_status(true);
        // status
        C1Stat cmd_stat; // Status

        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        CxGlobalStatus * gs =
            dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

        if (gs == nullptr) throw FatalException("Comm",
                                                "q_current_seq_number",
                                                "gs nullptr");

        return gs -> current_data_sequence_number();
    }

    //! These two functions can be generalized for any of the 14 output
    //! definitions. cal/center are currently used. In general, more than
    //! one output can be set in the sce, but this is not needed or tested.
    // --------------------------------------------------------------------- //
    BmSensorControlEnable sensor_control_cal(Digitizer & q,
                                             Sensor const & s) {
        // sensor_control_map, C1Rqsc , C1Sc
        C1Rqsc cmd_rqsc;
        C1Sc cmd_sc;
        md.send_recv(q.port_config, cmd_rqsc, cmd_sc, false);

        auto const sco = (s.config.input == Sensor::Input::a) ?
                         BmSensorControlOutput::Line::sensor_a_calibration :
                         BmSensorControlOutput::Line::sensor_b_calibration;

        BmSensorControlEnable sce;

        if (cmd_sc.sensor_output_1.line() == sco) sce.output_1(true); else
        if (cmd_sc.sensor_output_2.line() == sco) sce.output_2(true); else
        if (cmd_sc.sensor_output_3.line() == sco) sce.output_3(true); else
        if (cmd_sc.sensor_output_4.line() == sco) sce.output_4(true); else
        if (cmd_sc.sensor_output_5.line() == sco) sce.output_5(true); else
        if (cmd_sc.sensor_output_6.line() == sco) sce.output_6(true); else
        if (cmd_sc.sensor_output_7.line() == sco) sce.output_7(true); else
        if (cmd_sc.sensor_output_8.line() == sco) sce.output_8(true); else
        throw WarningException("Comm",
                               "sensor_control_cal",
                               "No sensor control configured for calibration");

        return sce;
    }

    // --------------------------------------------------------------------- //
    BmSensorControlEnable sensor_control_center(Digitizer & q,
                                                Sensor const & s) {
        // sensor_control_map, C1Rqsc , C1Sc
        C1Rqsc cmd_rqsc;
        C1Sc cmd_sc;
        md.send_recv(q.port_config, cmd_rqsc, cmd_sc, false);

        auto const sco = (s.config.input == Sensor::Input::a) ?
                         BmSensorControlOutput::Line::sensor_a_centering :
                         BmSensorControlOutput::Line::sensor_b_centering;

        BmSensorControlEnable sce;

        if (cmd_sc.sensor_output_1.line() == sco) sce.output_1(true); else
        if (cmd_sc.sensor_output_2.line() == sco) sce.output_2(true); else
        if (cmd_sc.sensor_output_3.line() == sco) sce.output_3(true); else
        if (cmd_sc.sensor_output_4.line() == sco) sce.output_4(true); else
        if (cmd_sc.sensor_output_5.line() == sco) sce.output_5(true); else
        if (cmd_sc.sensor_output_6.line() == sco) sce.output_6(true); else
        if (cmd_sc.sensor_output_7.line() == sco) sce.output_7(true); else
        if (cmd_sc.sensor_output_8.line() == sco) sce.output_8(true); else
        throw WarningException("Comm",
                               "sensor_control_center",
                               "No sensor control configured for centering");

        return sce;
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
