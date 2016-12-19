// *%* CmdField Class ** //
// Manzano software
#include "cmd_field_time.h"
namespace mzn {


/*
    // from manual, for several time vars, time is seconds since jan 1 2000
    std::tm t2000 = {0};
    t2000.tm_year = 2000 - 1900;
    t2000.tm_mon = 0;
    t2000.tm_mday = 1;
    t2000.tm_hour = 0;
    t2000.tm_yday = 0;
    t2000.tm_isdst = 0;

    std::time_t t2000_t = std::mktime(&t2000);
    std::chrono::system_clock::time_point last_reboot_tp =
        std::chrono::system_clock::from_time_t(t2000_t);

    auto time_seconds = (*p_cvv_)[cv_index_].cv_value.to_ulong();
    last_reboot_tp += std::chrono::seconds(time_seconds);

    std::time_t last_reboot_date =
        std::chrono::system_clock::to_time_t(last_reboot_tp);

    // workaround
    // the number of seconds since jan 1 2000 that the q330 provides
    // already include the conversion to UTC.
    // trying to use gmtime on it is meaningless
    // trying to use it the date as a local time does not work with DST
    // using gmtime with last_reboot_date + now - now_utc
    // basically eliminates the current locality on last_reboot_time
    // as the now_utc also gets localized with mktime
    // cancelling each other.
    // hopefully with the changes in g++5 and put_time and better chrono
    // this workaround would not be needed
    // another way is to check if currently we are in DST and
    // set tm_isdst in t2000, allowing to use std::localtime
    // which would actually give the right UTC time

    // TODO: revisit with g++5
    std::time_t now = std::chrono::system_clock::to_time_t (
            std::chrono::system_clock::now() );
    std::tm now_utc_tm = *std::gmtime(&now);
    std::time_t now_utc = std::mktime(&now_utc_tm);
    std::time_t lrd_utc = last_reboot_date + now - now_utc;

    std::ostringstream hv_oss;
    hv_oss << std::asctime( std::gmtime(&lrd_utc) );

    return hv_oss.str();

    */
}// << mzn

