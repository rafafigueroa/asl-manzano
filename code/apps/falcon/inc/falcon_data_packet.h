#ifndef _MZN_FALCON_DATA_PACKET_H_
#define _MZN_FALCON_DATA_PACKET_H_

#include "falcon_header.h"
#include "minute_time_point.h"

//! all data expected to be in minute intervals
// -------------------------------------------------------------------------- //
struct FalconDataPacket {

    FalconHeader falcon_header;

    MinuteTimePoint mtp;

    // raw input takes integer data
    std::vector<int32_t> data;

    // to the raw input format
    std::string to_raw_input_format() const;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, FalconDataPacket const & fdp) {

    os << fdp.falcon_header;

    os << fdp.mtp;

    for (auto const & d : fdp.data) os << d << " ";

    os << "]" << " size(" << fdp.data.size() << ")";

    return os;
}

#endif // _MZN_FALCON_DATA_PACKET_H_
