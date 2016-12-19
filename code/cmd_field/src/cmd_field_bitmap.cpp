// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_bitmap.h"
namespace mzn {

// -------------------------------------------------------------------------- //
template <std::size_t N>
CmdFieldBitmap<N>::CmdFieldBitmap() : CmdField<data_type, N>{} {};

// <bitset<N*8>, N> same as with <T, N> except for:
// - cf_data is set to unsigned long
// - a bitset<N*8> is created at the end
// same for N = 1, 2, 4

template class CmdFieldBitmap<1>;
template class CmdFieldBitmap<2>;
template class CmdFieldBitmap<4>;

}// << mzn

