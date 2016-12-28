// ** message_dispatch Object ** //
// Manzano software

#ifndef _MZN_MSG_TASK_H
#define _MZN_MSG_TASK_H

#include <chrono>
#include <mutex>
#include <future>

#include "date.h" // <- howard hinnants library
#include "seismic_network.h"
#include "commands.h"
#include "user_instruction.h"

namespace mzn {

//! Packages everything needed to send a multithreaded task to send a msg
//! in the future
/*!
    @author rfigueroa@usgs.gov
 */

// -------------------------------------------------------------------------- //
struct MsgTask {

    //! setups the threads for timed_send
    //! TODO: a lot of this will not be constant
    // --------------------------------------------------------------------- //
    template<typename SendType, typename RecvType>
    MsgTask(SendType const & cmd_send,
            RecvType const & cmd_recv,
            std::chrono::seconds const & delay,
            std::chrono::seconds const & run_duration,
            UserInstruction const & ui,
            TargetAddress const & ta) :

            action(ui.action),
            kind(ui.kind),
            ta(ta),
            cmd_send( std::make_unique<SendType>(cmd_send) ),
            cmd_recv( std::make_unique<RecvType>(cmd_recv) ),
            delay(delay),
            run_duration(run_duration) {}

    ~MsgTask() = default;

    //! no copy
    // --------------------------------------------------------------------- //
    MsgTask(MsgTask const & rhs) = delete;
    MsgTask & operator=(MsgTask const & rhs) = delete;

    //! yes move
    // --------------------------------------------------------------------- //
    MsgTask(MsgTask && rhs) noexcept :
        action(rhs.action),
        kind(rhs.kind),
        ta(rhs.ta),
        cmd_send( std::move(rhs.cmd_send) ),
        cmd_recv( std::move(rhs.cmd_recv) ),
        delay(rhs.delay),
        run_duration(rhs.run_duration),
        conf_time( Time::SysClock::now() ) {}

    Action const action;
    Kind const kind;
    TargetAddress const ta;

    std::unique_ptr<Command> cmd_send;
    std::unique_ptr<Command> cmd_recv;

    //! time related members and functions
    // --------------------------------------------------------------------- //
    CmdFieldDuration<int> delay;
    CmdFieldDuration<int> run_duration;

    // all using seconds precision
    CmdFieldTime<int> const conf_time;
    CmdFieldTime<int> const exec_time;
    CmdFieldTime<int> const end_time;

private:
    CmdFieldTime<int> configuration_tp_;

public:
    std::atomic<bool> done{false};

    template<typename ExceptionType>
    void set_exception(ExceptionType && e) {
        exception_ = std::make_unique<ExceptionType>(e);
    }

    bool has_exception() const {
        return not (exception_ == nullptr);
    }

private:
    std::unique_ptr<Exception> exception_ = nullptr;

};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, MsgTask const & msg_task) {

    os << "++ " << msg_task.action << " " << msg_task.kind << " " << msg_task.ta
       << " ~~ " << "run_duration: " << msg_task.run_duration
       << " done[" << CmdFieldBitmap<1>::bool_indicator(msg_task.done) << "]";

    return os;
}

} // end namespace
#endif // _MZN_MSG_TASK_H_
