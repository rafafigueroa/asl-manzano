// ** DataStore Object ** //
// Manzano software

#ifndef _MZN_MESSAGE_H_
#define _MZN_MESSAGE_H_

#include <vector>
#include <cstdint>
#include <iostream>

namespace mzn {

namespace Qdp {using Message = std::vector<uint8_t>;}

//! prints message types as integers on base 16
// -------------------------------------------------------------------------- //
template<typename M>
inline
void stream_msg(M const & msg, std::ostream & os = std::cout) {

    os << std::uppercase << std::showbase << std::hex << "\n{";
    for (auto const & b : msg) os << static_cast<const int>(b) << ", ";
    os << "}\n" << std::noshowbase << std::uppercase << std::dec;
}

} // <- mzn



#endif // _MZN_MESSAGE_H_
