#include "cx_global_status.h"

namespace mzn {
CxGlobalStatus::CxGlobalStatus():
    Command(0, 52),
    acquisition_control(),
    clock_quality(),
    minutes_since_loss(),
    analog_voltage_control_valve("counts"),
    seconds_offset(),
    usec_offset(),
    total_time_in_seconds(),
    power_on_time_in_seconds(),
    time_of_last_re_sync(),
    total_number_of_re_syncs(),
    gps_status(),
    calibrator_status(),
    sensor_control_active_high(),
    sensor_control_map(),
    current_vco(),
    data_sequence_number(),
    pll_running(),
    status_inputs(),
    misc_inputs(),
    current_data_sequence_number() { }

uint16_t CxGlobalStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                     uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxGlobalStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = acquisition_control.msg_to_data(msg, mf_begin);
    mf_begin = clock_quality.msg_to_data(msg, mf_begin);
    mf_begin = minutes_since_loss.msg_to_data(msg, mf_begin);
    mf_begin = analog_voltage_control_valve.msg_to_data(msg, mf_begin);
    mf_begin = seconds_offset.msg_to_data(msg, mf_begin);
    mf_begin = usec_offset.msg_to_data(msg, mf_begin);
    mf_begin = total_time_in_seconds.msg_to_data(msg, mf_begin);
    mf_begin = power_on_time_in_seconds.msg_to_data(msg, mf_begin);
    mf_begin = time_of_last_re_sync.msg_to_data(msg, mf_begin);
    mf_begin = total_number_of_re_syncs.msg_to_data(msg, mf_begin);
    mf_begin = gps_status.msg_to_data(msg, mf_begin);
    mf_begin = calibrator_status.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_map.msg_to_data(msg, mf_begin);
    mf_begin = current_vco.msg_to_data(msg, mf_begin);
    mf_begin = data_sequence_number.msg_to_data(msg, mf_begin);
    mf_begin = pll_running.msg_to_data(msg, mf_begin);
    mf_begin = status_inputs.msg_to_data(msg, mf_begin);
    mf_begin = misc_inputs.msg_to_data(msg, mf_begin);
    mf_begin = current_data_sequence_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxGlobalStatus::data_to_msg(std::vector<uint8_t> & msg,
                                     uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxGlobalStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = acquisition_control.data_to_msg(msg, mf_begin);
    mf_begin = clock_quality.data_to_msg(msg, mf_begin);
    mf_begin = minutes_since_loss.data_to_msg(msg, mf_begin);
    mf_begin = analog_voltage_control_valve.data_to_msg(msg, mf_begin);
    mf_begin = seconds_offset.data_to_msg(msg, mf_begin);
    mf_begin = usec_offset.data_to_msg(msg, mf_begin);
    mf_begin = total_time_in_seconds.data_to_msg(msg, mf_begin);
    mf_begin = power_on_time_in_seconds.data_to_msg(msg, mf_begin);
    mf_begin = time_of_last_re_sync.data_to_msg(msg, mf_begin);
    mf_begin = total_number_of_re_syncs.data_to_msg(msg, mf_begin);
    mf_begin = gps_status.data_to_msg(msg, mf_begin);
    mf_begin = calibrator_status.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_map.data_to_msg(msg, mf_begin);
    mf_begin = current_vco.data_to_msg(msg, mf_begin);
    mf_begin = data_sequence_number.data_to_msg(msg, mf_begin);
    mf_begin = pll_running.data_to_msg(msg, mf_begin);
    mf_begin = status_inputs.data_to_msg(msg, mf_begin);
    mf_begin = misc_inputs.data_to_msg(msg, mf_begin);
    mf_begin = current_data_sequence_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxGlobalStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_GLOBAL_STATUS ---  \n";

    cmd_os << "\nacquisition_control: "; cmd_os << acquisition_control;

    cmd_os << "\nclock_quality: "; cmd_os << clock_quality;

    cmd_os << "\nminutes_since_loss: "; cmd_os << minutes_since_loss;

    cmd_os << "\nanalog_voltage_control_valve: "; cmd_os << analog_voltage_control_valve;

    cmd_os << "\nseconds_offset: "; cmd_os << seconds_offset;

    cmd_os << "\nusec_offset: "; cmd_os << usec_offset;

    cmd_os << "\ntotal_time_in_seconds: "; cmd_os << total_time_in_seconds;

    cmd_os << "\npower_on_time_in_seconds: "; cmd_os << power_on_time_in_seconds;

    cmd_os << "\ntime_of_last_re_sync: "; cmd_os << time_of_last_re_sync;

    cmd_os << "\ntotal_number_of_re_syncs: "; cmd_os << total_number_of_re_syncs;

    cmd_os << "\ngps_status: "; cmd_os << gps_status;

    cmd_os << "\ncalibrator_status: "; cmd_os << calibrator_status;

    cmd_os << "\nsensor_control_active_high: "; cmd_os << sensor_control_active_high;

    cmd_os << "\nsensor_control_map: "; cmd_os << sensor_control_map;

    cmd_os << "\ncurrent_vco: "; cmd_os << current_vco;

    cmd_os << "\ndata_sequence_number: "; cmd_os << data_sequence_number;

    cmd_os << "\npll_running: "; cmd_os << pll_running;

    cmd_os << "\nstatus_inputs: "; cmd_os << status_inputs;

    cmd_os << "\nmisc_inputs: "; cmd_os << misc_inputs;

    cmd_os << "\ncurrent_data_sequence_number: "; cmd_os << current_data_sequence_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace