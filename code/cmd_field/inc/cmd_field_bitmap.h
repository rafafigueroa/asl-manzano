// ** CmdField Class ** //
// Manzano software

#ifndef _MZN_CMD_FIELD_BItypeMAP_H
#define _MZN_CMD_FIELD_BItypeMAP_H

#include "cmd_field.h"

namespace mzn {

//! Bitset is only used as the base of all the bitset types.
/*! typehe functions here are utilities for the bitset types.
    bitset types are built in auto_generation and documentation
    of bitset architecture design can be found there.

    template parameter must be given to CmdField or else it will take
    sizeof(std::bitset<number_of_bits>) which can be different than N,
    since includes
    extra space to make to the full integer number, an 8 bit bitset
    std::bitset<8> is stored as 4 byte number

    @author rfigueroa@usgs.gov
 */

template <std::size_t N>
class CmdFieldBitmap : public CmdField< std::bitset<N*8>, N > {

public:

    using data_type = typename std::bitset<N*8>;
    using CmdField<data_type, N>::data_;

    //! initializes base
    explicit
    CmdFieldBitmap();

    ~CmdFieldBitmap() = default;

    /*! Used for operator<< of bool answers,
      e.g., for false: bool_property [ ]
      e.g., for true:  bool_property [*] */
    // --------------------------------------------------------------------- //
    char bool_indicator(bool const b) const { return (b) ? '*' : ' '; }

    //! bitset as unsigned long, use codes
    // --------------------------------------------------------------------- //
    unsigned long raw_value() const {
        return data_.to_ulong();
    }

    /*! Same as raw_value but only evaluating a part of the bitset.
        in addition to the use for codes, some bitsets have actual
        values embedded. */
    // --------------------------------------------------------------------- //
    unsigned long raw_value_from_range(unsigned int const bmv_val_begin,
                                       unsigned int const bmv_val_mask) const {

        return (data_.to_ulong() >> bmv_val_begin) & bmv_val_mask;
    }

    // --------------------------------------------------------------------- //
    void set_raw_value_in_range(unsigned int const bmv_val_begin,
                                unsigned long const bmv_val_mask,
                                unsigned long const bmv_val_raw) {

        // put the raw value in the right range
        // the & with the mask is just in case the value is bigger than the mask
        data_type const temp_raw_value(
            (bmv_val_raw & bmv_val_mask) << bmv_val_begin );

        data_type mask(bmv_val_mask);

        // reset the bits corresponding to the range ( given by mask/begin)
        // and put the raw value in the range
        data_ = ( data_ & (mask.flip() << bmv_val_begin) ) | temp_raw_value;
    }

};

//! no special ostream<< operator. Using either the default from CmdField
//! or the ones created from the derived classes of CmdFieldBitmap

} // << mzn

#endif
