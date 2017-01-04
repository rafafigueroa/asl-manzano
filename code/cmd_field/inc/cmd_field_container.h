// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_CONTAINER_H
#define _MZN_CMD_FIELD_CONTAINER_H

#include "cmd_field.h"

namespace mzn {

//! The most basic type of command, a single container of CmdField<T>
/*! @author rfigueroa@usgs.gov */
// -------------------------------------------------------------------------- //
template <typename cf_T>
class CmdFieldContainer {

    using T = CmdField<cf_T>;
    // CmdField container
    std::vector<T> data_;

    // so sizeof(T) is the CmdField N, in general cmd_field.n
    std::size_t const cf_n_;


public:

    // message type
    using M = std::vector<uint8_t>;

    explicit
    CmdFieldContainer() : cf_n_{sizeof(cf_T)} {};

    ~CmdFieldContainer() = default;

    //! same signature as CmdField
    std::size_t msg_to_data(M const & msg, std::size_t const mf_begin = 0);
    std::size_t data_to_msg(M & msg, std::size_t const mf_begin = 0) const;

    //! consistency with commands
    std::size_t const cmd_data_size() const {return cf_n_ * data_.size();}

    //! do not overload, use operator() for overloads
    std::vector<T> const & data() const {return data_;}

    //! Used as the default accessor/mutator
    /*! Inherited CmdField overload this function when is different than
        accessing the plain data field. */
    std::vector<T> operator() () const {return data_;}
    void operator() (std::vector<T> const & data) {data_ = data;}

    //! to add values to the internal vector from the internal type of the cf
    void push_back(cf_T const & value) {
        T cf;
        cf(value);
        data_.push_back(cf);
    }

    //! internal one
    std::size_t size() const {return data_.size();}
};

// -------------------------------------------------------------------------- //
template <typename os_cf_T>
inline
std::ostream & operator<<(std::ostream & cfc_os,
                          CmdFieldContainer<os_cf_T> const & cfc) {

    for ( auto const & cf : cfc.data() ) {
        cfc_os << cf.data();
        cfc_os << " ";
    }

    return cfc_os;
}

// -------------------------------------------------------------------------- //
template <typename cf_T>
std::size_t
CmdFieldContainer<cf_T>::msg_to_data(M const & msg,
                                     std::size_t const mf_begin) {


    // would need lock if multi threaded operations
    // total number of bytes in this container
    auto const & n = cmd_data_size();

    if (msg.size() < mf_begin + n) {
        throw FatalException( "CmdFieldContainer",
                              "msg_to_data",
                              "msg size " + std::to_string( msg.size() )
                              + ", mf_begin " + std::to_string(mf_begin)
                              + ", n " + std::to_string(n) );
    }

    // the starting position for the internal data
    auto mf_pos = mf_begin;

    for (auto const & d : data_) {
        d.msg_to_data(msg, mf_pos);
        mf_pos += cf_n_;
    }

    return mf_begin + n;
}

// -------------------------------------------------------------------------- //
template <typename cf_T>
std::size_t
CmdFieldContainer<cf_T>::data_to_msg(M & msg,
                                     std::size_t const mf_begin) const {

    // would need lock if multi threaded operations
    // total number of bytes in this container
    auto const & n = cmd_data_size();

    if (msg.size() < mf_begin + n) {
        throw FatalException( "CmdFieldContainer",
                              "data_to_msg",
                              "msg size " + std::to_string( msg.size() )
                              + ", mf_begin " + std::to_string(mf_begin)
                              + ", n " + std::to_string(n) );
    }

    // the starting position for the internal data
    auto mf_pos = mf_begin;

    for (auto const & d : data_) {
        d.data_to_msg(msg, mf_pos);
        mf_pos += cf_n_;
    }

    return mf_begin + n;
}

} // << mzn

#endif
