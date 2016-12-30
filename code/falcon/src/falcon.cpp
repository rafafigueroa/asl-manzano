#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "date.h"

//! all data expected to be in minute intervals
// -------------------------------------------------------------------------- //
struct FalconData {

    // should be the same on all
    std::string name = "";
    // specific to this data
    std::string channel = ""; // falcon channel, not seed format channel
    std::string code = "";
    std::string description = "";
    std::string unit_of_measure = "";

    int d = 0;
    int m = 0;
    int y = 0;
    int h = 0;
    int min = 0;

    // raw input takes integer data
    std::vector<int32_t> data;

    // to the raw input format
    std::string to_raw_input_format();

    // ---------------------------------------------------------------------- //
    int seconds_since_midnight() const {

        /* overkill (left here, just in case is not the simple case)
        using Days = date::days;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using Hours = std::chrono::hours;

        auto ymd_data = date::year{y}/m/d;
        if ( not ymd_data.ok() ) throw std::runtime_error("Invalid date");
        auto tp_data = date::day_point(ymd_data) + Minutes(min) + Hours(h);
        // days precision, (almost due to leap seconds) UTC midnight of tp
        auto const tp_mn = date::floor<Days>(tp_data);
        // seconds precision of difference, seconds since UTC midnight of tp
        Seconds const since_midnight = date::floor<Seconds>(tp_data - tp_mn);
        // from seconds to hour/minute/second that can be streamed
        return since_midnight.count();
        */
        return h * 60 * 60 + min * 60;
    }

    // as in january 1 is 1
    // ---------------------------------------------------------------------- //
    int julian_day() const {

        auto ymd_start = date::year{y}/1/1;
        auto ymd_data = date::year{y}/m/d;

        if ( not ymd_start.ok() ) throw std::runtime_error("Invalid date");
        if ( not ymd_data.ok() ) throw std::runtime_error("Invalid date");

        auto days_start = date::day_point(ymd_start);
        auto days_data = date::day_point(ymd_data);

        auto days_diff = (days_data - days_start );

        return days_diff.count() + 1; // if same, jan 1 is 1, not 0
    }

    // ---------------------------------------------------------------------- //
    int days_in_year() const {

        auto ymd_this = date::year{y}/1/1;
        auto ymd_next = date::year{y+1}/1/1;

        if ( not ymd_this.ok() ) throw std::runtime_error("Invalid date");
        if ( not ymd_next.ok() ) throw std::runtime_error("Invalid date");

        auto days_this = date::day_point(ymd_this);
        auto days_next = date::day_point(ymd_next);

        auto days_diff = (days_next - days_this);

        return days_diff.count(); // if same, jan 1 is 1, not 0
    }

};

// -------------------------------------------------------------------------- //
std::ostream & operator<<(std::ostream & os, FalconData const & fd) {

    os << "\nname: " << fd.name
       << "\nchan: " << fd.channel
       << "\ncode: " << fd.code
       << "\ndesc: " << fd.description
       << "\nunit: " << fd.unit_of_measure
       << "\ndate: " << fd.y << "/" << fd.m << "/" << fd.d
       << " (day " << fd.julian_day() << " of " << fd.days_in_year() << ")"
       << "\ntime: " << fd.h << ":" << fd.min
       << " (second " << fd.seconds_since_midnight() << " since UTC midnight)"
       << "\ndata: [";

    for (auto const & d : fd.data) os << d << " ";

    os << "]" << " size(" << fd.data.size() << ")";

    return os;
}

// -------------------------------------------------------------------------- //
struct CsvFile {

    std::ifstream csv_fs;

    // ---------------------------------------------------------------------- //
    explicit
    CsvFile(std::string const & path) {

        // open and parse the csv
        csv_fs.open(path);

        if (not csv_fs) {
            std::stringstream ss_error;
            ss_error << "ERROR: can't open csv falcon file";
            ss_error << "\nfor path: " << path;

            throw std::runtime_error( ss_error.str() );
        }
    }

    // --------------------------------------------------------------------- //
    static
    std::vector<std::string> get_tokens(std::string const & line,
                                        char const delimiter = ',') {

        std::vector<std::string> tokens;
        std::stringstream ss;
        ss.str(line);
        std::string token;

        while ( std::getline(ss, token, delimiter) ) tokens.push_back(token);

        return tokens;
    }

    // --------------------------------------------------------------------- //
    static
    void expect(std::string const & expected,
                std::string const & token) {

        if (token != expected) {
            std::stringstream ss;
            ss << "expected:" << expected << " got:" << token;
            throw std::logic_error{ss.str()};
        }
    }

