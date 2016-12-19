#include "cy_thread_status.h"

namespace mzn {
CyThreadStatus::CyThreadStatus():
    Command(0, 16),
    total_running_time("ms"),
    priority(),
    priority_counter(),
    time_since_last_run("ms"),
    thread_flags() { }

uint16_t CyThreadStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                     uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyThreadStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = total_running_time.msg_to_data(msg, mf_begin);
    mf_begin = priority.msg_to_data(msg, mf_begin);
    mf_begin = priority_counter.msg_to_data(msg, mf_begin);
    mf_begin = time_since_last_run.msg_to_data(msg, mf_begin);
    mf_begin = thread_flags.msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t CyThreadStatus::data_to_msg(std::vector<uint8_t> & msg,
                                     uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyThreadStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = total_running_time.data_to_msg(msg, mf_begin);
    mf_begin = priority.data_to_msg(msg, mf_begin);
    mf_begin = priority_counter.data_to_msg(msg, mf_begin);
    mf_begin = time_since_last_run.data_to_msg(msg, mf_begin);
    mf_begin = thread_flags.data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & CyThreadStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CY_THREAD_STATUS ---  \n";

    cmd_os << "\ntotal_running_time: "; cmd_os << total_running_time;

    cmd_os << "\npriority: "; cmd_os << priority;

    cmd_os << "\npriority_counter: "; cmd_os << priority_counter;

    cmd_os << "\ntime_since_last_run: "; cmd_os << time_since_last_run;

    cmd_os << "\nthread_flags: "; cmd_os << thread_flags;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace