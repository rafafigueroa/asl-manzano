// ** Comm Object ** //
// Manzano software
#include "comm.h"
namespace mzn {

// -------------------------------------------------------------------------- //
inline
std::chrono::time_point<Time::SysClock, Time::Seconds<> >
q_time(TargetAddress const & ta) {

    /*
    // request status
    C1Rqstat cmd_rqstat;
    cmd_rqstat.request_bitmap.global_status(true);
    // status
    C1Stat cmd_stat; // Status

    md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

    CxGlobalStatus * gs =
        dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

    if (gs == nullptr) throw FatalException("Comm", "autocal", "gs nullptr");

    // Q330 manual: "Seconds offset ... when added to a data sequence number
    // is seconds since January 1 2000"
    auto const q_data_seq_number = gs -> data_sequence_number.data();
    auto const q_seconds_offset = gs -> seconds_offset.data();
    auto const q_sec_since_epoch = q_data_seq_number + q_seconds_offset;

    CmdFieldTime<uint32_t, Time::k_shift_seconds_1970_2000> q_now_time;
    q_now_time.data(q_sec_since_epoch);

    std::cout << std::endl << "q_now_seq_num: " << q_sec_since_epoch;
    std::cout << std::endl << "q_now_time: " << q_now_time;
    std::cout << std::endl << " ### now: "
                      << Time::sys_time_of_day() << " ###\n";

    return q_now_time();
    */

    return std::chrono::time_point< Time::SysClock, Time::Seconds<> >{};
}

// -------------------------------------------------------------------------- //
Comm::Comm() : sn{},
               md{},
               output_store{},
               input_store{sn},
               stream_output{sn},
               ip_address_number{0},
               msg_task_manager_{sn, md},
               cmd_file_reader_{sn} {}

// -------------------------------------------------------------------------- //
using TA = TargetAddress;
using OI = OptionInput;

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::poll>(TA const & ta, OI const & oi) {
    // C1Pollsn, C1Mysn
    q_send_recv<Action::get, Kind::poll>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::ping>(TA const & ta, OI const & oi) {
    // C1Ping4, C1Ping5
    q_send_recv<Action::get, Kind::ping>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::global>(TA const & ta, OI const & oi) {
    // C1Rqglob, C1glob
    q_send_recv<Action::get, Kind::global>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::stat>(TA const & ta,  OI const & oi) {
    // C1Rqstat, C1Stat (multi command)
    q_send_recv<Action::get, Kind::stat>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::center>(TA const & ta, OI const & oi) {
    // C2Rqamass, C2Amass
    q_send_recv<Action::get, Kind::center>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::output>(TA const & ta, OI const & oi) {
    // sensor_control_map, C1Rqsc , C1Sc
    q_send_recv<Action::get, Kind::output>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::dev>(TA const & ta, OI const & oi) {
    // devices, C1Rqdev, C1Dev (multicommand)
    q_send_recv<Action::get, Kind::dev>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::stop, Kind::cal>(TA const & ta, OI const & oi) {

    // C1Stop, C1Cack
    q_send_recv<Action::stop, Kind::cal>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::reg>(TA const & ta, OI const & oi) {

    // get current reg status from q, C2Regchk, C2Regresp
    auto & q = sn.q_ref(ta);

    C2Regresp cmd_regresp;

    q_is_reg(q, cmd_regresp);

    std::cout << cmd_regresp;

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::get, Kind::reg);
    auto const task_id = ta.hash() + ui_id;

    // move to cmd store
    output_store.cmd_map[task_id] =
        std::make_unique<C2Regresp>( std::move(cmd_regresp) );
}

// *** CALIBRATIONS *** //
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::start, Kind::cal>(TA const & ta, OI const & oi) {

    auto constexpr action = Action::start;
    auto constexpr kind = Kind::cal;

    auto & q = sn.q_ref(ta);
    auto & s = sn.s_ref(ta);

    // --------- E300 registration ----------- //
    if (s.config.has_e300) {
        s.port_e300_ref().reg();
        std::cout << std::endl << "\n** E300 REGISTERED! ** " << std::endl;
        s.port_e300_ref().cal_connect();
    }

    // send and receive commands
    auto cmd_input = input_store.get_input_cmd<action, kind>(ta, oi);

    auto const sce = sensor_control_cal(q, s);
    cmd_input.sensor_control_enable = sce;

    // empty (default constructed) cmd for receiving
    using Co = typename Co<action, kind>::type;
    Co cmd_output{};

    md.send_recv(q.port_config, cmd_input, cmd_output, true);

    auto constexpr ui_id = UserInstruction::hash(action, kind);
    auto const task_id = ta.hash() + ui_id;

    // Co needs to be move_constructible
    output_store.cmd_map[task_id] =
        std::make_unique<Co>( std::move(cmd_output) );
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::show, Kind::target>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::target>(ta);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::show, Kind::plan>(TA const & ta, OI const & oi) {

    std::cout << std::endl << "show plan";
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::show, Kind::wait>(TA const & ta, OI const & oi) {

    // using sleep_until to discard the time streaming to cout
    auto const start_time = std::chrono::system_clock::now();

    std::chrono::seconds wait_seconds;

    if (oi.option.find('&') != std::string::npos) {
        wait_seconds = Utility::match_duration(oi.option);
    } else {
        wait_seconds = std::chrono::minutes(1);
    }

    std::cout << std::endl << wait_seconds << std::flush;

    int const cols = Utility::get_terminal_cols() - 4;
    auto const limit = static_cast<float>( wait_seconds.count() );

    std::cout << std::endl << "[";

    int pos = 0;
    int new_pos;

    for (std::chrono::seconds d(1); d <= wait_seconds; d++) {

        // std::this_thread::sleep_until(now + d);
        if ( Utility::cin_cancel(start_time + d) ) break;

        float const progress = d.count() / limit;
        new_pos = static_cast<int>(progress * cols);

        if (new_pos > pos) {
            std::cout << std::string(new_pos - pos, '=') << std::flush;
            pos = new_pos;
        }
    }

    std::cout << "]";
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::edit, Kind::plan>(TA const & ta, OI const & oi) {

    std::cout << std::endl << "edit plan";
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::set, Kind::ctrl>(TA const & ta, OI const & oi) {

    std::cout << std::endl << " !!!! sending reboot signal to digitizer !!!!";
    q_send_recv<Action::set, Kind::ctrl>(ta, oi);
    std::cout << std::endl << " !!!! reboot signal sent !!!!";
}

// -------------------------------------------------------------------------- //
/*
void Comm::last_reboot(Digitizer & q) {

    //TODO: change to an actual date
    C1Ping2 cmd_ping_2;
    cmd_ping_2.ping_type(2); // mini status request, empty

    C1Ping3 cmd_ping_3;
    md.send_recv(q.port_config, cmd_ping_2, cmd_ping_3);

    //cmd_ping_3.time_of_last_reboot();
}
*/

// *** REGISTRATION *** //
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::set, Kind::reg>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    try {
        // DP Request server registration
        C1Rqsrv cmd_rqsrv;
        cmd_rqsrv.serial_number(q.config.serial_number);

        // Q Response: Server Challenge
        C1Srvch cmd_srvch;

        // dance
        md.send_recv(q.port_config, cmd_rqsrv, cmd_srvch, false);

        // Store this computer ip_address_number, for future get reg checking
        ip_address_number = cmd_srvch.server_ip_address();

        // DP Response to Q challenge
        C1Srvrsp cmd_srvrsp;

        // Set all values for DP Response
        cmd_srvrsp.serial_number(q.config.serial_number);
        cmd_srvrsp.challenge_value( cmd_srvch.challenge_value() );
        cmd_srvrsp.server_ip_address( cmd_srvch.server_ip_address() );
        cmd_srvrsp.server_udp_port( cmd_srvch.server_udp_port() );
        cmd_srvrsp.registration_number( cmd_srvch.registration_number() );

        // generate random value
        // this should happen only once during the program

        std::random_device rd;
        std::mt19937 gen( rd() );
        std::uniform_int_distribution<> dis(0, 9999999);
        cmd_srvrsp.server_random_value( dis(gen) );

        // md5
        std::stringstream md5_ss;

        md5_ss << std::hex      << std::setfill('0');
        md5_ss << std::setw(16) << cmd_srvrsp.challenge_value();
        md5_ss << std::setw(8)  << cmd_srvrsp.server_ip_address();
        md5_ss << std::setw(4)  << cmd_srvrsp.server_udp_port();
        md5_ss << std::setw(4)  << cmd_srvrsp.registration_number();
        md5_ss << std::setw(16) << q.port_config.auth_code;
        md5_ss << std::setw(16) << cmd_srvrsp.serial_number();
        md5_ss << std::setw(16) << cmd_srvrsp.server_random_value();

        std::array<uint8_t, 16> md5_digest = Md5::hash( md5_ss.str() );

        cmd_srvrsp.md5_result(md5_digest);

        // Q ack/cerr of DP Response
        C1Cack cmd_cack;

        md.send_recv(q.port_config, cmd_srvrsp, cmd_cack, false);

        std::cout << std::endl << "\n** Digitizer REGISTERED! ** " << std::endl;

        // setup status
        q.port_config.registered = true;

        // create task_id
        auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::reg);
        auto const task_id = ta.hash() + ui_id;

        // move to cmd store
        output_store.cmd_map[task_id] =
            std::make_unique<C1Cack>( std::move(cmd_cack) );

    } catch (Exception const & e) {

        std::cerr << std::endl << "caught @Comm::run<set, reg>";
        q.port_config.registered = false;

        // error will trigger if a cerr is sent by the digitizer
        std::cerr << e.what();
        throw WarningException("Comm",
                               "registration",
                               "Registration failed");
    }
}

// *** DE - REGISTRATION *** //
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::set, Kind::dereg>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    C1Dsrv cmd_dsrv;
    cmd_dsrv.serial_number(q.config.serial_number);

    // Q ack/cerr of DP Response
    C1Cack cmd_cack;

    md.send_recv(q.port_config, cmd_dsrv, cmd_cack, false);

    std::cout << "\n\n ** deregistered from digitizer ** \n";

    // setup status
    q.port_config.registered = false;

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::dereg);
    auto const task_id = ta.hash() + ui_id;

    // move to cmd store
    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::auto_, Kind::cal>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto & s = sn.s_ref(ta);

    // log this
    // ---------------------------------------------------------------------- //
    /*
    TODO: use a logging library, output should be file + console
    std::ofstream out;
    out.open("out.txt", std::ofstream::out | std::ofstream::trunc);
    //save old buffer
    std::streambuf * cout_buffer = std::cout.rdbuf();
    //redirect std::cout to out.txt
    std::cout.rdbuf( out.rdbuf() );
    */

    // use msg task mechanism for the plan
    // ---------------------------------------------------------------------- //
    // with start cal as individual action for when printing individual msg_task
    // since auto cal is a series of start cal
    auto const start_cal_ui = UserInstruction(Action::start, Kind::cal);

    auto msg_tasks =
        cmd_file_reader_.construct_msg_tasks<Action::start, Kind::cal>(
            start_cal_ui,
            ta);

    auto const sce = sensor_control_cal(q, s);

    // the cal times depend on the digitizer time (seconds since epoch)
    // ---------------------------------------------------------------------- //
    for (auto & msg_task : msg_tasks) {
        auto * cal = dynamic_cast<C1Qcal *>( msg_task.cmd_send.get() );
        if (cal == nullptr) throw FatalException("Comm",
                                                 "autocal",
                                                 "cal nullptr");

        // settling_time is a duration, named following manual
        cal->starting_time( msg_task.exec_time() + cal->settling_time() );
        cal->sensor_control_enable = sce;
    }

    // stream full sequence
    // ---------------------------------------------------------------------- //
    std::cout << std::endl << "auto cal sequence for " << ta << ":\n";
    for (auto const & msg_task : msg_tasks) msg_task.stream<C1Qcal>(std::cout);

    // e300 keep alive setup
    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) {

        // plan cal might have been called before digitizer registration
        // where the e300 registration is done. This makes sure the e300
        // gets registered before proceeding. Also this is not caught here,
        // so if e300 registration fails, calibration fails, as it should.

        s.port_e300_ref().reg();

        // connect external calibration signal from e300

        s.port_e300_ref().cal_connect();

        // if the calibration plan takes more than an hour, the e300
        // needs to be kept awake or it will go back to "safe" mode
        // technically this only needs to happen if a single calibration
        // takes more than an hour.
        bool need_to_be_kept_alive = false;

        for (auto const & msg_task : msg_tasks) {
            if ( msg_task.run_duration() >= std::chrono::hours(1) ) {
                need_to_be_kept_alive = true;
                break;
            }
        }

        // for now the e300 gets kept alive their entire duration
        // TODO: use a vector of futures on keep_alive, to have different
        // keep alive periods if needed
        // right now it could do one long calibration like that
        // keeps alive on a separate thread
        if (need_to_be_kept_alive) {

            auto total_plan_run_duration = std::chrono::seconds(0);

            for (auto const & msg_task : msg_tasks) {
                total_plan_run_duration += msg_task.run_duration();
            }

            // give some time for registration and task creation
            auto constexpr keep_alive_delay = std::chrono::seconds(10);

            // sets cancel_keep_alive to false
            // but it can be set to true during the keep_alive_delay
            s.port_e300_ref().keep_alive(total_plan_run_duration,
                                     keep_alive_delay);
        }
    }

    // make sure cals are coordinated (not send a cal when there is one)
    // ---------------------------------------------------------------------- //
    auto cal_is_running = [&]() {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.global_status(true);
        // status
        C1Stat cmd_stat;

        // needs to be registered
        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        // get global status
        CxGlobalStatus * gs =
            dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

        if (gs == nullptr) throw FatalException("Comm",
                                                "autocal",
                                                "global stat nullptr");

        auto const & cs = gs -> calibrator_status;

        return ( cs.calibration_enable_is_on_this_second() or
                 cs.calibration_signal_is_on_this_second() or
                 cs.calibrator_should_be_generating_a_signal_but_isnt() );
    };

    // main loop of auto calibration
    // ---------------------------------------------------------------------- //
    try {

        // can't use delay since delay is meant for independently sent cals
        for (auto & msg_task : msg_tasks) {

            std::cout << std::endl << " ### now: "
                      << Time::sys_time_of_day() << " ###\n";

            Comm::run<Action::set, Kind::reg>(ta);

            // check if a calibration is going on
            for (int i = 0; i < 2; i++) {

                if ( cal_is_running() ) break;

                // add some more wiggle time, digitizer still running cals
                auto constexpr wiggle_duration = std::chrono::seconds(10);
                std::this_thread::sleep_for(wiggle_duration);

                if (i == 2) {
                    throw FatalException("Comm",
                                         "run<auto, cal>",
                                         "Calibrations are not coordinated");
                }
            }

            // ok ready to go
            msg_task.exec_time( std::chrono::system_clock::now() );
            msg_task.end_time( msg_task.exec_time() + msg_task.run_duration() );

            std::cout << "\nnext cal:\n";
            msg_task.stream<C1Qcal>(std::cout);

            md.send_recv( q.port_config,
                          *(msg_task.cmd_send.get()),
                          *(msg_task.cmd_recv.get()) );

            // no throw so far, received c1_cack
            msg_task.done = true;

            Comm::run<Action::set, Kind::dereg>(ta);

            // add some wiggle time in between
            auto constexpr wiggle_duration = std::chrono::seconds(20);
            // sleep on this thread, each msg task has the run_duration
            // already calculated.
            auto const sleep_duration = msg_task.run_duration() +
                                        wiggle_duration;

            CmdFieldTime<> sleep_until_time;
            sleep_until_time(std::chrono::system_clock::now() + sleep_duration);

            std::cout << std::endl << " ### now: "
                      << Time::sys_time_of_day() << " ###\n";

            std::cout << std::endl << "sleep for: " << sleep_duration
                      << " until: " << sleep_until_time << std::endl;

            std::this_thread::sleep_for(sleep_duration);
        }

        std::cout << std::endl << " ### now: "
                  << Time::sys_time_of_day() << " ###\n";

        std::cout << std::endl << "\nautocal success\n";

        // print out what was done
        for (auto & msg_task : msg_tasks) {
            std::cout << std::endl;
            msg_task.stream<C1Qcal>(std::cout);
        }

        // all done successfully, get the future from keep alive
        if (s.config.has_e300) s.port_e300_ref().wait_keep_alive();

    } catch (Exception const & e) {

        //std::cout.rdbuf(cout_buffer);

        // cancel keep alive thread and rethrow exception
        std::cerr << std::endl << "caught @Comm::run<auto, cal>";
        std::cerr << std::endl << "cancelling auto cal"
                  << "\n deregistering: \n";
        Comm::run<Action::set, Kind::dereg>(ta);
        std::cerr << "\n cancelling keep alive for e300: \n";
        // ok to set even if keep_alive(...)  was not called here
        if (s.config.has_e300) s.port_e300_ref().cancel_keep_alive();
        std::cerr << std::endl << "rethrow";
        throw e;
    }

    //std::cout.rdbuf(cout_buffer);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::auto_, Kind::stat>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    if ( not q_is_reg(q) ) Comm::run<Action::set, Kind::reg>(ta);

    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) { std::cout << "\nTODO E300 auto stat"; return;}

    // ---------------------------------------------------------------------- //
    using Point = std::array<int16_t, 3>;

    // ---------------------------------------------------------------------- //
    auto constexpr number_of_axis = 3;
    auto constexpr period = std::chrono::milliseconds(500);
    auto constexpr pps = 2; // points per second (changes with period)
    auto constexpr ppl = 2;  // points per line, changes plot looks

    StreamPlotter<int16_t, number_of_axis, pps, int8_t> sp(ppl);

    // changes the plot looks, adding a > when value at 127
    sp.min_limit = -126; sp.max_limit = 126;

    std::chrono::seconds loop_limit = std::chrono::minutes(2);

    for (std::chrono::milliseconds i(0); i < loop_limit; i += period) {

        Point const bp = boom_positions<Point>(q, s);

        sp.add(bp);
        sp.plot_lines();
        std::cout << std::flush;
        if ( Utility::cin_cancel(period) ) break;
    }
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::set, Kind::center>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    // --------- E300 registration ----------- //
    if (s.config.has_e300) throw InfoException("Comm",
                                               "run<set, center>",
                                               "TODO: set center for e300");
    using Minutes = std::chrono::minutes;

    // mass centering set
    auto constexpr maximum_tries = 5;
    auto constexpr normal_interval = Minutes(2);
    auto constexpr squelch_interval = Minutes(3);

    // how does this work with invalid sensors (boom at 20)?
    // legitimate sensors can also take the value 20
    auto constexpr tolerance = 0;

    // the pulse needs to be in CentiSeconds.
    // (q330 manual "Duration 10 ms intervals")
    // using centi directly assures there is no truncation, using milliseconds
    // will be interpreted as possible truncation and will not compile unless
    // using floor to the expected type
    auto constexpr pulse_duration = std::chrono::seconds(2);

    C2Samass cmd_samass;

    auto const sce = sensor_control_center(q, s);

    if (s.config.input == Sensor::Input::a) {
        cmd_samass.pulse_duration_1(pulse_duration);
        cmd_samass.tolerance_1a(tolerance);
        cmd_samass.tolerance_1b(tolerance);
        cmd_samass.tolerance_1c(tolerance);
        cmd_samass.maximum_tries_1(maximum_tries);
        cmd_samass.normal_interval_1(normal_interval);
        cmd_samass.squelch_interval_1(squelch_interval);
        cmd_samass.sensor_control_enable_1 = sce;
    } else {
        cmd_samass.pulse_duration_2(pulse_duration);
        cmd_samass.tolerance_2a(tolerance);
        cmd_samass.tolerance_2b(tolerance);
        cmd_samass.tolerance_2c(tolerance);
        cmd_samass.maximum_tries_2(maximum_tries);
        cmd_samass.normal_interval_2(normal_interval);
        cmd_samass.squelch_interval_2(squelch_interval);
        cmd_samass.sensor_control_enable_2 = sce;
    }


    C1Cack cmd_cack;
    md.send_recv(q.port_config, cmd_samass, cmd_cack);

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::center);
    auto const task_id = ta.hash() + ui_id;

    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::start, Kind::center>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    // ---------------------------------------------------------------------- //
    auto centering_is_running = [&]() {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.global_status(true);
        // status
        C1Stat cmd_stat;

        // needs to be registered
        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        // get global status
        CxGlobalStatus * gs =
            dynamic_cast<CxGlobalStatus *>( cmd_stat.inner_commands[0].get() );

        if (gs == nullptr) throw FatalException("Comm",
                                                "autocal",
                                                "global stat nullptr");

        auto const & scm = gs -> sensor_control_enable();

        return ( static_cast<bool>( scm.to_ulong() ) );
    };

    // make sure another centering is not running
    // ---------------------------------------------------------------------- //
    if ( centering_is_running() ) {
        throw FatalException("Comm",
                             "run<auto, cal>",
                             "Calibrations are not coordinated");
    }

    // ---------------------------------------------------------------------- //
    // pulse setup for mass centering
    C1Pulse cmd_pulse;

    // the pulse needs to be in CentiSeconds.
    // (q330 manual "Duration 10 ms intervals")
    // using centi directly assures there is no truncation, using milliseconds
    // will be interpreted as possible truncation and will not compile unless
    // using floor to the expected type
    std::chrono::duration<uint16_t, std::centi> pulse_duration;

    if (oi.option.find('&') != std::string::npos) {
        pulse_duration = Utility::match_duration(oi.option);
    } else {
        pulse_duration = std::chrono::seconds(2);
    }

    auto constexpr consensus_max_pulse_duration = std::chrono::seconds(10);
    if (pulse_duration > consensus_max_pulse_duration) {
        throw WarningException("Comm",
                               "run <start, center>",
                               "pulse duration > 10 seconds");
    }

    cmd_pulse.pulse_duration(pulse_duration);

    auto const sce = sensor_control_center(q, s);
    cmd_pulse.sensor_control_enable = sce;

    C1Cack cmd_cack;
    md.send_recv(q.port_config, cmd_pulse, cmd_cack);

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::start, Kind::center);
    auto const task_id = ta.hash() + ui_id;

    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::auto_, Kind::center>(TA const & ta, OI const & oi) {


    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_const_ref(ta);

    if ( not q_is_reg(q) ) Comm::run<Action::set, Kind::reg>(ta);

    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) { std::cout << "\nTODO E300 auto center"; return;}

    // ---------------------------------------------------------------------- //
    using Point = std::array<int16_t, 3>;

    auto constexpr tolerance = 10;
    // ---------------------------------------------------------------------- //
    auto centered = [](Point const & bp, auto const & tolerance) {
        for (auto const & bp_axis : bp) {
            if (std::abs(bp_axis) > tolerance) return false;
        }
        return true;
    };

    // ---------------------------------------------------------------------- //
    auto stream_bp = [](auto const & bp) {
        std::cout << "[";
        for (auto const & bp_axis : bp) {
            std::cout << std::setw(5) << bp_axis << " ";
        }
        std::cout << "]";
    };

    std::vector<Point> bps;
    auto constexpr max_attempts = 5;
    bool success = false;

    OptionInput const oi_show_wait("&2m");
    OptionInput const oi_auto_stat("boom");
    // should match show wait (2m) and auto stat (2m)
    auto constexpr sleep_duration = std::chrono::minutes(4);

    // ---------------------------------------------------------------------- //
    for (int i = 0; i < max_attempts; i++) {

        Point const bp = boom_positions<Point>(q, s);
        bps.push_back(bp);
        success = centered(bp, tolerance);
        if (success) break;

        auto const start_time = std::chrono::system_clock::now();

        std::cout << std::endl << "## "
                  << Time::sys_time_of_day(start_time) << " ## "
                  << Time::sys_year_month_day(start_time) << " ## "
                  << "julian( " << Time::julian_day() << ") ##\n";

        std::cout << "\ntolerance      : " << tolerance;
        std::cout << "\nmass positions : "; stream_bp(bp);


        // lets try to center this
        run<Action::start, Kind::center>(ta, oi);
        std::cout << std::endl << "mass center updates:";
        run<Action::auto_, Kind::stat>(ta, oi_auto_stat);
        std::cout << std::endl << "wait before checking centers again";
        run<Action::show,  Kind::wait>(ta, oi_show_wait);
        // force the wait even without stream output, if both previous
        // instructions are not interrupted, this should sleep almost nothing
        std::this_thread::sleep_until(start_time + sleep_duration);
        std::cout << std::flush;
    }

    // ---------------------------------------------------------------------- //
    auto const & st = sn.st[ta.st_child.index];

    std::cout << "\nstation   : " << st.config.station_name;
    std::cout << "\ndigitizer : " << q;
    std::cout << "\nsensor    : " << s;
    std::cout << "\nmass positions :\n";

    for (auto const bp : bps) {
        std::cout << "\n";
        stream_bp(bp);
    }

    std::cout << std::endl << " ### now: "
              << Time::sys_time_of_day() << " ###\n";
    std::cout << "\n\nresult    : " << (success ? "centered" : "!! failure");
    std::cout <<   "\ntolerance : " << tolerance;
}

// *** QUICK VIEW *** //
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::auto_, Kind::qview>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    if ( not q_is_reg(q) ) Comm::run<Action::set, Kind::reg>(ta);

    C2Rqqv cmd_rqqv;
    C2Qv cmd_qv;

    auto const & s = sn.s_const_ref(ta);

    if (s.config.input == Sensor::Input::a) {
        cmd_rqqv.channel_map.channel_1(true);
        cmd_rqqv.channel_map.channel_2(true);
        cmd_rqqv.channel_map.channel_3(true);
    } else {
        cmd_rqqv.channel_map.channel_4(true);
        cmd_rqqv.channel_map.channel_5(true);
        cmd_rqqv.channel_map.channel_6(true);

    }

    using Point = std::array<int32_t, 3>;

    // initialized with current, updated with the received ones
    int qv_seq_number = q_current_seq_number(q);

    // ---------------------------------------------------------------------- //
    auto qview_values = [&]() {

        // "39 byte differences from starting value (40 bytes actually used)"
        // the last point in the bit difference vector is always zero.
        // it provides 40 points but only 39 are usable.
        // the first point (absolute value) of the 40 is actually given by
        // starting value, the other 39 are calculated using the differences
        auto constexpr N = 40;
        std::vector<Point> qview_values(N);

        // you can get data from before seq_number_
        // then you get several qv values
        // now we would have to deal either with overlap or a small gap
        cmd_rqqv.lowest_sequence_number(qv_seq_number);

        md.send_recv(q.port_config, cmd_rqqv, cmd_qv, false);

        // update values for next qv, assuming one
        qv_seq_number = cmd_qv.starting_sequence_number() +
                        cmd_qv.seconds_count().count();

        // get the values
        for (int axis = 0; axis < cmd_qv.inner_commands.size(); axis++) {

            auto & cmd_cx_qv = cmd_qv.inner_commands[axis];
            CxQv * qv = dynamic_cast<CxQv *>( cmd_cx_qv.get() );

            if (qv == nullptr) throw std::logic_error{"@Comm::qview"};

            // "Each [byte] difference must be multiplied by (1 << shift count)"
            // CmdField<uint16_t, 2> shift_count;
            auto const shift_count = qv -> shift_count();
            auto const multiplier = 1 << shift_count;

            auto const starting_value = qv -> starting_value();

            qview_values[0][axis] = starting_value;

            // takes data differences, which are signed bytes
            // CmdField<std::array<int8_t, 40>, 40> byte_difference;
            int8_t data_point;

            // first point already taken
            for (int i = 1; i < N; i++) {
                data_point = qv -> byte_difference()[i];
                auto const starting_diff = data_point * multiplier;
                qview_values[i][axis] = starting_value + starting_diff;
            }
        }

        return qview_values;
    };

    // number is 26 bit signed integer, T and Tc both int32_t
    // ---------------------------------------------------------------------- //
    auto constexpr number_of_axis = 3;
    auto constexpr ppl = 20;  // points per line, changes plot looks
    auto constexpr pps = 40; // points per second (property of qview)
    StreamPlotter<int32_t, number_of_axis, pps> sp(ppl);

    // just changes the plot looks
    sp.min_limit = -33'000'000; sp.max_limit = 33'000'000;

    // 16 minutes, less than 999 seconds
    auto constexpr max_loop_limit = std::chrono::minutes(15);

    std::chrono::seconds loop_limit;
    if (oi.option.find('&') == std::string::npos) loop_limit = max_loop_limit;
    else loop_limit = Utility::match_duration(oi.option);

    // do not change the period
    auto constexpr period = std::chrono::seconds(1);

    for (std::chrono::seconds i(0); i < loop_limit; i += period) {

        auto const qvv = qview_values();

        sp.add(qvv);
        sp.plot_lines();
        if ( Utility::cin_cancel(period) ) break;
    }

    sp.set_ppl(4);
    std::cout << std::endl;
    sp.plot_all();

}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::quit, Kind::mzn>(TA const & ta, OI const & oi) {
    // TODO clean up mtm
    // md.join_timed_threads();
}

//TODO: compound, utility function, move separetely?
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::uptime>(TA const & ta, OI const & oi) {

    auto & st = sn.st_ref(ta);

    std::ofstream uptime_report_fs;

    uptime_report_fs.open("uptime_report.txt",
          std::ofstream::out | std::ofstream::trunc);

    uptime_report_fs << "\nstation: " << st.config.station_name << std::endl;

    for (auto & q : st.q) {

        uptime_report_fs << "\n    digitizer: ";
        uptime_report_fs << "    " << std::hex
                         << q.config.serial_number
                         << std::dec;

        uptime_report_fs << "\n    time of last reboot: ";
        // uptime_report_fs <<  last_reboot(q)  ;
    }

    uptime_report_fs << "\n    data processor: uptime:\n ";
    uptime_report_fs << "    " << st.dp[0].uptime();

    uptime_report_fs.close();
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::edit, Kind::stat>(TA const & ta, OI const & oi) {
    //TODO: use type traits
    //TODO: show users shortcut and live options
    // cmd_show_options<C1Ping4, Action::edit, Kind::stat>();
}

// -------------------------------------------------------------------------- //
// for stream_output
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::show, Kind::config>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::config>(ta);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::show, Kind::status>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::status>(ta);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::show, Kind::command>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::command>(ta);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::start, Kind::link>(TA const & ta, OI const & oi) {

    auto & s = sn.s_ref(ta);

    // --------- E300 registration ----------- //
    if (not s.config.has_e300) {
        throw WarningException("Comm",
                               "run<start, link>",
                               "this sensor does not have E300 setup");
    }

    auto & e300 = s.port_e300_ref();

    e300.connect();

    std::cout << std::endl << "input commands for E300, type quit to quit";
    std::string input{};

    while (true) {
        std::cout << "\n_______________________________________";
        std::cout << std::endl << "E300> ";
        std::getline(std::cin, input);
        if (input == "quit") break;
        auto const response = e300.send_recv(input);
        std::cout << response;
    }
}

} // << mzn