    // --------------------------------------------------------------------- //
    FalconData data() {

        FalconData falcon_data;
        std::string line;
        std::vector<std::string> tokens;

        // the first 5 lines are part of a special header
        // Name:,DEV Falcon
        // ----------------------------------------------------------------- //
        std::getline(csv_fs, line); tokens = get_tokens(line);
        expect("Name:", tokens[0]);
        falcon_data.name = tokens[1];

        // Chan:,105
        // ----------------------------------------------------------------- //
        std::getline(csv_fs, line); tokens = get_tokens(line);
        expect("Chan:", tokens[0]);
        falcon_data.channel = tokens[1];

        // Type:,Minute <- all supossed to be Minute, check:
        // ----------------------------------------------------------------- //
        std::getline(csv_fs, line); tokens = get_tokens(line);
        expect("Type:", tokens[0]);
        expect("Minute", tokens[1]);

        // Desc:,ITS: Internal Temperature Sensor
        // ----------------------------------------------------------------- //
        std::getline(csv_fs, line); tokens = get_tokens(line);
        expect("Desc:", tokens[0]);
        auto code_and_desc = tokens[1];
        // ITS: Internal Temperature Sensor
        auto colon_index = code_and_desc.find_first_of(':');

        if (colon_index == std::string::npos) {
            throw std::logic_error{"no colon on description"};
        }

        auto code = code_and_desc.substr(0, colon_index);
        auto desc = code_and_desc.substr(colon_index + 2);
        falcon_data.code        = code;
        falcon_data.description = desc;

        // UOM:,DegF
        // ----------------------------------------------------------------- //
        std::getline(csv_fs, line); tokens = get_tokens(line);
        expect("UOM:", tokens[0]);
        falcon_data.unit_of_measure = tokens[1];

        // Date,Time,Avg,High,Low
        // ----------------------------------------------------------------- //
        std::getline(csv_fs, line); tokens = get_tokens(line);
        expect("Date", tokens[0]);
        expect("Time", tokens[1]);
        expect("Avg",  tokens[2]);
        expect("High", tokens[3]);
        expect("Low",  tokens[4]);

        // 12/28/16,22:17,74,74,74
        // 12/28/16,22:18,74,74,74
        // ----------------------------------------------------------------- //
        // process data all the way to the end, taking it from here
        bool date_set = false;
        for (int i = 0; std::getline(csv_fs, line); i++) {
        // while ( std::getline(csv_fs, line) ) {

            tokens = get_tokens(line);

            if (not date_set) {

                auto date_tokens = get_tokens(tokens[0], '/');
                // falcon date is month/day/year (2 digit year)
                // making it 4 digit, starting with 2000
                // TODO: change this in 2100
                falcon_data.y = std::stoi(std::string("20") + date_tokens[2]);
                falcon_data.m = std::stoi(date_tokens[0]);
                falcon_data.d = std::stoi(date_tokens[1]);

                auto time_tokens = get_tokens(tokens[1], ':');
                // falcon time is hour:minute, no seconds
                falcon_data.h = std::stoi(time_tokens[0]);
                falcon_data.min = std::stoi(time_tokens[1]);

                date_set = true;
            }

            // use the average only
            falcon_data.data.push_back( std::stoi(tokens[2]) );
        }

        return falcon_data;
    }

    ~CsvFile() = default;

};


// -------------------------------------------------------------------------- //
std::string FalconData::to_raw_input_format() {

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
       << std::setw(short_w) << y
       << std::setw(short_w) << static_cast<uint16_t>( julian_day() )

       << std::setw(short_w) << rate_mantissa
       << std::setw(short_w) << rate_multiplier
       << std::setw(byte_w) << activity
       << std::setw(byte_w) << io_clock
       << std::setw(byte_w) << quality
       << std::setw(byte_w) << timing_quality

       // TODO
       // 28 int sec : Since midnight for the starting sample
       << std::setw(int_w) << seconds_since_midnight()
       // 32 int usec : Of the second
       << std::setw(int_w) << 0
       // 36 int seq : Sequence number of the packet
       << std::setw(int_w) << sequence_number_of_packet;

       // 40 int[nsamp] data : integer data
    return ss.str();
}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {

    std::cout << "\n +++++ falcon: csv files to data transformer +++++\n";

    try {

        std::vector<std::string> args(argv, argv + argc);

        if (args.size() == 1) {
            std::cerr << "\nNo files to process\n";
            return EXIT_SUCCESS;
        }

        // ------------------------------------------------------------------ //
        for (int i = 1; i < args.size(); i++) {

            std::cout << std::endl
                << "\n--------------------------------------"
                << "----------------------------";

            std::cout << "\nProcessing: " << args[i];

            CsvFile csv_file(args[i]);
            auto falcon_data = csv_file.data();
            std::cout << "\n" << falcon_data;
            auto raw_input = falcon_data.to_raw_input_format();
        }

        std::cout << "\n\n";

    } catch (std::exception const & e) {

        std::cerr << "\nunexpected error, closing program";
        std::cerr << std::endl << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
