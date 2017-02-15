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

    // ---------------------------------------------------------------------- //
    void stream_config(std::ostream & os) const;
    void stream_status(std::ostream & os) const;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & dp_os,
                          DataProcessor const & dp) {

    dp_os << "data_processor(" << dp.config.ip << ")";
    return dp_os;
};

} // end namespace
#endif // _MZN_DATAPROCESSOR_H_
