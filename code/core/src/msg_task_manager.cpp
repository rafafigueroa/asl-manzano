// Manzano software

#include "msg_task_manager.h"

namespace mzn {

// -------------------------------------------------------------------------- //
MsgTaskManager::MsgTaskManager(SeismicNetwork & sn,
                               MessageDispatch & md) :
        sn_(sn),
        md_(md) {}

// -------------------------------------------------------------------------- //
MsgTaskManager::~MsgTaskManager() {
    try {
        stop_timed_send();
    } catch(std::exception const & e) {
        std::cerr << std::endl << "Error in MsgTaskManager Destructor:\n";
        std::cerr << e.what() << std::endl;
    }
}

// -------------------------------------------------------------------------- //
void MsgTaskManager::stop_timed_send() {
    // get the lock before waiting
    {
        std::unique_lock<std::mutex> lk(cv_mutex_);

        cv_cancel_task_ = true;
        waiting_cv_.notify_all();
    }

    std::cout << std::endl << "Getting results from futures";
    for (auto & ftr : futures) {

        bool ready_ftr = (ftr.wait_for(std::chrono::milliseconds(100))
                          == std::future_status::ready);

        if (ready_ftr) {
            auto const msg_task = ftr.get();
        } else {
            // all the futures should have been notified
            // and stopped
            throw std::logic_error("ftr not ready MsgTaskManager::stop_timed_send");
        }
    }
}

// -------------------------------------------------------------------------- //
void MsgTaskManager::add(std::vector<MsgTask> && msg_tasks) {

    // Where to lock? when changing the storage vectors?
    for (auto & msg_task : msg_tasks) {

        futures.push_back( std::async( std::launch::async,
                                       &MsgTaskManager::wait_send_recv,
                                       this,
                                       std::move(msg_task) ) );
    }
}

// -------------------------------------------------------------------------- //
MsgTask MsgTaskManager::wait_send_recv(MsgTask && msg_task) {

    try {
        // predicated for the condition variable
        // a secondary mechanism to interrupt a thread is needed
        // due to spurious awakes from the system (posix threads)

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

        throw std::logic_error("MsgTaskManager::wait_and_send");

    } catch (Exception & e) {

        std::cerr << std::endl << "Caught @MsgTaskManager::wait_and_send\n";
        std::cerr << std::endl << e.what() << std::endl;
        // this does the work of future.set_exception, but in this case
        // the future is only being used in this class, privately, while
        // this msg_task will be moved/used by other classes
        msg_task.set_exception( std::move(e) );
        return std::move(msg_task);
    }
}


} // << mzn

