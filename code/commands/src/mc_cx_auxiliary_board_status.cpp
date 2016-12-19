#include "cx_auxiliary_board_status.h"

namespace mzn {
int CxAuxiliaryBoardStatus::number_of_ic(std::size_t const msg_size) const {
    // this is called after base_set_data
    // the header data is available

   return (size_of_this_block.data() - 8)/4;

}
}
