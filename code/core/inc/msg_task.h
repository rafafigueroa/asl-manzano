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
        conf_time( Time::SysClock::now() ),
        exec_time( conf_time() + delay() ),
        end_time( exec_time() + run_duration() ) {}

    Action const action;
    Kind const kind;
    TargetAddress const ta;

    std::unique_ptr<Command> cmd_send;
    std::unique_ptr<Command> cmd_recv;

    //! time related members and functions
    // --------------------------------------------------------------------- //
    CmdFieldDuration<int> delay;
    CmdFieldDuration<int> run_duration;

    //! all using seconds precision
    CmdFieldTime<int> const conf_time;
    //! first provided with approximate values, but not const
    //! so it can be udpated with values closer to actual
    CmdFieldTime<int> exec_time;
    CmdFieldTime<int> end_time;

    std::atomic<bool> done{false};

    // --------------------------------------------------------------------- //
    template<typename ExceptionType>
    void set_exception(ExceptionType && e) {
        exception_ = std::make_unique<ExceptionType>(e);
    }

    // --------------------------------------------------------------------- //
    bool has_exception() const {
        return not (exception_ == nullptr);
    }

    // --------------------------------------------------------------------- //
    template<typename Ci>
    std::ostream &
    stream(std::ostream & os) const;

private:

    std::unique_ptr<Exception> exception_ = nullptr;
};

// -------------------------------------------------------------------------- //
inline
std::ostream & operator<<(std::ostream & os, MsgTask const & msg_task) {

    os << "\n.............................."
       << "....................................\n";

    os << "++ " << msg_task.action << " " << msg_task.kind << " " << msg_task.ta
       << " ~~ " << "run_duration: " << msg_task.run_duration
       << "\n   | exec_time: " << msg_task.exec_time
       << "\n   | end_time:  " << msg_task.end_time;
       // << " done[" << CmdFieldBitmap<1>::bool_indicator(msg_task.done) << "]";

    return os;
}

// -------------------------------------------------------------------------- //
template<typename Ci>
inline
std::ostream & MsgTask::stream(std::ostream & os) const {
    os << *this;
    return os;
}

// -------------------------------------------------------------------------- //
template<>
inline
std::ostream & MsgTask::stream<C1Qcal>(std::ostream & os) const {

    os << *this;

    // get the specific information from the command
    try {

        auto const & cal =
            dynamic_cast<C1Qcal const &>( *( cmd_send.get() ) );

        // do a lean printing of the calibration command
        os << "\n       | settling: " << cal.settling_time
           << "\n       | duration: " << cal.cal_duration
           << "\n       | trailing: " << cal.trailer_time;

    } catch (std::bad_cast const & e) {
        throw std::logic_error(
            "MsgTask::stream<C1Qcal> cmd_send is not of C1Qcal type");
    }

    return os;
}

} // end namespace
#endif // _MZN_MSG_TASK_H_
