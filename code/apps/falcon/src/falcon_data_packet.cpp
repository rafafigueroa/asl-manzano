#include "falcon_data_packet.h"

// -------------------------------------------------------------------------- //
std::string FalconDataPacket::to_raw_input_format() const {

    std::stringstream ss;

    auto constexpr byte_w = 1;
    auto constexpr short_w = 2;
    auto constexpr int_w = 4;

    auto network  = "NN"; // 2 chars
    auto station  = "sssss"; // 5 chars
    auto channel  = "CCC"; // 3 chars
    auto location = "LL"; // 2 chars

    uint16_t constexpr endian_check = 0xa1b2;

    // from SEED format
    // 20 short rateMantissa
    // 22 short rateMultiplier
    // 24 byte activity
    // 25 byte ioCLock
    // 26 byte quality
    // 27 byte timingQuality

    uint16_t constexpr rate_mantissa = 2;
    uint16_t constexpr rate_multiplier = 2;
    uint8_t constexpr activity = 1;
    uint8_t constexpr io_clock = 1;
    uint8_t constexpr quality = 1;
    uint8_t constexpr timing_quality = 1;

    uint32_t constexpr sequence_number_of_packet = 4;

    ss << std::hex << std::setfill('0')
       // 0 short 0xa1b2 : this also sets the endianness of the stream
       << std::setw(short_w) << endian_check
       // 2 short nsamp : Number of samples that follow
       << std::setw(short_w) << static_cast<uint16_t>( data.size() )

       // 4 string*12 seedname : NNSSSSSCCCLL seed channel name
       << std::setfill(' ')
       << std::setw(2) << network
       << std::setw(5) << station
       << std::setw(3) << channel
       << std::setw(2) << location

       << std::hex << std::setfill('0')
       // 16 short year
       << std::setw(short_w) << mtp.y
       << std::setw(short_w) << static_cast<uint16_t>( mtp.julian_day() )

       << std::setw(short_w) << rate_mantissa
       << std::setw(short_w) << rate_multiplier
       << std::setw(byte_w) << activity
       << std::setw(byte_w) << io_clock
       << std::setw(byte_w) << quality
       << std::setw(byte_w) << timing_quality

       // TODO
       // 28 int sec : Since midnight for the starting sample
       << std::setw(int_w) << mtp.seconds_since_midnight()
       // 32 int usec : Of the second
       << std::setw(int_w) << 0
       // 36 int seq : Sequence number of the packet
       << std::setw(int_w) << sequence_number_of_packet;

       // 40 int[nsamp] data : integer data
    return ss.str();
}
