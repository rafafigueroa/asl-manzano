#ifndef _MZN_CSV_FILE_H_
#define _MZN_CSV_FILE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "date.h"
#include "falcon_data_packet.h"
#include "string_utilities.h"

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

    ~CsvFile() = default;

    // --------------------------------------------------------------------- //
    static
    void set_fh_date(std::vector<std::string> const & tokens,
                     FalconHeader & fh);

    //! uses get_line which is not const
    // --------------------------------------------------------------------- //
    std::vector<FalconDataPacket> falcon_data_packets();
};

#endif // _MZN_CSV_FILE_H_
