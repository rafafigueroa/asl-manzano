#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cctype>
#include <algorithm>
#include <string>

/* This file autogenerates:
   For each cal sequence entry in the plain text
   it generates a json formatted entry
*/
#include "json.h"

#include <tuple>

std::tuple<unsigned int, unsigned int, unsigned int>duration_txt_to_hms(
        std::string &txt) {

    if (txt == "0" or txt == "0s" or txt == "0m" or txt == "0h") {
        return std::make_tuple(0, 0, 0);
    }

    unsigned int h = 0; // hours
    unsigned int m = 0; // minutes
    unsigned int s = 0; // seconds

    if ( txt.size() <= 1 ) {
        std::cerr << "\nwrong duration format, good examples: 5m, 2h or 20s";
        std::exit(EXIT_FAILURE);
    }

    if (txt.back() == 's') {
        s = std::stoi( txt.substr(0, txt.size() - 1) );
    } else if (txt.back() == 'm') {
        m = std::stoi( txt.substr(0, txt.size() - 1) );
    } else if (txt.back() == 'h') {
        h = std::stoi( txt.substr(0, txt.size() - 1) );
    }

    return std::make_tuple(h, m, s);
}

int main() {
    std::cout << "calibration class builder" << std::endl;

// <ports> <cal_monitor> <amplitude> <settle> <duration> <trailer>

    struct Cal {
        std::string port;
        std::string monitor;
        std::string amplitude;
        std::string settling_time;
        std::string cal_duration;
        std::string trailer_time;
        std::string wave_form;
        std::string frequency;
    };

    //open and parse the plain format cal format
    std::ifstream plain_fs;
    plain_fs.open("../autocal_sequences.config");

    if (not plain_fs) {
        std::cerr << "ERROR: can't open cals format file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    struct CalRoutine {
        std::vector<Cal> cals;
        std::string model;
    };

    std::vector<CalRoutine> cal_routines;

    for (std::string line; std::getline(plain_fs, line); ) {
        if (line[0] == '#') {

            cal_routines.push_back( CalRoutine{} );
            cal_routines.back().model = line.substr(1);

        } else {
            Cal cal;
            std::istringstream line_iss(line);

            line_iss
                >> cal.port
                >> cal.monitor
                >> cal.amplitude
                >> cal.settling_time
                >> cal.cal_duration
                >> cal.trailer_time
                >> cal.wave_form
                >> cal.frequency;

            cal_routines.back().cals.push_back(cal);
        }
    }
/*
    for (const auto& cr : cal_routines) {
        std::cout << "\nCal Routine: " << std::endl;
        std::cout << cr.model;
        for (const auto& cal : cr.cals) {
            std::cout << std::endl;
            std::cout << cal.port << " * ";
            std::cout << cal.monitor << " * ";
            std::cout << cal.amplitude << " * ";
            std::cout << cal.settling_time << " * ";
            std::cout << cal.cal_duration << " * ";
            std::cout << cal.trailer_time << " * ";
            std::cout << cal.wave_form << " * ";
            std::cout << cal.frequency << " * ";
        }
    }
    */

   // close files
    plain_fs.close();

    // open the json file
    std::ofstream json_fs;
    json_fs.open("../../runtime_config/cal_sequences.json",
            std::ofstream::out | std::ofstream::trunc);

    if (not json_fs) {
        std::cerr << "ERROR: can't open cals format file" << std::endl;
        std::exit(EXIT_FAILURE);
    }


    json_fs << "{\n";

    for (auto const & cr : cal_routines) {

        json_fs << "\"" << cr.model << "\": {";
        json_fs << "\n  \"port\": ";

        // 1, 2, 3 = A
        // 4, 5, 6 = B

        if (cr.cals[0].port.find("1") != std::string::npos or
            cr.cals[0].port.find("2") != std::string::npos or
            cr.cals[0].port.find("3") != std::string::npos) {

            json_fs << "\"A\",";
        } else {
            json_fs << "\"B\",";
        }

        json_fs << "\n  \"monitor\": ";
        json_fs << cr.cals[0].monitor << ",";
        json_fs << "\n  \"E300_installed\": ";

        if ( cr.model.find("E300") !=  std::string::npos ) {
            json_fs << "true,";
        } else {
            json_fs << "false,";
        }

        json_fs << "\n  \"cals\": [\n";

        for (auto const & cal : cr.cals) {

            json_fs << "    {\"amplitude\": ";
            json_fs << cal.amplitude;

            // duration objects
            std::string t;
            unsigned int h, m, s;

            json_fs << ", \"settling_time\": ";
            t = cal.settling_time;
            std::tie(h, m, s) = duration_txt_to_hms(t);
            json_fs << "{\"h\":" << h << ",\"m\":" << m << "}";

            json_fs << ", \"cal_duration\": ";
            t = cal.cal_duration;
            std::tie(h, m, s) = duration_txt_to_hms(t);
            json_fs << "{\"h\":" << h << ",\"m\":" << m << "}";

            json_fs << ", \"trailer_time\": ";
            t = cal.trailer_time;
            std::tie(h, m, s) = duration_txt_to_hms(t);
            json_fs << "{\"h\":" << h << ",\"m\":" << m << "}";

            // waveform
            json_fs << ", \"wave_form\": ";
            json_fs << "\"" << cal.wave_form << "\"";

            if (cal.wave_form != "step") {
                json_fs << ", \"frequency\": ";
                json_fs << cal.frequency;
            }

            json_fs << "}";

            if ( &cal != &cr.cals.back() ) {
                json_fs << ",";
            }
            json_fs << "\n";

        }

        json_fs << "  ]";
        json_fs << "\n}";
        if (&cr != &cal_routines.back() ) {
            json_fs << ",";
        }
        json_fs << "\n\n";

    }
    json_fs << "}";

    json_fs.close();
    return 0;
};


