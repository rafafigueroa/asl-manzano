#include "cx_pll_status.h"

namespace mzn {
CxPllStatus::CxPllStatus():
    Command(0, 28),
    initial_vco(),
    time_error(),
    rms_vco(),
    best_vco(),
    spare_0(),
    ticks_since_last_update(),
    km(),
    pll_state() { }

uint16_t CxPllStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                  uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxPllStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = initial_vco.msg_to_data(msg, mf_begin);
    mf_begin = time_error.msg_to_data(msg, mf_begin);
    mf_begin = rms_vco.msg_to_data(msg, mf_begin);
    mf_begin = best_vco.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = ticks_since_last_update.msg_to_data(msg, mf_begin);
    mf_begin = km.msg_to_data(msg, mf_begin);
    mf_begin = pll_state.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CxPllStatus::data_to_msg(std::vector<uint8_t> & msg,
                                  uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CxPllStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = initial_vco.data_to_msg(msg, mf_begin);
    mf_begin = time_error.data_to_msg(msg, mf_begin);
    mf_begin = rms_vco.data_to_msg(msg, mf_begin);
    mf_begin = best_vco.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = ticks_since_last_update.data_to_msg(msg, mf_begin);
    mf_begin = km.data_to_msg(msg, mf_begin);
    mf_begin = pll_state.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CxPllStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CX_PLL_STATUS ---  \n";

    cmd_os << "\ninitial_vco: "; cmd_os << initial_vco;

    cmd_os << "\ntime_error: "; cmd_os << time_error;

    cmd_os << "\nrms_vco: "; cmd_os << rms_vco;

    cmd_os << "\nbest_vco: "; cmd_os << best_vco;

    cmd_os << "\nticks_since_last_update: "; cmd_os << ticks_since_last_update;

    cmd_os << "\nkm: "; cmd_os << km;

    cmd_os << "\npll_state: "; cmd_os << pll_state;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace