// ** Data Processor Object ** //
// Manzano software
#include "data_processor.h"

namespace mzn {

// -------------------------------------------------------------------------- //
std::string DataProcessor::uptime() {

    std::string spass = "sshpass -p";
    std::string sso = "ssh -o StrictHostKeyChecking=no";
    std::string ss_cmd = "uptime";


    std::string sys_cmd = spass + " " +
                          config.pw + " " + sso + " " +
                          config.user + "@" + config.ip + " " +
                          ss_cmd +" > .LOCALTIME.tmp";  // + :port

    std::cout << "sys cmd ssh: " << sys_cmd.c_str();
    std::system( sys_cmd.c_str() );

    std::ifstream uptime_fs(".LOCALTIME.tmp");
    std::string uptime_value;
    std::getline(uptime_fs, uptime_value);
    return uptime_value;
}

} // << mzn
