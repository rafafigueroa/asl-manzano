// ** E300 Object ** //
// Manzano software

#ifndef _MZN_E300_H
#define _MZN_E300_H

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>


namespace mzn {

//! Holds configuration and status of e300 port
/*!
    In addition, has a unique udp connection
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //

class E300 {

public:
    enum class Instruction {
        status,
        enable,
        calibrate,
        ext_cal_connect,
    };

    E300() = default;
    ~E300() = default;

    template <Instruction ei>
    std::string ei_str();
};

template <>
std::string E300::ei_str<E300::Instruction::status>() {return "status";}

} // end mzn
#endif // _MZN_E300_H_
