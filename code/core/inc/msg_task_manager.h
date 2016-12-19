// Manzano software

#ifndef _MZN_MSG_TASK_MANAGER_H
#define _MZN_MSG_TASK_MANAGER_H

#include <chrono>
#include <mutex>
#include <future>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "seismic_network.h"
#include "message_dispatch.h"
#include "commands.h"
#include "msg_task.h"

namespace mzn {

//!
/*!
    notify_one() and notify_all() are only used to cancel the task
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
class MsgTaskManager {

    SeismicNetwork & sn_;
    //! calls send_recv function
    MessageDispatch & md_;

public:
    //! setups the threads for timed_send
    // --------------------------------------------------------------------- //
    explicit
    MsgTaskManager(SeismicNetwork & sn, MessageDispatch & md);
    //! calls stop_timed_send
    ~MsgTaskManager();

    // --------------------------------------------------------------------- //
    MsgTaskManager(MsgTaskManager const & rhs) = delete;
    MsgTaskManager & operator=(MsgTaskManager const & rhs) = delete;

    // --------------------------------------------------------------------- //
    void add(std::vector<MsgTask> && msg_tasks);

private:

    // --------------------------------------------------------------------- //
    std::vector<MsgTask> tasks_to_process_;
    std::vector<MsgTask> waiting_tasks_;
    std::vector<MsgTask> processed_tasks_;
    // --------------------------------------------------------------------- //
    std::vector< std::future<MsgTask> > futures;
    // --------------------------------------------------------------------- //
    std::atomic<bool> cv_cancel_task_{false};
    std::mutex cv_mutex_;
    std::condition_variable waiting_cv_;

    // --------------------------------------------------------------------- //
    void stop_timed_send();
    // --------------------------------------------------------------------- //
    MsgTask wait_send_recv(MsgTask && msg_task);
};

} // end namespace
#endif // _MZN_MSG_TASK_MANAGER_H_
