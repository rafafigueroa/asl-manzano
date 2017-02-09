#include "c1_qcal.h"

namespace mzn {
C1Qcal::C1Qcal():
    Command(0x23, 36),
    starting_time(),
    waveform(),
    amplitude(),
    cal_duration(),
    settling_time(),
    calibration_bitmap(),
    trailer_time(),
    sensor_control_enable(),
    monitor_channel_bitmap(),
    frequency_divider(),
    spare_0(),
    coupling_bytes() { }

uint16_t C1Qcal::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Qcal",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_time.msg_to_data(msg, mf_begin);
    mf_begin = waveform.msg_to_data(msg, mf_begin);
    mf_begin = amplitude.msg_to_data(msg, mf_begin);
    mf_begin = cal_duration.msg_to_data(msg, mf_begin);
    mf_begin = settling_time.msg_to_data(msg, mf_begin);
    mf_begin = calibration_bitmap.msg_to_data(msg, mf_begin);
    mf_begin = trailer_time.msg_to_data(msg, mf_begin);
    mf_begin = sensor_control_enable.msg_to_data(msg, mf_begin);
    mf_begin = monitor_channel_bitmap.msg_to_data(msg, mf_begin);
    mf_begin = frequency_divider.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = coupling_bytes.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Qcal::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Qcal",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = starting_time.data_to_msg(msg, mf_begin);
    mf_begin = waveform.data_to_msg(msg, mf_begin);
    mf_begin = amplitude.data_to_msg(msg, mf_begin);
    mf_begin = cal_duration.data_to_msg(msg, mf_begin);
    mf_begin = settling_time.data_to_msg(msg, mf_begin);
    mf_begin = calibration_bitmap.data_to_msg(msg, mf_begin);
    mf_begin = trailer_time.data_to_msg(msg, mf_begin);
    mf_begin = sensor_control_enable.data_to_msg(msg, mf_begin);
    mf_begin = monitor_channel_bitmap.data_to_msg(msg, mf_begin);
    mf_begin = frequency_divider.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = coupling_bytes.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Qcal::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_QCAL ---  \n";

    cmd_os << "\nstarting_time: "; cmd_os << starting_time;

    cmd_os << "\nwaveform: "; cmd_os << waveform;

    cmd_os << "\namplitude: "; cmd_os << amplitude;

    cmd_os << "\ncal_duration: "; cmd_os << cal_duration;

    cmd_os << "\nsettling_time: "; cmd_os << settling_time;

    cmd_os << "\ncalibration_bitmap: "; cmd_os << calibration_bitmap;

    cmd_os << "\ntrailer_time: "; cmd_os << trailer_time;

    cmd_os << "\nsensor_control_enable: "; cmd_os << sensor_control_enable;

    cmd_os << "\nmonitor_channel_bitmap: "; cmd_os << monitor_channel_bitmap;

    cmd_os << "\nfrequency_divider: "; cmd_os << frequency_divider;

    cmd_os << "\ncoupling_bytes: "; cmd_os << coupling_bytes;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace