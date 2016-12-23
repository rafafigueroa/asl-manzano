// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_ARRAY_H
#define _MZN_CMD_FIELD_ARRAY_H

#include "cmd_field.h"

namespace mzn {

//! CmdField specialization for T = std::array<array_T, array_N>
/*  @tparam array_T template parameter for std::array<T,N>
    @tparam array_N template parameter for std::array<T,N>
    if sizeof(array_T) == 1, CmdField N == array_N
    TODO: add flexibility for sizeof(array_T) > 1
    @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <typename array_T, std::size_t array_N, std::size_t N = array_N>
class CmdFieldArray : public CmdField<std::array<array_T, array_N>, N> {

    static_assert(sizeof(array_T)==1,
                  "CmdFieldArray for sizeof(array_T) != 1 not  implemented");
public:

    using data_type = typename std::array<array_T, array_N>;
    using M = typename CmdField<data_type>::M;

    //! initializes base
    explicit
    CmdFieldArray() : CmdField<data_type, N>{} {}

    ~CmdFieldArray() = default;

    //! same signature as CmdField
    std::size_t msg_to_data(M const & msg, std::size_t const mf_pos);
    std::size_t data_to_msg(M & msg, std::size_t const mf_pos) const;
};

// -------------------------------------------------------------------------- //
template <typename osT, std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldArray<osT, osN> const & cf) {
    cf_os << "[";
    for ( auto const & b : cf.data() ) cf_os << b << " ";
    cf_os << "]";
    return cf_os;
}

// once std::array is satisfies ContiguousContainer in C++17
// then reinterpret_cast can be used safely
// or use std::copy_n with parallel execution policy?
// currently all the arrays are really small
// -------------------------------------------------------------------------- //
template <typename array_T, std::size_t array_N, std::size_t N>
std::size_t
CmdFieldArray<array_T, array_N, N>::msg_to_data(M const & msg,
                                                std::size_t const mf_pos) {
    // no temporary objects, only cast
    auto data_index = 0;
    using DataType = typename data_type::value_type;

    for (auto i = mf_pos; i < mf_pos + array_N; i++) {
        this->data_[data_index] = static_cast<DataType>(msg[i]);
        data_index++;
    }

    return mf_pos + array_N;

    // TODO: implement this for sizeof(T)>1, possibly 2 loops
    // when using byte/ubyte arrays (sizeof(array_T)==1)
    // if not, endianness corrections will have to be made to the array elements
    // C1_Glob channel_frequency could benefit from this, currently they are
    // described as separate variables due to this limitation
}

// -------------------------------------------------------------------------- //
template <typename array_T, std::size_t array_N, std::size_t N >
std::size_t
CmdFieldArray<array_T, array_N, N>::data_to_msg(M & msg,
                                                std::size_t const mf_pos) const {
    auto data_index = 0;
    for (auto i = mf_pos; i < mf_pos + array_N; i++) {
        msg[i] = static_cast<typename M::value_type>(this->data_[data_index]);
        data_index++;
    }

    return mf_pos + array_N;
}
} // << mzn

#endif
