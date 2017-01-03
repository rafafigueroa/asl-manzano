#include "f1_raw_input.h"

namespace mzn {
F1RawInput::F1RawInput():
    Command(0x0, 40),
    endian_check(),
    number_of_samples(),
    seed_name(),
    year(),
    doy(),
    sample_rate_factor(),
    sample_rate_multiplier(),
    activity_flags(),
    io_and_clock_flags(),
    data_quality_flags(),
    timing_quality(),
    sec(),
    usec(),
    sequence_number() { }

uint16_t F1RawInput::msg_to_data(std::vector<uint8_t> const & msg,
                                 uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "F1RawInput",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = endian_check.msg_to_data(msg, mf_begin);
    mf_begin = number_of_samples.msg_to_data(msg, mf_begin);
    mf_begin = seed_name.msg_to_data(msg, mf_begin);
    mf_begin = year.msg_to_data(msg, mf_begin);
    mf_begin = doy.msg_to_data(msg, mf_begin);
    mf_begin = sample_rate_factor.msg_to_data(msg, mf_begin);
    mf_begin = sample_rate_multiplier.msg_to_data(msg, mf_begin);
    mf_begin = activity_flags.msg_to_data(msg, mf_begin);
    mf_begin = io_and_clock_flags.msg_to_data(msg, mf_begin);
    mf_begin = data_quality_flags.msg_to_data(msg, mf_begin);
    mf_begin = timing_quality.msg_to_data(msg, mf_begin);
    mf_begin = sec.msg_to_data(msg, mf_begin);
    mf_begin = usec.msg_to_data(msg, mf_begin);
    mf_begin = sequence_number.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t F1RawInput::data_to_msg(std::vector<uint8_t> & msg,
                                 uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "F1RawInput",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = endian_check.data_to_msg(msg, mf_begin);
    mf_begin = number_of_samples.data_to_msg(msg, mf_begin);
    mf_begin = seed_name.data_to_msg(msg, mf_begin);
    mf_begin = year.data_to_msg(msg, mf_begin);
    mf_begin = doy.data_to_msg(msg, mf_begin);
    mf_begin = sample_rate_factor.data_to_msg(msg, mf_begin);
    mf_begin = sample_rate_multiplier.data_to_msg(msg, mf_begin);
    mf_begin = activity_flags.data_to_msg(msg, mf_begin);
    mf_begin = io_and_clock_flags.data_to_msg(msg, mf_begin);
    mf_begin = data_quality_flags.data_to_msg(msg, mf_begin);
    mf_begin = timing_quality.data_to_msg(msg, mf_begin);
    mf_begin = sec.data_to_msg(msg, mf_begin);
    mf_begin = usec.data_to_msg(msg, mf_begin);
    mf_begin = sequence_number.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & F1RawInput::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- F1_RAW_INPUT ---  \n";

    cmd_os << "\nendian_check: "; cmd_os << endian_check;

    cmd_os << "\nnumber_of_samples: "; cmd_os << number_of_samples;

    cmd_os << "\nseed_name: "; cmd_os << seed_name;

    cmd_os << "\nyear: "; cmd_os << year;

    cmd_os << "\ndoy: "; cmd_os << doy;

    cmd_os << "\nsample_rate_factor: "; cmd_os << sample_rate_factor;

    cmd_os << "\nsample_rate_multiplier: "; cmd_os << sample_rate_multiplier;

    cmd_os << "\nactivity_flags: "; cmd_os << activity_flags;

    cmd_os << "\nio_and_clock_flags: "; cmd_os << io_and_clock_flags;

    cmd_os << "\ndata_quality_flags: "; cmd_os << data_quality_flags;

    cmd_os << "\ntiming_quality: "; cmd_os << timing_quality;

    cmd_os << "\nsec: "; cmd_os << sec;

    cmd_os << "\nusec: "; cmd_os << usec;

    cmd_os << "\nsequence_number: "; cmd_os << sequence_number;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace