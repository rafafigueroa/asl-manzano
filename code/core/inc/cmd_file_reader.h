#ifndef _MZN_CMD_FILE_READER_H_
#define _MZN_CMD_FILE_READER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#include "commands.h"
#include "seismic_network.h"
#include "msg_task.h"
#include "ui_to_cmd_type.h"
#include "system_calls.h"

namespace mzn {

//! Creates C1Qcal cmds from cal config files and timed sends them
/*!
    @author rfigueroa@usgs.gov
 */

// -------------------------------------------------------------------------- //
class CmdFileReader {

    using Seconds = std::chrono::seconds;

    //! same lifetime as CmdFieldReader
    SeismicNetwork const & sn_;

public:
    explicit
    CmdFileReader(SeismicNetwork const & sn);

    ~CmdFileReader() = default;

    //! no copy construction or assignment
    CmdFileReader(CmdFileReader const & rhs) = delete;
    CmdFileReader & operator=(CmdFileReader const & rhs) = delete;

    //! calls construct_cmds and package them in tasks
    template <Action action, Kind kind,
    class Ci = typename Ci<action, kind>::type,
    class Co = typename Co<action, kind>::type >
    std::vector<MsgTask> construct_msg_tasks(UserInstruction const & ui,
                                             TargetAddress const & ta);
private:
    //! checks json format and throws if error
    //! @throws if json format error
    void check_json(bool const check, std::string const & e_what);

    //! read cals cal sequences config file and setup cal_tasks_;
    template <typename Ci>
    std::vector<Ci> construct_cmds(TargetAddress const & ta);

    //! when using msg_task_manager, the delays allows for independent delayed
    //! msg sending, calibrations not currently using the msg_task_manager
    std::vector<Seconds>
    calculate_delays(std::vector<Seconds> const & run_durations);

    //! total duration, i.e.: for calibrations includes settling time
    template <typename Ci>
    std::vector<Seconds> calculate_run_durations(std::vector<Ci> const & cmds);
};

// -------------------------------------------------------------------------- //
template <Action action, Kind kind, class Ci, class Co>
inline
std::vector<MsgTask>
CmdFileReader::construct_msg_tasks(UserInstruction const & ui,
                                   TargetAddress const & ta) {

    auto const cmds = construct_cmds<Ci>(ta);
    auto const run_durations = calculate_run_durations(cmds);
    auto const delays = calculate_delays(run_durations);

    Co const cmd_recv;

    // --- package into tasks ---
    std::vector<MsgTask> msg_tasks{};

    for (int i = 0; i < cmds.size(); i++) {

        MsgTask msg_task(cmds[i],
                         cmd_recv,
                         delays[i],
                         run_durations[i],
                         ui,
                         ta);

        msg_tasks.push_back( std::move(msg_task) );
    }
    // error
    return msg_tasks;
}

} // <- mzn

#endif // _MZN_CMD_FILE_READER_H_

