// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_H
#define _MZN_CMD_FIELD_H

#include <iostream>
#include <vector>
#include <bitset>
#include <array>

#include "mzn_except.h"

namespace mzn {

//! CmdField temporary type
/*! During the msg_to_data and data_to_msg in CmdField, the data type might
    temporarily be cast to an unsigned int for endianness correction operations
    (except for array/vector types which do not use this).
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename DataType>
struct CmdFieldTempType {using type = DataType;};

//! template specialization for special cases
// -------------------------------------------------------------------------- //
template<> struct CmdFieldTempType<float>{using type = uint32_t;};
template<> struct CmdFieldTempType<bool>{using type = uint32_t;};

//! return type of std::bitset<N>::to_ulong()
template<> struct CmdFieldTempType<std::bitset<8>> {using type = unsigned long;};
template<> struct CmdFieldTempType<std::bitset<16>>{using type = unsigned long;};
template<> struct CmdFieldTempType<std::bitset<32>>{using type = unsigned long;};

//! cast to the cmd_field temporary type
// -------------------------------------------------------------------------- //
template <typename T>
typename CmdFieldTempType<T>::type
cmd_field_temp_cast(T data) {
    return static_cast<typename CmdFieldTempType<T>::type>(data);
}

// -------------------------------------------------------------------------- //
template <std::size_t Nbits>
typename CmdFieldTempType< std::bitset<Nbits>  >::type
cmd_field_temp_cast(std::bitset<Nbits> data) {
    return data.to_ulong();
}

//! Holds the data representing a Command variable.
/*! Command classes are composed of CmdField classes.

    Each CmdField can serialize/deserialize itself.

    All CmdField are forward declared for the current types
    needed in the digitizer manual.

    For some cases N != sizeof(T), i.e.: integers that take
    6 bytes in the msg (N=6) but are stored with int64_t
    (sizeof(int64_t) = 8) hence the need for 2 independent
    template parameters.

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    IMPORTANT: msg_to_data / data_to_msg do not check the size of the msg.
               this is must be done at the data structure level that holds
               the cmd_field(s). This is done for performance, since it only needs
               to be checked once in a full command msg_to_data / data_to_msg.
               In manzano, this is always cheched at the command.

    @see Command
    @tparam T data_ type.
    @tparam N number of bytes the data_ takes in the msg.
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename T, std::size_t N = sizeof(T)>
class CmdField {

protected:

    T data_;

public:

    // message type
    using M = std::vector<uint8_t>;
    using data_type = T;

    //! TODO: remove n from here? document where it is being used
    std::size_t const n = N;

    //! used only in operator<<
    //! TODO: units should be done with template meta-programming
    //! compile time and conversions (better with C++17)
    std::string units;

    //! Initialize data to zeros (including array).
    explicit
    CmdField(std::string const in_units = "");

    ~CmdField() = default;

    // --------------------------------------------------------------------- //
    CmdField(CmdField && rhs) = default;
    CmdField & operator=(CmdField && rhs) = default;
    CmdField & operator=(CmdField const & rhs) = default;
    CmdField(CmdField const & rhs) = default;

    //! Reads the corresponding msg bytes and saves it in data.
    /*! @param msg the message formatted according to digitizer manual
                   in network byte order.
        @param mf_pos where to start reading the msg for this cmd_field.
        @return mf_pos for the next msg_to_data. */
    std::size_t msg_to_data(M const & msg, std::size_t const mf_pos);

    //! Saves the data to the corresponding msg bytes.
    /*! @param msg the message formatted according to digitizer manual
                   in network byte order.
        @param mf_pos where to start reading the msg for this cmd_field.
        @return mf_pos for the next msg_to_data. */
    std::size_t data_to_msg(M & msg, std::size_t const mf_pos) const;

    //! do not overload, use operator() for overloads
    T const & data() const {return data_;}

    //! avoid unless necessary
    void data(T const data) {data_ = data;};

    //! Used as the default accessor/mutator
    /*! Inherited CmdField overload this function when is different than
        accessing the plain data field. */
    T operator() () const { return data_; }
    void operator() (T const & data) { data_ = data; }
};

//! Constructor
// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
CmdField<T, N>::CmdField(std::string const in_units /* = 0 */) :
        data_{},
        units{in_units} {};

