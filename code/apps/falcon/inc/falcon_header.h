#ifndef _MZN_FALCON_HEADER_H_
#define _MZN_FALCON_HEADER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "date.h"

//! header data that could be the same of several FalconData
// -------------------------------------------------------------------------- //
struct FalconHeader {

    // should be the same on all
    std::string name = "";
    // specific to this data
    std::string channel = ""; // falcon channel, not seed format channel
    std::string code = "";
    std::string description = "";
    std::string unit_of_measure = "";
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, FalconHeader const & fh) {

    os << "\nname: " << fh.name
       << "\nchan: " << fh.channel
       << "\ncode: " << fh.code
       << "\ndesc: " << fh.description
       << "\nunit: " << fh.unit_of_measure;

    return os;
}

#endif // _MZN_FALCON_HEADER_H_
