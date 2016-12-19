#include "cx_gps_status.h"

namespace mzn {
CxGpsStatus::CxGpsStatus():
    Command(0, 84),
    gps_power_on_off_time(),
    gps_on(),
    number_of_satellites_used(),
    number_of_satellites_in_view(),
    gps_time(),
    gps_date(),
    gps_fix(),
    gps_height(),
    gps_latitude(),
    gps_longitude(),
    time_of_last_good_1pps(),
    total_checksum_errors() { }

uint16_t CxGpsStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxGpsStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = gps_power_on_off_time.msg_to_data(msg, mf_begin);
    mf_begin = gps_on.msg_to_data(msg, mf_begin);
    mf_begin = number_of_satellites_used.msg_to_data(msg, mf_begin);
    mf_begin = number_of_satellites_in_view.msg_to_data(msg, mf_begin);
    mf_begin = gps_time.msg_to_data(msg, mf_begin);
    mf_begin = gps_date.msg_to_data(msg, mf_begin);
    mf_begin = gps_fix.msg_to_data(msg, mf_begin);
    mf_begin = gps_height.msg_to_data(msg, mf_begin);
    mf_begin = gps_latitude.msg_to_data(msg, mf_begin);
    mf_begin = gps_longitude.msg_to_data(msg, mf_begin);
    mf_begin = time_of_last_good_1pps.msg_to_data(msg, mf_begin);
    mf_begin = total_checksum_errors.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxGpsStatus::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxGpsStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = gps_power_on_off_time.data_to_msg(msg, mf_begin);
    mf_begin = gps_on.data_to_msg(msg, mf_begin);
    mf_begin = number_of_satellites_used.data_to_msg(msg, mf_begin);
    mf_begin = number_of_satellites_in_view.data_to_msg(msg, mf_begin);
    mf_begin = gps_time.data_to_msg(msg, mf_begin);
    mf_begin = gps_date.data_to_msg(msg, mf_begin);
    mf_begin = gps_fix.data_to_msg(msg, mf_begin);
    mf_begin = gps_height.data_to_msg(msg, mf_begin);
    mf_begin = gps_latitude.data_to_msg(msg, mf_begin);
    mf_begin = gps_longitude.data_to_msg(msg, mf_begin);
    mf_begin = time_of_last_good_1pps.data_to_msg(msg, mf_begin);
    mf_begin = total_checksum_errors.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxGpsStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_GPS_STATUS ---  \n";

    cmd_os << "\ngps_power_on_off_time: "; cmd_os << gps_power_on_off_time;

    cmd_os << "\ngps_on: "; cmd_os << gps_on;

    cmd_os << "\nnumber_of_satellites_used: "; cmd_os << number_of_satellites_used;

    cmd_os << "\nnumber_of_satellites_in_view: "; cmd_os << number_of_satellites_in_view;

    cmd_os << "\ngps_time: "; cmd_os << gps_time;

    cmd_os << "\ngps_date: "; cmd_os << gps_date;

    cmd_os << "\ngps_fix: "; cmd_os << gps_fix;

    cmd_os << "\ngps_height: "; cmd_os << gps_height;

    cmd_os << "\ngps_latitude: "; cmd_os << gps_latitude;

    cmd_os << "\ngps_longitude: "; cmd_os << gps_longitude;

    cmd_os << "\ntime_of_last_good_1pps: "; cmd_os << time_of_last_good_1pps;

    cmd_os << "\ntotal_checksum_errors: "; cmd_os << total_checksum_errors;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace