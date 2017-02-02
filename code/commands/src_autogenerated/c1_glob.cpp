#include "c1_glob.h"

namespace mzn {
C1Glob::C1Glob():
    Command(0xA6, 160),
    clock_timeout(),
    initial_vco(),
    gps_backup_power(),
    aux_status_web_flags(),
    gain_pre_amp(),
    linear_phase_filters(),
    input_bitmap(),
    web_server_tcp_port(),
    server_timeout(),
    drift_tolerance(),
    jump_filter(),
    jump_threshold(),
    calibrator_offset(),
    sensor_control_active_high(),
    sensor_control_map(),
    sampling_phase(),
    gps_cold_start_seconds(),
    kmi_user_tag(),
    channel_1_freq_bit_0_scale(),
    channel_1_freq_bit_1_scale(),
    channel_1_freq_bit_2_scale(),
    channel_1_freq_bit_3_scale(),
    channel_1_freq_bit_4_scale(),
    channel_1_freq_bit_5_scale(),
    channel_1_freq_bit_6_scale(),
    channel_1_freq_bit_7_scale(),
    channel_2_freq_bit_0_scale(),
    channel_2_freq_bit_1_scale(),
    channel_2_freq_bit_2_scale(),
    channel_2_freq_bit_3_scale(),
    channel_2_freq_bit_4_scale(),
    channel_2_freq_bit_5_scale(),
    channel_2_freq_bit_6_scale(),
    channel_2_freq_bit_7_scale(),
    channel_3_freq_bit_0_scale(),
    channel_3_freq_bit_1_scale(),
    channel_3_freq_bit_2_scale(),
    channel_3_freq_bit_3_scale(),
    channel_3_freq_bit_4_scale(),
    channel_3_freq_bit_5_scale(),
    channel_3_freq_bit_6_scale(),
    channel_3_freq_bit_7_scale(),
    channel_4_freq_bit_0_scale(),
    channel_4_freq_bit_1_scale(),
    channel_4_freq_bit_2_scale(),
    channel_4_freq_bit_3_scale(),
    channel_4_freq_bit_4_scale(),
    channel_4_freq_bit_5_scale(),
    channel_4_freq_bit_6_scale(),
    channel_4_freq_bit_7_scale(),
    channel_5_freq_bit_0_scale(),
    channel_5_freq_bit_1_scale(),
    channel_5_freq_bit_2_scale(),
    channel_5_freq_bit_3_scale(),
    channel_5_freq_bit_4_scale(),
    channel_5_freq_bit_5_scale(),
    channel_5_freq_bit_6_scale(),
    channel_5_freq_bit_7_scale(),
    channel_6_freq_bit_0_scale(),
    channel_6_freq_bit_1_scale(),
    channel_6_freq_bit_2_scale(),
    channel_6_freq_bit_3_scale(),
    channel_6_freq_bit_4_scale(),
    channel_6_freq_bit_5_scale(),
    channel_6_freq_bit_6_scale(),
    channel_6_freq_bit_7_scale(),
    channel_1_offset(),
    channel_2_offset(),
    channel_3_offset(),
    channel_4_offset(),
    channel_5_offset(),
    channel_6_offset(),
    channel_1_gain(),
    channel_2_gain(),
    channel_3_gain(),
    channel_4_gain(),
    channel_5_gain(),
    channel_6_gain(),
    ignore() { }

uint16_t C1Glob::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Glob",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = clock_timeout.msg_to_data(msg, mf_begin);
    mf_begin = initial_vco.msg_to_data(msg, mf_begin);
    mf_begin = gps_backup_power.msg_to_data(msg, mf_begin);
    mf_begin = aux_status_web_flags.msg_to_data(msg, mf_begin);
    mf_begin = gain_pre_amp.msg_to_data(msg, mf_begin);
    mf_begin = linear_phase_filters.msg_to_data(msg, mf_begin);
    mf_begin = input_bitmap.msg_to_data(msg, mf_begin);
    mf_begin = web_server_tcp_port.msg_to_data(msg, mf_begin);
    mf_begin = server_timeout.msg_to_data(msg, mf_begin);
    mf_begin = drift_tolerance.msg_to_data(msg, mf_begin);
    mf_begin = jump_filter.msg_to_data(msg, mf_begin);
    mf_begin = jump_threshold.msg_to_data(msg, mf_begin);
    mf_begin = calibrator_offset.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_active_high.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_map.msg_to_data(msg, mf_begin);
    mf_begin = sampling_phase.msg_to_data(msg, mf_begin);
    mf_begin = gps_cold_start_seconds.msg_to_data(msg, mf_begin);
    mf_begin = kmi_user_tag.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_0_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_1_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_2_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_3_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_4_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_5_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_6_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_freq_bit_7_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_0_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_1_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_2_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_3_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_4_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_5_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_6_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_freq_bit_7_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_0_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_1_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_2_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_3_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_4_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_5_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_6_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_freq_bit_7_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_0_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_1_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_2_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_3_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_4_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_5_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_6_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_freq_bit_7_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_0_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_1_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_2_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_3_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_4_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_5_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_6_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_freq_bit_7_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_0_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_1_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_2_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_3_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_4_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_5_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_6_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_freq_bit_7_scale.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_offset.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_offset.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_offset.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_offset.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_offset.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_offset.msg_to_data(msg, mf_begin);
    mf_begin = channel_1_gain.msg_to_data(msg, mf_begin);
    mf_begin = channel_2_gain.msg_to_data(msg, mf_begin);
    mf_begin = channel_3_gain.msg_to_data(msg, mf_begin);
    mf_begin = channel_4_gain.msg_to_data(msg, mf_begin);
    mf_begin = channel_5_gain.msg_to_data(msg, mf_begin);
    mf_begin = channel_6_gain.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type

