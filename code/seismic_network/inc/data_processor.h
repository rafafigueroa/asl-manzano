// ** Data Processor Object ** //
// Manzano software

#ifndef _MZN_DATAPROCESSOR_H
#define _MZN_DATAPROCESSOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "date.h"

namespace mzn {

//! Holds configuration and status of data processor
/*!
    @author rfigueroa@usgs.gov
 */
class DataProcessor {

private:

    struct Config {

        friend class DataProcessor;

        Config(std::string const in_user,
               std::string const in_pw,
               std::string const in_ip) :
            user(in_user),
            pw(in_pw),
            ip(in_ip) {}

        ~Config() = default;


        public:
            std::string user;
            std::string pw;
            std::string ip;
    };

    struct Status {
        friend class DataProcessor;
        std::string uptime;
    };


public:
    //! constructor
    DataProcessor(std::string const in_user,
                  std::string const in_pw,
                  std::string const in_ip) :
        config(in_user, in_pw, in_ip) {}

    ~DataProcessor() = default;

    Config config;
    Status status;

    //! time that this dp has been without rebooting
    std::string uptime();

    friend std::ostream & operator<<(std::ostream & os,
                                     Config const & c);

    friend std::ostream & operator<<(std::ostream & os,
                                     Status const & c);
};

inline
std::ostream & operator<<(std::ostream & dp_os,
                          DataProcessor const & dp) {

    dp_os << "data_processor(" << dp.config.ip << ")";
    return dp_os;
};

inline
std::ostream & operator<<(std::ostream & os,
                          DataProcessor::Config const & c) {

    os << "\n    ip: " << c.ip;
    return os;
}


inline
std::ostream & operator<<(std::ostream & os,
                          DataProcessor::Status const & c) {

    os << "\n    uptime : " << c.uptime;
    return os;
}

} // end namespace
#endif // _MZN_DATAPROCESSOR_H_
