#include "c1_dev.h"
namespace mzn {

int C1Dev::number_of_ic(std::size_t const msg_size) const {

    int constexpr ic_msg_length = 20; // msg length of cx_dev
    return msg_size / ic_msg_length;
}

}