    return mf_begin;
}

uint16_t C1Glob::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Glob",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = clock_timeout.data_to_msg(msg, mf_begin);
    mf_begin = initial_vco.data_to_msg(msg, mf_begin);
    mf_begin = gps_backup_power.data_to_msg(msg, mf_begin);
    mf_begin = aux_status_web_flags.data_to_msg(msg, mf_begin);
    mf_begin = gain_pre_amp.data_to_msg(msg, mf_begin);
    mf_begin = linear_phase_filters.data_to_msg(msg, mf_begin);
    mf_begin = input_bitmap.data_to_msg(msg, mf_begin);
    mf_begin = web_server_tcp_port.data_to_msg(msg, mf_begin);
    mf_begin = server_timeout.data_to_msg(msg, mf_begin);
    mf_begin = drift_tolerance.data_to_msg(msg, mf_begin);
    mf_begin = jump_filter.data_to_msg(msg, mf_begin);
    mf_begin = jump_threshold.data_to_msg(msg, mf_begin);
    mf_begin = calibrator_offset.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_active_high.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_map.data_to_msg(msg, mf_begin);
    mf_begin = sampling_phase.data_to_msg(msg, mf_begin);
    mf_begin = gps_cold_start_seconds.data_to_msg(msg, mf_begin);
    mf_begin = kmi_user_tag.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_0_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_1_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_2_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_3_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_4_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_5_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_6_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_freq_bit_7_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_0_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_1_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_2_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_3_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_4_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_5_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_6_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_freq_bit_7_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_0_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_1_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_2_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_3_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_4_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_5_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_6_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_freq_bit_7_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_0_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_1_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_2_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_3_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_4_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_5_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_6_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_freq_bit_7_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_0_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_1_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_2_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_3_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_4_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_5_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_6_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_freq_bit_7_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_0_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_1_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_2_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_3_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_4_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_5_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_6_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_freq_bit_7_scale.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_offset.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_offset.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_offset.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_offset.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_offset.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_offset.data_to_msg(msg, mf_begin);
    mf_begin = channel_1_gain.data_to_msg(msg, mf_begin);
    mf_begin = channel_2_gain.data_to_msg(msg, mf_begin);
    mf_begin = channel_3_gain.data_to_msg(msg, mf_begin);
    mf_begin = channel_4_gain.data_to_msg(msg, mf_begin);
    mf_begin = channel_5_gain.data_to_msg(msg, mf_begin);
    mf_begin = channel_6_gain.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type

    return mf_begin;
}

