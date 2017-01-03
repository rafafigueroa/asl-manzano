#include "csv_file.h"

// --------------------------------------------------------------------- //
std::vector<FalconDataPacket> CsvFile::falcon_data_packets() {

    std::vector<FalconDataPacket> falcon_data_packets;

    FalconHeader falcon_header;
    std::string line;
    std::vector<std::string> tokens;

    // the first 5 lines are part of a special header
    // Name:,DEV Falcon
    // ----------------------------------------------------------------- //
    std::getline(csv_fs, line); tokens = get_tokens(line);
    expect_string("Name:", tokens[0]);
    falcon_header.name = tokens[1];

    // Chan:,105
    // ----------------------------------------------------------------- //
    std::getline(csv_fs, line); tokens = get_tokens(line);
    expect_string("Chan:", tokens[0]);
    falcon_header.channel = tokens[1];

    // Type:,Minute <- all supossed to be Minute, check:
    // ----------------------------------------------------------------- //
    std::getline(csv_fs, line); tokens = get_tokens(line);
    expect_string("Type:", tokens[0]);
    expect_string("Minute", tokens[1]);

    // Desc:,ITS: Internal Temperature Sensor
    // ----------------------------------------------------------------- //
    std::getline(csv_fs, line); tokens = get_tokens(line);
    expect_string("Desc:", tokens[0]);
    auto code_and_desc = tokens[1];
    // ITS: Internal Temperature Sensor
    auto colon_index = code_and_desc.find_first_of(':');

    if (colon_index == std::string::npos) {
        throw std::logic_error{"no colon on description"};
    }

    auto code = code_and_desc.substr(0, colon_index);
    auto desc = code_and_desc.substr(colon_index + 2);
    falcon_header.code        = code;
    falcon_header.description = desc;

    // UOM:,DegF
    // ----------------------------------------------------------------- //
    std::getline(csv_fs, line); tokens = get_tokens(line);
    expect_string("UOM:", tokens[0]);
    falcon_header.unit_of_measure = tokens[1];

    // Date,Time,Avg,High,Low
    // ----------------------------------------------------------------- //
    std::getline(csv_fs, line); tokens = get_tokens(line);
    expect_string("Date", tokens[0]);
    expect_string("Time", tokens[1]);
    expect_string("Avg",  tokens[2]);
    expect_string("High", tokens[3]);
    expect_string("Low",  tokens[4]);

    // 12/28/16,22:17,74,74,74
    // 12/28/16,22:18,74,74,74
    // ----------------------------------------------------------------- //
    // process data all the way to the end, taking it from here

    // default constructed, all zeros
    MinuteTimePoint mtp_previous;

    // i == line index of raw data
    for (int i = 0; std::getline(csv_fs, line); i++) {

        tokens = get_tokens(line);

        // uses tokens 0, 1
        MinuteTimePoint mtp(tokens);

        // if the data points times are not contiguous, this data point needs
        // to go into a new FalconDataPacket
        bool const create_fdp = not mtp.is_contiguous_to(mtp_previous);

        if (create_fdp) {
            FalconDataPacket fdp;
            fdp.falcon_header = falcon_header;
            fdp.mtp = mtp;
            falcon_data_packets.push_back(fdp);
        }

        if ( falcon_data_packets.empty() ) throw std::logic_error("fdps empty");

        // get current fdp
        auto & fdp = falcon_data_packets.back();

        // use the average only
        auto value = std::stoi(tokens[2]);
        // add value to data on current fdp
        fdp.data.push_back(value);
        // setup for next loop
        mtp_previous = mtp;
    }

    return falcon_data_packets;
}

