#include "falcon_data_packet.h"
#include "f1_raw_input.h"

// -------------------------------------------------------------------------- //
std::vector<uint8_t> FalconDataPacket::to_raw_input_format() const {

    // auto network  = "NN"; // 2 chars
    // auto station  = "sssss"; // 5 chars
    // auto channel  = "CCC"; // 3 chars
    // auto location = "LL"; // 2 chars

    /*
        F1RawInput has the same format of the CWB RawInputServer
        without the data part

        CmdFieldHex<uint16_t> endian_check;
        CmdField<uint16_t> number_of_samples;
        CmdFieldArrayChar<12> seed_name;
        CmdField<uint16_t> year;
        CmdField<uint16_t> doy;

        // names from "Fixed Seciont of Data Header" on SEED format
        CmdField<int16_t> sample_rate_factor;
        CmdField<int16_t> sample_rate_multiplier;
        BmSeedHeaderActivityFlags activity_flags;
        BmSeedHeaderIoAndClockFlags io_and_clock_flags;
        BmSeedHeaderDataQualityFlags data_quality_flags;

        // from SEED format TODO: confirm variable type, might be bitmap
        CmdFieldHex<uint8_t> timing_quality;

        CmdFieldDuration<uint32_t> sec;
        CmdFieldDuration<uint32_t, std::micro> usec;
        CmdField<uint32_t> sequence_number;
    */

    mzn::F1RawInput fri;

    // 0 short 0xa1b2 : this also sets the endianness of the stream
    fri.endian_check(0xA1B2);

    // 2 short nsamp : Number of samples that follow
    fri.number_of_samples( data.size() );

    // 4 string*12 seedname : NNSSSSSCCCLL seed channel name
    std::array<char, 12> constexpr seed_name
        { {'N','N','s','s','s','s','s','C','C','C','L','L' } };

    fri.seed_name(seed_name);

    // 16 short year
    fri.year(mtp.y);
    fri.doy( mtp.julian_day() );

    // from SEED format
    // 20 short rateMantissa == sample_rate_factor
    // 22 short rateMultiplier == sample_rate_multiplier
    fri.sample_rate_factor(0);
    fri.sample_rate_multiplier(0);

    // from SEED format
    // 24 byte activity
    // 25 byte ioCLock
    // 26 byte quality
    fri.activity_flags.calibration_signals_present(false);
    fri.io_and_clock_flags.station_volume_parity_error_possibly_present(false);
    fri.data_quality_flags.amplifier_saturation_detected(false);

    // 27 byte timingQuality
    fri.timing_quality(0);

    // 28 int sec : Since midnight for the starting sample
    fri.sec( mtp.seconds_since_midnight() );
    // 32 int usec : Of the second
    fri.usec( std::chrono::microseconds(0) );
    fri.sequence_number(0);

    // serialization
    auto const fri_data_size = fri.cmd_data_size();
    auto const fdp_data_size = data.cmd_data_size();

    std::vector<uint8_t> msg(fri_data_size + fdp_data_size, 0);

    auto constexpr serialization_starting_byte = 0;
    fri.data_to_msg(msg, serialization_starting_byte);

    std::cout << std::endl << fri;

    // copy data on message starting where the F1RawInput message ends
    data.data_to_msg(msg, fri_data_size);

    std::cout << std::endl << data;

    // print out the message
    std::cout << std::endl << std::hex << "\n<" ;

    for (auto const & b : msg) std::cout << static_cast<int>(b) << " ";

    std::cout << ">" << std::dec;

    return msg;
}
