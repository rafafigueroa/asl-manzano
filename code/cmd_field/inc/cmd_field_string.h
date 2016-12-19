// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_STRING_H
#define _MZN_CMD_FIELD_STRING_H

#include "cmd_field.h"

namespace mzn {

//! String of T, printed as is
//! TODO: fix for sizeof(T) > 1
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
class CmdFieldString : public CmdField< std::string<T, N>, N > {

    static_assert(sizeof(T)==1, "CmdFieldString for sizeof(T) is in progress");
public:

    using data_type = typename std::string<T, N>;
    using M = typename CmdField<data_type>::M;

    //! initializes base
    explicit
    CmdFieldString();

    ~CmdFieldString() = default;

    //! same signature as CmdField
    std::size_t msg_to_data(M const & msg, std::size_t const mf_pos);
    std::size_t data_to_msg(M & msg, std::size_t const mf_pos) const;
};

// constructor
// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
CmdFieldString<T, N>::CmdFieldString() : CmdField<data_type, N>{} {};


// -------------------------------------------------------------------------- //
template <typename osT, std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdFieldString<osT, osN> const & cf) {

    cf_os << "[";
    for (auto const & b : cf.data() ) cf_os << b << " ";
    cf_os << "]";
    return cf_os;
}

// once std::string is satisfies ContiguousContainer in C++17
// then reinterpret_cast can be used safely
// or use std::copy_n with parallel execution policy?
// currently all the strings are really small
// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
std::size_t CmdFieldString<T, N>::msg_to_data(M const & msg,
                                             std::size_t const mf_pos) {

    // for std::string<T, N> N is the number of elements
    // for CmdField<T, N> N is the number of bytes the data takes in the msg
    // when using byte/ubyte strings, these two Ns are the same

    // this will separate both variables.
    // Note that N_bytes == N when sizof(N) == 1
    // TODO: implement this, possibly 2 loops
    // auto constexpr N_size = sizeof(N);
    // auto constexpr N_bytes = N * N_size;

    // no temporary objects, only cast
    auto data_index = 0;
    using DataType = typename data_type::value_type;

    for (auto i = mf_pos; i < mf_pos + N; i++) {
        this->data_[data_index] = static_cast<DataType>(msg[i]);
        data_index++;
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
std::size_t CmdFieldString<T, N>::data_to_msg(M & msg,
                                             std::size_t const mf_pos) const {

    auto data_index = 0;
    for (auto i = mf_pos; i < mf_pos + N; i++) {
        msg[i] = static_cast<typename M::value_type>(this->data_[data_index]);
        data_index++;
    }

    return mf_pos + N;
}
} // << mzn

#endif
