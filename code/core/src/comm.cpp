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
void Comm::run<Action::get, Kind::ctrl>(TA const & ta, OI const & oi) {
    // sensor_control_mapping, C1Rqsc , C1Sc
    q_send_recv<Action::get, Kind::ctrl>(ta, oi);
}

// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::dev>(TA const & ta, OI const & oi) {
    // devices, C1Rqdev, C1Dev (multicommand)
    q_send_recv<Action::get, Kind::dev>(ta, oi);
}

// *** CALIBRATIONS *** //
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::start, Kind::cal>(TA const & ta, OI const & oi) {

    auto & s = sn.s_ref(ta);

    // --------- E300 registration ----------- //
    if (s.config.has_e300) {
        s.port_e300_ref().reg();
        std::cout << std::endl << "\n** E300 REGISTERED! ** " << std::endl;
        s.port_e300_ref().cal_connect();
    }

    // qcal, C1Qcal, C1Cack, no default option
    q_send_recv<Action::start, Kind::cal>(ta, oi);
}
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::stop, Kind::cal>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);

    C1Rqglob cmd_send;
    C1Glob cmd_recv;
    md.send_recv(q.port_config, cmd_send, cmd_recv);

    /*

    //TODO: does it receive ack?
    auto & q = sn.q_ref(ta);

    C1Stop cmd_stop; // Calibration Stop
    C1Cack cack;
    md.send_recv(q.port_config, cmd_stop, cack);
    */
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

    // the cal times depend on the digitizer time (seconds since epoch)
    // ---------------------------------------------------------------------- //
    for (auto & msg_task : msg_tasks) {
        auto * cal = dynamic_cast<C1Qcal *>( msg_task.cmd_send.get() );
        if (cal == nullptr) throw FatalException("Comm",
                                                 "autocal",
                                                 "cal nullptr");

        // settling_time is a duration, named following manual
        cal->starting_time( msg_task.exec_time() + cal->settling_time() );
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
    auto & s = sn.s_ref(ta);

    // e300 keep alive setup
    // ---------------------------------------------------------------------- //
    if (s.config.has_e300) { std::cout << "\nTODO test if needed"; return;}

    // make sure cals are coordinated (not send a cal when there is one)
    // ---------------------------------------------------------------------- //
    auto boom_positions = [&]() -> std::array<int, 6> const {

        // request status
        C1Rqstat cmd_rqstat;
        cmd_rqstat.request_bitmap.boom_positions(true);
        // status
        C1Stat cmd_stat;

        // needs to be registered
        md.send_recv(q.port_config, cmd_rqstat, cmd_stat, false);

        // get global status
        CxBoomPositions * bp =
            dynamic_cast<CxBoomPositions  *>( cmd_stat.inner_commands[0].get() );

        if (bp == nullptr) throw FatalException("Comm",
                                                "run",
                                                "boom positions nullptr");

        std::array<int, 6> const boom_positions {bp -> channel_1_boom(),
                                                 bp -> channel_2_boom(),
                                                 bp -> channel_3_boom(),
                                                 bp -> channel_4_boom(),
                                                 bp -> channel_5_boom(),
                                                 bp -> channel_6_boom()};
        return boom_positions;
    };

    // ---------------------------------------------------------------------- //
    try {

        auto constexpr loop_limit = 10;
        auto constexpr period = std::chrono::seconds(1); 
        Comm::run<Action::set, Kind::reg>(ta);

        std::cout << std::endl << " ### now: " << Time::sys_time_of_day() << " ###\n";

        // can't use delay since delay is meant for independently sent cals
        for (int i = 0; i < loop_limit; i++) {

            auto const bp = boom_positions();

            std::cout << "<" << bp[0] << ">";

            std::this_thread::sleep_for(period);
        }

        Comm::run<Action::set, Kind::dereg>(ta);

        std::cout << std::endl << " ### now: "
                  << Time::sys_time_of_day() << " ###\n";

    } catch (Exception const & e) {

        //std::cout.rdbuf(cout_buffer);

        // cancel keep alive thread and rethrow exception
        std::cerr << std::endl << "caught @Comm::run<auto, stat:boom>";
        std::cerr << std::endl << "cancelling auto stat:boom"
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
void Comm::run<Action::set, Kind::center>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_ref(ta);

    // TODO experiment with these values

    using Minutes = std::chrono::minutes;

    // mass centering set
    C2Samass cmd_samass;
    cmd_samass.tolerance_1a(1);
    cmd_samass.tolerance_1b(1);
    cmd_samass.tolerance_1c(1);
    cmd_samass.maximum_tries_1(3);
    cmd_samass.normal_interval_1( Minutes(2) );
    cmd_samass.squelch_interval_1( Minutes(2) );
    // the pulse needs to be in CentiSeconds.
    // (q330 manual "Duration 10 ms intervals")
    // using centi directly assures there is no truncation, using milliseconds
    // will be interpreted as possible truncation and will not compile unless
    // using floor to the expected type
    auto constexpr pulse_duration = std::chrono::duration<int, std::centi>(95);

    cmd_samass.pulse_duration_1(pulse_duration);

    if (s.config.input == Sensor::Input::a) {
        cmd_samass.sensor_control_bitmap_1.sensor_control_mapping(
            BmStatSensorControlBitmap::SensorControlMapping::sensor_a_centering);
    } else {
        cmd_samass.sensor_control_bitmap_1.sensor_control_mapping(
            BmStatSensorControlBitmap::SensorControlMapping::sensor_b_centering);
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
void Comm::run<Action::start, Kind::pulse>(TA const & ta, OI const & oi) {

    auto & q = sn.q_ref(ta);
    auto const & s = sn.s_ref(ta);

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

        auto const & scm = gs -> sensor_control_bitmap.sensor_control_mapping();

        using SCM = BmStatSensorControlBitmap::SensorControlMapping;

        return (scm == SCM::sensor_a_centering or
                scm == SCM::sensor_b_centering or
                scm == SCM::sensor_a_calibration or
                scm == SCM::sensor_b_calibration);
    };

    // make sure another centering is not running
    if ( centering_is_running() ) {
        throw FatalException("Comm",
                             "run<auto, cal>",
                             "Calibrations are not coordinated");
    }

    // pulse setup for mass centering
    C1Pulse cmd_pulse;

    // the pulse needs to be in CentiSeconds.
    // (q330 manual "Duration 10 ms intervals")
    // using centi directly assures there is no truncation, using milliseconds
    // will be interpreted as possible truncation and will not compile unless
    // using floor to the expected type
    auto constexpr pulse_duration = std::chrono::duration<int, std::centi>(90);

    cmd_pulse.pulse_duration(pulse_duration);

    if (s.config.input == Sensor::Input::a) {
        cmd_pulse.sensor_control_bitmap.sensor_control_mapping(
            BmStatSensorControlBitmap::SensorControlMapping::sensor_a_centering);
    } else {
        cmd_pulse.sensor_control_bitmap.sensor_control_mapping(
            BmStatSensorControlBitmap::SensorControlMapping::sensor_b_centering);
    }

    C1Cack cmd_cack;
    md.send_recv(q.port_config, cmd_pulse, cmd_cack);

    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::set, Kind::center);
    auto const task_id = ta.hash() + ui_id;

    output_store.cmd_map[task_id] =
        std::make_unique<C1Cack>( std::move(cmd_cack) );
}

// *** QUICK VIEW *** //
// -------------------------------------------------------------------------- //
template<>
void Comm::run<Action::get, Kind::qview>(TA const & ta, OI const & oi) {
    // no option yet in green_manzano
    // handled completely by red_manzano, no common library worked
    /*
    // create task_id
    auto constexpr ui_id = UserInstruction::hash(Action::get, Kind::qview);
    auto const task_id = ta.hash() + ui_id;

    // move to cmd store
     output_store.cmd_map[task_id] =
        std::make_unique<C2Qv>( std::move(cmd_qv) );
        */
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

template<>
void Comm::run<Action::show, Kind::status>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::status>(ta);
}

template<>
void Comm::run<Action::show, Kind::command>(TA const & ta, OI const & oi) {
    stream_output.show<Kind::command>(ta);
}

/*
template<>
void Comm::data_recv(Digitizer & q) {

    auto & q = sn.q_ref(ta);

    DtOpen cmd_data_open; // start data stream

    md.data_recv(q, cmd_data_open, data_msg);
}

*/
} // << mzn

