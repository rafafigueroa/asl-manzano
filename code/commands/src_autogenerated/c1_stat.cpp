#include "c1_stat.h"

namespace mzn {
C1Stat::C1Stat():
    MultiCommandMap(0xA9, 4),
    status_bitmap() { }

uint16_t C1Stat::msg_to_data(std::vector<uint8_t> const & msg,
                             uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Stat",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = status_bitmap.msg_to_data(msg, mf_begin);
    mf_begin = MultiCommandMap::msg_to_data(msg, mf_begin);

    return mf_begin;
}

uint16_t C1Stat::data_to_msg(std::vector<uint8_t> & msg,
                             uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "C1Stat",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = status_bitmap.data_to_msg(msg, mf_begin);
    mf_begin = MultiCommandMap::data_to_msg(msg, mf_begin);

    return mf_begin;
}

std::ostream & C1Stat::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- C1_STAT ---  \n";

    cmd_os << "\nstatus_bitmap: "; cmd_os << status_bitmap;
    cmd_os << std::endl;

    return MultiCommand::os_print(cmd_os);
}


void C1Stat::create_new_ic(uint8_t const cmd_key) {

    switch(cmd_key) {
        case k_cx_global_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxGlobalStatus>() } );
            break;
        }
        case k_cx_gps_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxGpsStatus>() } );
            break;
        }
        case k_cx_power_supply_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxPowerSupplyStatus>() } );
            break;
        }
        case k_cx_boom_positions : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxBoomPositions>() } );
            break;
        }
        case k_cx_thread_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxThreadStatus>() } );
            break;
        }
        case k_cx_pll_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxPllStatus>() } );
            break;
        }
        case k_cx_gps_satellites : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxGpsSatellites>() } );
            break;
        }
        case k_cx_arp_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxArpStatus>() } );
            break;
        }
        case k_cx_data_port_1_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxDataPort1Status>() } );
            break;
        }
        case k_cx_data_port_2_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxDataPort2Status>() } );
            break;
        }
        case k_cx_data_port_3_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxDataPort3Status>() } );
            break;
        }
        case k_cx_data_port_4_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxDataPort4Status>() } );
            break;
        }
        case k_cx_serial_interface_1_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxSerialInterface1Status>() } );
            break;
        }
        case k_cx_serial_interface_2_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxSerialInterface2Status>() } );
            break;
        }
        case k_cx_serial_interface_3_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxSerialInterface3Status>() } );
            break;
        }
        case k_cx_ethernet_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxEthernetStatus>() } );
            break;
        }
        case k_cx_baler_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxBalerStatus>() } );
            break;
        }
        case k_cx_dynamic_ip : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxDynamicIp>() } );
            break;
        }
        case k_cx_auxiliary_board_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxAuxiliaryBoardStatus>() } );
            break;
        }
        case k_cx_serial_sensor_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxSerialSensorStatus>() } );
            break;
        }
        case k_cx_environmental_processor_status : {

            inner_commands.push_back(
                std::unique_ptr<Command>{ std::make_unique<CxEnvironmentalProcessorStatus>() } );
            break;
        }
    }
}
} // end namespace