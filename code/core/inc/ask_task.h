// ** message_dispatch Object ** //
// Manzano software

#ifndef _MZN_ASK_TASK_H
#define _MZN_ASK_TASK_H

#include <chrono>
#include <mutex>
#include <future>

#include "date.h" // <- howard hinnants library
#include "seismic_network.h"
#include "commands.h"
#include "user_instruction.h"

namespace mzn {

/*!
    @author rfigueroa@usgs.gov
 */

// -------------------------------------------------------------------------- //
struct AskCharTask {

    std::future<char> ftr;
    std::atomic<bool> cv_cancel_task_{false};
    std::mutex cv_mutex_;
    std::condition_variable waiting_cv_;

    template <typename Rep, typename Period>
    char ask_while_sleep(std::chrono::duration<Rep, Period> const & timeout);

private:
    template <typename Rep, typename Period>
    char ask();
};

// -------------------------------------------------------------------------- //
template <typename R, typename P>
inline
char
AskCharTask::ask_while_sleep(std::chrono::duration<R, P> const & timeout) {


    ftr = std::async( std::launch::async,
                      &AskCharTask::ask,
                      this );

    std::this_thread::sleep_for(timeout);

    auto ready_ftr = [&]() {
        auto constexpr zero_duration = std::chrono::milliseconds(0);
        return ftr.wait_for(zero_duration) == std::future_status::ready;
    }

    if ( ready_ftr() ) {
        return ftr.get();
    } else {
        std::unique_lock<std::mutex> lk(cv_mutex_);
        cv_cancel_task_ = true;
        waiting_cv_.notify_all();
        if ( not ready_ftr() ) throw std::runtime_error("ask_with_timeout");
    }
}

// -------------------------------------------------------------------------- //
inline
char ask() {

    // get the lock before waiting
    std::unique_lock<std::mutex> lk(cv_mutex_);

    // cppref answer for return value:
    // "false if the predicate pred still evaluates to false after the
    // rel_time timeout expired, otherwise true."

    // return of waiting_cv_ relation with cv_predicate predicate:
    // rows are cv_notified
    // --------------------------------------------------------- //
    // cv_predicate > |                     |                    //
    // cv_notified  v |         true        |       false        //
    // --------------------------------------------------------- //
    //       true     |     cancel task     | * (spurious wake)  //
    //       false    |     logic error     | wait over, do work //
    // --------------------------------------------------------- //
    // * in the case of a spurious wake, the function does not return
    // that is the feature of this weird mechanic

    auto cv_predicate = [this]{return (cv_cancel_task_ == true);};

    bool cv_notified = waiting_cv_.wait_for(lk,
                                            msg_task.delay(),
                                            cv_predicate);

    std::cout << std::endl << "wait_and_send: wait over\n";

    if (cv_notified and cv_cancel_task_) {
        std::cout << std::endl << "Task Cancelled\n";
        return std::move(msg_task);
    }

    if ( (not cv_notified) and (not cv_cancel_task_) ) {
        // wait over (without cancelling notification) do work
        std::cout << std::endl << "\nwait_and_send: doing work\n";
        auto & q = sn_.q_ref(msg_task.ta);

        md_.send_recv( q.port_config,
                       *(msg_task.cmd_send.get()),
                       *(msg_task.cmd_recv.get()) );

        std::cout << std::endl << *(msg_task.cmd_recv.get());
        msg_task.done = true;
        return std::move(msg_task);
    }

    // accept enter as response
    char const c = std::getchar();
    return c;
}


} // end namespace
#endif // _MZN_ASK_TASK_H_
