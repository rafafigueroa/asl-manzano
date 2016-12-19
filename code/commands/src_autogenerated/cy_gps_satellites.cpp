#include "cy_gps_satellites.h"

namespace mzn {
CyGpsSatellites::CyGpsSatellites():
    Command(0, 8),
    satellite_number(),
    elevation(),
    azimuth(),
    snr() { }

uint16_t CyGpsSatellites::msg_to_data(std::vector<uint8_t> const & msg,
                                      uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyGpsSatellites",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = satellite_number.msg_to_data(msg, mf_begin);
    mf_begin = elevation.msg_to_data(msg, mf_begin);
    mf_begin = azimuth.msg_to_data(msg, mf_begin);
    mf_begin = snr.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CyGpsSatellites::data_to_msg(std::vector<uint8_t> & msg,
                                      uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyGpsSatellites",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = satellite_number.data_to_msg(msg, mf_begin);
    mf_begin = elevation.data_to_msg(msg, mf_begin);
    mf_begin = azimuth.data_to_msg(msg, mf_begin);
    mf_begin = snr.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CyGpsSatellites::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CY_GPS_SATELLITES ---  \n";

    cmd_os << "\nsatellite_number: "; cmd_os << satellite_number;

    cmd_os << "\nelevation: "; cmd_os << elevation;

    cmd_os << "\nazimuth: "; cmd_os << azimuth;

    cmd_os << "\nsnr: "; cmd_os << snr;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace