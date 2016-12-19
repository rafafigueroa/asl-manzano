// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_hex.h"
namespace mzn {

template <typename T>
CmdFieldHex<T>::CmdFieldHex() : CmdField<T>{} {};

//! for Qdp_header -> cmd_number
template class CmdFieldHex<uint8_t>;

//! for C1_pollsn -> serial_number_mask
template class CmdFieldHex<uint16_t>;

template class CmdFieldHex<uint32_t>;
template class CmdFieldHex<uint64_t>;

}// << mzn