std::ostream & C1Glob::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_GLOB ---  \n";

    cmd_os << "\nclock_timeout: "; cmd_os << clock_timeout;

    cmd_os << "\ninitial_vco: "; cmd_os << initial_vco;

    cmd_os << "\ngps_backup_power: "; cmd_os << gps_backup_power;

    cmd_os << "\naux_status_web_flags: "; cmd_os << aux_status_web_flags;

    cmd_os << "\ngain_pre_amp: "; cmd_os << gain_pre_amp;

    cmd_os << "\nlinear_phase_filters: "; cmd_os << linear_phase_filters;

    cmd_os << "\ninput_bitmap: "; cmd_os << input_bitmap;

    cmd_os << "\nweb_server_tcp_port: "; cmd_os << web_server_tcp_port;

    cmd_os << "\nserver_timeout: "; cmd_os << server_timeout;

    cmd_os << "\ndrift_tolerance: "; cmd_os << drift_tolerance;

    cmd_os << "\njump_filter: "; cmd_os << jump_filter;

    cmd_os << "\njump_threshold: "; cmd_os << jump_threshold;

    cmd_os << "\ncalibrator_offset: "; cmd_os << calibrator_offset;

    cmd_os << "\nsensor_control_active_high: "; cmd_os << sensor_control_active_high;

    cmd_os << "\nsensor_control_map: "; cmd_os << sensor_control_map;

    cmd_os << "\nsampling_phase: "; cmd_os << sampling_phase;

    cmd_os << "\ngps_cold_start_seconds: "; cmd_os << gps_cold_start_seconds;

    cmd_os << "\nkmi_user_tag: "; cmd_os << kmi_user_tag;

    cmd_os << "\nchannel_1_freq_bit_0_scale: "; cmd_os << channel_1_freq_bit_0_scale;

    cmd_os << "\nchannel_1_freq_bit_1_scale: "; cmd_os << channel_1_freq_bit_1_scale;

    cmd_os << "\nchannel_1_freq_bit_2_scale: "; cmd_os << channel_1_freq_bit_2_scale;

    cmd_os << "\nchannel_1_freq_bit_3_scale: "; cmd_os << channel_1_freq_bit_3_scale;

    cmd_os << "\nchannel_1_freq_bit_4_scale: "; cmd_os << channel_1_freq_bit_4_scale;

    cmd_os << "\nchannel_1_freq_bit_5_scale: "; cmd_os << channel_1_freq_bit_5_scale;

    cmd_os << "\nchannel_1_freq_bit_6_scale: "; cmd_os << channel_1_freq_bit_6_scale;

    cmd_os << "\nchannel_1_freq_bit_7_scale: "; cmd_os << channel_1_freq_bit_7_scale;

    cmd_os << "\nchannel_2_freq_bit_0_scale: "; cmd_os << channel_2_freq_bit_0_scale;

    cmd_os << "\nchannel_2_freq_bit_1_scale: "; cmd_os << channel_2_freq_bit_1_scale;

    cmd_os << "\nchannel_2_freq_bit_2_scale: "; cmd_os << channel_2_freq_bit_2_scale;

    cmd_os << "\nchannel_2_freq_bit_3_scale: "; cmd_os << channel_2_freq_bit_3_scale;

    cmd_os << "\nchannel_2_freq_bit_4_scale: "; cmd_os << channel_2_freq_bit_4_scale;

    cmd_os << "\nchannel_2_freq_bit_5_scale: "; cmd_os << channel_2_freq_bit_5_scale;

    cmd_os << "\nchannel_2_freq_bit_6_scale: "; cmd_os << channel_2_freq_bit_6_scale;

    cmd_os << "\nchannel_2_freq_bit_7_scale: "; cmd_os << channel_2_freq_bit_7_scale;

    cmd_os << "\nchannel_3_freq_bit_0_scale: "; cmd_os << channel_3_freq_bit_0_scale;

    cmd_os << "\nchannel_3_freq_bit_1_scale: "; cmd_os << channel_3_freq_bit_1_scale;

    cmd_os << "\nchannel_3_freq_bit_2_scale: "; cmd_os << channel_3_freq_bit_2_scale;

    cmd_os << "\nchannel_3_freq_bit_3_scale: "; cmd_os << channel_3_freq_bit_3_scale;

    cmd_os << "\nchannel_3_freq_bit_4_scale: "; cmd_os << channel_3_freq_bit_4_scale;

    cmd_os << "\nchannel_3_freq_bit_5_scale: "; cmd_os << channel_3_freq_bit_5_scale;

    cmd_os << "\nchannel_3_freq_bit_6_scale: "; cmd_os << channel_3_freq_bit_6_scale;

    cmd_os << "\nchannel_3_freq_bit_7_scale: "; cmd_os << channel_3_freq_bit_7_scale;

    cmd_os << "\nchannel_4_freq_bit_0_scale: "; cmd_os << channel_4_freq_bit_0_scale;

    cmd_os << "\nchannel_4_freq_bit_1_scale: "; cmd_os << channel_4_freq_bit_1_scale;

    cmd_os << "\nchannel_4_freq_bit_2_scale: "; cmd_os << channel_4_freq_bit_2_scale;

    cmd_os << "\nchannel_4_freq_bit_3_scale: "; cmd_os << channel_4_freq_bit_3_scale;

    cmd_os << "\nchannel_4_freq_bit_4_scale: "; cmd_os << channel_4_freq_bit_4_scale;

    cmd_os << "\nchannel_4_freq_bit_5_scale: "; cmd_os << channel_4_freq_bit_5_scale;

    cmd_os << "\nchannel_4_freq_bit_6_scale: "; cmd_os << channel_4_freq_bit_6_scale;

    cmd_os << "\nchannel_4_freq_bit_7_scale: "; cmd_os << channel_4_freq_bit_7_scale;

    cmd_os << "\nchannel_5_freq_bit_0_scale: "; cmd_os << channel_5_freq_bit_0_scale;

    cmd_os << "\nchannel_5_freq_bit_1_scale: "; cmd_os << channel_5_freq_bit_1_scale;

    cmd_os << "\nchannel_5_freq_bit_2_scale: "; cmd_os << channel_5_freq_bit_2_scale;

    cmd_os << "\nchannel_5_freq_bit_3_scale: "; cmd_os << channel_5_freq_bit_3_scale;

    cmd_os << "\nchannel_5_freq_bit_4_scale: "; cmd_os << channel_5_freq_bit_4_scale;

    cmd_os << "\nchannel_5_freq_bit_5_scale: "; cmd_os << channel_5_freq_bit_5_scale;

    cmd_os << "\nchannel_5_freq_bit_6_scale: "; cmd_os << channel_5_freq_bit_6_scale;

    cmd_os << "\nchannel_5_freq_bit_7_scale: "; cmd_os << channel_5_freq_bit_7_scale;

    cmd_os << "\nchannel_6_freq_bit_0_scale: "; cmd_os << channel_6_freq_bit_0_scale;

    cmd_os << "\nchannel_6_freq_bit_1_scale: "; cmd_os << channel_6_freq_bit_1_scale;

    cmd_os << "\nchannel_6_freq_bit_2_scale: "; cmd_os << channel_6_freq_bit_2_scale;

    cmd_os << "\nchannel_6_freq_bit_3_scale: "; cmd_os << channel_6_freq_bit_3_scale;

    cmd_os << "\nchannel_6_freq_bit_4_scale: "; cmd_os << channel_6_freq_bit_4_scale;

    cmd_os << "\nchannel_6_freq_bit_5_scale: "; cmd_os << channel_6_freq_bit_5_scale;

    cmd_os << "\nchannel_6_freq_bit_6_scale: "; cmd_os << channel_6_freq_bit_6_scale;

    cmd_os << "\nchannel_6_freq_bit_7_scale: "; cmd_os << channel_6_freq_bit_7_scale;

    cmd_os << "\nchannel_1_offset: "; cmd_os << channel_1_offset;

    cmd_os << "\nchannel_2_offset: "; cmd_os << channel_2_offset;

    cmd_os << "\nchannel_3_offset: "; cmd_os << channel_3_offset;

    cmd_os << "\nchannel_4_offset: "; cmd_os << channel_4_offset;

    cmd_os << "\nchannel_5_offset: "; cmd_os << channel_5_offset;

    cmd_os << "\nchannel_6_offset: "; cmd_os << channel_6_offset;

    cmd_os << "\nchannel_1_gain: "; cmd_os << channel_1_gain;

    cmd_os << "\nchannel_2_gain: "; cmd_os << channel_2_gain;

    cmd_os << "\nchannel_3_gain: "; cmd_os << channel_3_gain;

    cmd_os << "\nchannel_4_gain: "; cmd_os << channel_4_gain;

    cmd_os << "\nchannel_5_gain: "; cmd_os << channel_5_gain;

    cmd_os << "\nchannel_6_gain: "; cmd_os << channel_6_gain;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace