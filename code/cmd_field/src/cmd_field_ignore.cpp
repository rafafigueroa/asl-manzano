// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_ignore.h"
namespace mzn {

template <typename T>
CmdFieldIgnore<T>::CmdFieldIgnore() : CmdField<T>{} {};

//! for C1_ack -> no parameters
//template class CmdFieldIgnore<uint8_t>;

//! for Qdp_header -> cmd_number
template class CmdFieldIgnore<uint8_t>;

//! for C1_pollsn -> serial_number_mask
template class CmdFieldIgnore<uint16_t>;

template class CmdFieldIgnore<uint32_t>;
template class CmdFieldIgnore<uint64_t>;

}// << mzn

