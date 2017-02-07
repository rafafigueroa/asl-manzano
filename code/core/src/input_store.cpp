// Manzano software
#include "input_store.h"
namespace mzn {

// -------------------------------------------------------------------------- //
InputStore::InputStore(SeismicNetwork const & sn) : sn_(sn) {}

// -------------------------------------------------------------------------- //
template<>
typename Ci<Action::get, Kind::ping>::type
InputStore::
get_default_cmd<Action::get, Kind::ping>(TargetAddress const & ta,
                                         std::string const & option) {
    //  unless there an option is provided and is in the cmd_map
    //if (ui.option != "") return get_live_cmd<C1Ping4>(ui.option);

    using Ci = typename Ci<Action::get, Kind::ping>::type;
    Ci cmd;

    if (option == "") cmd.ping_type(4); else
    throw_bad_option(option);

    return cmd;
}

// -------------------------------------------------------------------------- //
template<>
typename Ci<Action::get, Kind::stat>::type
InputStore::
get_default_cmd<Action::get, Kind::stat>(TargetAddress const & ta,
                                         std::string const & option) {

    //  unless there an option is provided and is in the cmd_map
    //if (ui.option != "") return get_live_cmd<C1Ping4>(ui.option);

    using Ci = typename Ci<Action::get, Kind::stat>::type;
    Ci cmd;

    if (option == "") cmd.request_bitmap.global_status(true); else
    if (option == "boom") cmd.request_bitmap.boom_positions(true); else
    if (option == "gps") cmd.request_bitmap.gps_status(true); else
    if (option == "gpssat") cmd.request_bitmap.gps_satellites(true); else
    if (option == "power") cmd.request_bitmap.power_supply_status(true); else
    if (option == "thread") cmd.request_bitmap.thread_status(true); else

    if (option == "dataport") {
        cmd.request_bitmap.data_port_1_status(true);
        cmd.request_bitmap.data_port_2_status(true);
        cmd.request_bitmap.data_port_3_status(true);
        cmd.request_bitmap.data_port_4_status(true);
    } else

    if (option == "pll") cmd.request_bitmap.pll_status(true); else
    throw_bad_option(option);

    return cmd;
}

// -------------------------------------------------------------------------- //
template<>
typename Ci<Action::start, Kind::cal>::type
InputStore::
get_default_cmd<Action::start, Kind::cal>(TargetAddress const & ta,
                                          std::string const & option) {

    using Ci = typename Ci<Action::start, Kind::cal>::type;
    Ci cmd;

    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
    // durations in seconds
    if (option == "step") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::step);
        cmd.amplitude(-6);
        cmd.settling_time( Seconds(0) );
        cmd.cal_duration( Minutes(2) );
        cmd.trailer_time( Seconds(0) );

    } else if (option == "sine") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::sine);
        cmd.amplitude(-30);
        cmd.settling_time( Minutes(2) );
        cmd.cal_duration( Minutes(2) );
        cmd.trailer_time( Minutes(1) );

    } else if (option == "longstep") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::step);
        cmd.amplitude(-6);
        cmd.settling_time( Minutes(15) );
        cmd.cal_duration( Minutes(15) );
        cmd.trailer_time( Minutes(15) );

    } else if (option == "longsine") {

        cmd.waveform.waveform(BmCalWaveform::Waveform::sine);
        cmd.amplitude(-30);
        cmd.settling_time( Minutes(5) );
        cmd.cal_duration( Minutes(10) );
        cmd.trailer_time( Minutes(5) );

    } else {
        throw_bad_option(option);
    }

    // Need to setup basic command before choosing options
    auto const & s = sn_.s_const_ref(ta);

    cmd.sensor_control_active_high(false);
    using SCML = BmSensorControlMap::Lines;

    // sensor calibration channel first
    if (s.config.input == Sensor::Input::a) {
        cmd.calibration_bitmap.input(BmCalibrationBitmap::Input::a);
        cmd.sensor_control_map.lines(SCML::sensor_a_calibration);
        cmd.monitor_channel_bitmap.channel_5(true);
    } else {
        cmd.calibration_bitmap.input(BmCalibrationBitmap::Input::b);
        cmd.sensor_control_map.lines(SCML::sensor_b_calibration);
        cmd.monitor_channel_bitmap.channel_2(true);
    }

    cmd.waveform.automatic_calibration(true);
    cmd.waveform.negative_step(false);
    cmd.frequency_divider(1);

    std::array<char, 12> constexpr
        coupling_bytes {{'r','e','s','i','s','t','i','v','e'}};

    cmd.coupling_bytes(coupling_bytes);

    cmd.starting_time( std::chrono::system_clock::now() + cmd.settling_time() );

    return cmd;
}

// -------------------------------------------------------------------------- //
std::map< uint16_t, InputStore::OptionCmdMap > InputStore::mpid_options_map;

// -------------------------------------------------------------------------- //
template<>
typename Ci<Action::set, Kind::ctrl>::type
InputStore::
get_default_cmd<Action::set, Kind::ctrl>(TargetAddress const & ta,
                                         std::string const & option) {

    using Ci = typename Ci<Action::set, Kind::ctrl>::type;
    Ci cmd;

    // no default command without explicit option

    if (option == "save")
        cmd.ctrl_flags.save_current_programming_to_eeprom(true); else

    if (option == "reboot") cmd.ctrl_flags.reboot(true); else
    if (option == "resync") cmd.ctrl_flags.re_sync(true); else
    if (option == "gps_on") cmd.ctrl_flags.turn_gps_on(true); else
    if (option == "gps_off") cmd.ctrl_flags.turn_gps_off(true); else
    if (option == "gps_cold_start") cmd.ctrl_flags.cold_start_gps(true); else
    throw_bad_option(option);

    return cmd;
}

} // <- mzn