//! Overload operator==
// -------------------------------------------------------------------------- //
template <typename osT, std::size_t osN>
inline
bool operator==(CmdField<osT, osN> const & lhs, CmdField<osT, osN> const & rhs) {

    return ( lhs.data() == rhs.data() ) and (lhs.units == rhs.units);
}

//! Overload operator!=
// -------------------------------------------------------------------------- //
template <typename osT, std::size_t osN>
inline
bool operator!=(CmdField<osT, osN> const & lhs, CmdField<osT, osN> const & rhs) {

    return not (lhs == rhs);
}

//! Implementation of operator<<
//! general case
// -------------------------------------------------------------------------- //
template <typename osT, std::size_t osN>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdField<osT, osN> const & cf) {
    cf_os << cf.data();
    if ( not cf.units.empty() ) cf_os << " " << cf.units;
    return cf_os;
}

// C1_stat -> pll_running has 2 bytes, resulting in a 2 byte boolean cf
// -------------------------------------------------------------------------- //
template <>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdField<bool> const & cf) {

    cf_os << std::boolalpha << cf() << std::noboolalpha;
    return cf_os;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdField<uint8_t> const & cf) {

    cf_os << static_cast<unsigned int>( cf.data() );
    return cf_os;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::ostream & operator<<(std::ostream & cf_os,
                          CmdField<int8_t> const & cf) {

    cf_os << static_cast<int>( cf.data() );
    return cf_os;
}

// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
inline
std::size_t CmdField<T, N>::msg_to_data(M const & msg,
                                        std::size_t const mf_pos) {

    using Ttemp = typename CmdFieldTempType<T>::type;

    Ttemp temp_data{0};

    for (auto i = mf_pos; i < mf_pos + N; i++ ) {
        temp_data = (temp_data << 8) | msg[i];
    }

    // should be no cost if Ttemp == T
    data_ = static_cast<T>(temp_data);

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <typename T, std::size_t N>
inline
std::size_t CmdField<T, N>::data_to_msg(M & msg,
                                        std::size_t const mf_pos) const {

    // temp_data is a copy casted to a type that can use bitwise left/right
    // shift operator.
    auto const temp_data = cmd_field_temp_cast(data_);

    auto byte_index = 0;
    for (auto i = mf_pos + N; i-- > mf_pos; byte_index++) {
        msg[i] = static_cast<uint8_t>( ( temp_data >> (8 * byte_index) ) & 0xFF );
    }

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::size_t CmdField<float, 4>::msg_to_data(M const & msg,
                                            std::size_t const mf_pos) {

    using T = float;
    auto constexpr N = 4;

    using Ttemp = typename CmdFieldTempType<T>::type;

    Ttemp temp_data{0};

    for (auto i = mf_pos; i < mf_pos + N; i++ ) {
        temp_data = (temp_data << 8) | msg[i];
    }

    // need to use reference or pointer for reinterpret cast
    // look at the bits of the unsigned int number as if it was a float
    auto & ref_temp_data = reinterpret_cast<T &>(temp_data);
    data_ = ref_temp_data;

    return mf_pos + N;
}

// -------------------------------------------------------------------------- //
template <>
inline
std::size_t CmdField<float, 4>::data_to_msg(M & msg,
                                            std::size_t const mf_pos) const {

    using T = float;
    auto constexpr N = 4;

    using Ttemp = typename CmdFieldTempType<T>::type;

    // look at the bits of the float number as if it was an unsigned int
    auto const temp_data = data_;
    auto const & ref_temp_data = reinterpret_cast<Ttemp const &>(temp_data);

    auto byte_index = 0;
    for (auto i = mf_pos + N; i-- > mf_pos; byte_index++) {
        msg[i] = static_cast<uint8_t>( ( ref_temp_data >> (8 * byte_index) ) & 0xFF );
    }

    return mf_pos + N;
}

//! utility
// -------------------------------------------------------------------------- //
template <typename T>
struct IsStdArray {bool constexpr static value = false;};
template <typename T, std::size_t N>
struct IsStdArray<std::array<T, N>> {bool constexpr static value = true;};
// -------------------------------------------------------------------------- //
template <typename T>
struct IsStdBitset {bool constexpr static value = false;};
template <std::size_t N>
struct IsStdBitset<std::bitset<N>> {bool constexpr static value = true;};

} // << mzn

#endif
