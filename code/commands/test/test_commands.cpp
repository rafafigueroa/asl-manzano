#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <limits>
#include <cmath>

#include "gtest/gtest.h"
#include "command.h"
#include "multi_command.h"
#include "multi_command_map.h"
#include "commands.h"

class FixtureCommand : public ::testing::Test {
public:

    mzn::C1Cack c1_cack{};
    mzn::C1Cerr c1_cerr{};
    mzn::C1Ctrl c1_ctrl{};
    mzn::C1Dev c1_dev{};
    mzn::C1Dsrv c1_dsrv{};
    mzn::C1Mysn c1_mysn{};
    mzn::C1Ping2 c1_ping_2{};
    mzn::C1Ping3 c1_ping_3{};
    mzn::C1Ping4 c1_ping_4{};
    mzn::C1Ping5 c1_ping_5{};
    mzn::C1Pollsn c1_pollsn{};
    mzn::C1Qcal c1_qcal{};
    mzn::C1Rqdev c1_rqdev{};
    mzn::C1Rqsc c1_rqsc{};
    mzn::C1Rqsrv c1_rqsrv{};
    mzn::C1Rqstat c1_rqstat{};
    mzn::C1Sc c1_sc{};
    mzn::C1Srvch c1_srvch{};
    mzn::C1Srvrsp c1_srvrsp{};
    mzn::C1Stat c1_stat{};
    mzn::C1Stop c1_stop{};
    mzn::C2Qv c2_qv{};
    mzn::C2Rqqv c2_rqqv{};
    mzn::C2Samass c2_samass{};
    mzn::C2Rqamass c2_rqamass{};
    mzn::C2Amass c2_amass{};
    mzn::CxArpStatus cx_arp_status{};
    mzn::CxAuxiliaryBoardStatus cx_auxiliary_board_status{};
    mzn::CxBalerStatus cx_baler_status{};
    mzn::CxBoomPositions cx_boom_positions{};
    mzn::CxDataPort1Status cx_data_port_1_status{};
    mzn::CxDataPort2Status cx_data_port_2_status{};
    mzn::CxDataPort3Status cx_data_port_3_status{};
    mzn::CxDataPort4Status cx_data_port_4_status{};
    mzn::CxDev cx_dev{};
    mzn::CxDynamicIp cx_dynamic_ip{};
    mzn::CxEnvironmentalProcessorStatus cx_environmental_processor_status{};
    mzn::CxEthernetStatus cx_ethernet_status{};
    mzn::CxGlobalStatus cx_global_status{};
    mzn::CxGpsSatellites cx_gps_satellites{};
    mzn::CxGpsStatus cx_gps_status{};
    mzn::CxPllStatus cx_pll_status{};
    mzn::CxPowerSupplyStatus cx_power_supply_status{};
    mzn::CxQv cx_qv{};
    mzn::CxSerialInterface1Status cx_serial_interface_1_status{};
    mzn::CxSerialInterface2Status cx_serial_interface_2_status{};
    mzn::CxSerialInterface3Status cx_serial_interface_3_status{};
    mzn::CxSerialSensorStatus cx_serial_sensor_status{};
    mzn::CxThreadStatus cx_thread_status{};
    mzn::CyArpStatus cy_arp_status{};
    mzn::CyAuxiliaryBoardStatus cy_auxiliary_board_status{};
    mzn::CyEnvironmentalProcessorStatus cy_environmental_processor_status{};
    mzn::CyGpsSatellites cy_gps_satellites{};
    mzn::CySerialSensorStatus cy_serial_sensor_status{};
    mzn::CyThreadStatus cy_thread_status{};
    mzn::CzHumidityAndExternalTemperature cz_humidity_and_external_temperature{};
    mzn::CzInternalTemperatureMeasurement cz_internal_temperature_measurement{};
    mzn::QdpCrc qdp_crc{};
    mzn::QdpHeader qdp_header{};

    virtual void SetUp() {

        // assign test values and run a
        // simple test to confirm right name/type
        // and function call operator
    }
    //virtual void TearDown() {};
};

// fails only if SetUp() fails
// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, commands_setup) {}
void print_vector(std::vector<uint8_t> const & msg) {

    std::cout << std::hex << std::showbase << std::uppercase;
    std::cout << "[";
    for (auto const & element : msg) {
        std::cout << static_cast<unsigned int>(element) << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << std::dec << std::noshowbase << std::nouppercase;
}

// -------------------------------------------------------------------------- //
void print_vector(std::vector<uint8_t> const & msg1,
                  std::vector<uint8_t> const & msg2) {

    if (msg1 != msg2) {
        std::cout << std::hex << std::showbase << std::uppercase;
        std::cout << "[";

        for (int i = 0; i < msg1.size(); i++) {

            if (static_cast<unsigned int>(msg1[i])
                != static_cast<unsigned int>(msg2[i])) {

                std::cout << static_cast<unsigned int>(msg1[i]) << ", "
                          << static_cast<unsigned int>(msg2[i]) << std::endl;
            }
        }
        std::cout << "]" << std::endl;
        std::cout << std::dec << std::noshowbase << std::nouppercase;
    }
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, msg_to_data) {
    std::vector<uint8_t> msg = {
        79, 23, 165, 184,
        56, 2, 0, 72,
        0, 0, 0, 2, // end of header
        0, 5, 0, 0, // c1_ping_5 msg
        0, 1, 0, 31,
        0, 0, 6, 14,
        1, 0, 0, 11,
        105, 186, 23, 211,
        0, 32, 0, 0,
        0, 13, 0, 0,
        0, 6, 128, 0,
        0, 76, 127, 80,
        0, 16, 0, 0,
        0, 36, 193, 5,
        0, 0, 0, 0,
        0, 65, 189, 36,
        0, 0, 11, 144,
        0, 0, 0, 0,
        0, 0, 0, 3,
        0, 0, 0, 3,
        0, 0, 1, 146};

    c1_ping_5.msg_to_data(msg, 12);

    EXPECT_EQ( 5, c1_ping_5.ping_type() );
    EXPECT_EQ( 0, c1_ping_5.ignore_0() );
    EXPECT_EQ( 1, c1_ping_5.version() );

    EXPECT_EQ( true, c1_ping_5.fix_flags.ethernet_installed() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_statreq_dynamic_ip() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_statreq_auxiliary_board() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_expanded_c1_web_cmds() );
    EXPECT_EQ( true, c1_ping_5.fix_flags.can_statreq_serial_sensor() );
    EXPECT_EQ( false, c1_ping_5.fix_flags.can_report_255maplus_supply_current() );
    EXPECT_EQ( false, c1_ping_5.fix_flags.at_least_one_environmental_proc_configured() );
    EXPECT_EQ( false, c1_ping_5.fix_flags.is_Q330S() );

    EXPECT_EQ( 1550, c1_ping_5.kmi_property_tag() );
    EXPECT_EQ( 0X100000B69BA17D3, c1_ping_5.serial_number() );
    EXPECT_EQ( 2097152, c1_ping_5.data_port_1_packet_memory_size() );
    EXPECT_EQ( 851968, c1_ping_5.data_port_2_packet_memory_size() );
    EXPECT_EQ( 425984, c1_ping_5.data_port_3_packet_memory_size() );
    EXPECT_EQ( 5013328, c1_ping_5.data_port_4_packet_memory_size() );
    EXPECT_EQ( 1048576, c1_ping_5.serial_interface_1_memory_trigger_level() );
    EXPECT_EQ( 2408709, c1_ping_5.serial_interface_2_memory_trigger_level() );
    EXPECT_EQ( 0, c1_ping_5.reserved_0() );
    EXPECT_EQ( 4308260, c1_ping_5.ethernet_interface_memory_trigger_level() );

    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.fill_mode_enabled() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags .flush_packet_buffer_time_based() );

    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.freeze_data_port_output() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.freeze_packet_buff_input() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.keep_oldest_data_packet_buffer() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.dp_piggyback_statreq_with_dt_dack() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.comm_fault_led_if_last_flush_buff_5plus() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.allow_hotswap_on_this_dataport() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.flush_sliding_window_buff_based_on_time() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.send_datapackets_encoded_base_96() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_1_advanced_flags.save_changes_in_eeprom() );

    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.fill_mode_enabled() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.flush_packet_buffer_time_based() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.freeze_data_port_output() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.freeze_packet_buff_input() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.keep_oldest_data_packet_buffer() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.dp_piggyback_statreq_with_dt_dack() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.comm_fault_led_if_last_flush_buff_5plus() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.allow_hotswap_on_this_dataport() );
    EXPECT_EQ( true, c1_ping_5.serial_interface_2_advanced_flags.flush_sliding_window_buff_based_on_time() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.send_datapackets_encoded_base_96() );
    EXPECT_EQ( false, c1_ping_5.serial_interface_2_advanced_flags.save_changes_in_eeprom() );

    EXPECT_EQ( 0, c1_ping_5.reserved_1() );

    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.fill_mode_enabled() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.flush_packet_buffer_time_based() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.freeze_data_port_output() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.freeze_packet_buff_input() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.keep_oldest_data_packet_buffer() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.dp_piggyback_statreq_with_dt_dack() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.comm_fault_led_if_last_flush_buff_5plus() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.allow_hotswap_on_this_dataport() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.flush_sliding_window_buff_based_on_time() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.send_datapackets_encoded_base_96() );
    EXPECT_EQ( false, c1_ping_5.ethernet_interface_advanced_flags.save_changes_in_eeprom() );

    EXPECT_EQ( 0, c1_ping_5.serial_interface_1_data_port_number() );
    EXPECT_EQ( 3, c1_ping_5.serial_interface_2_data_port_number() );
    EXPECT_EQ( 0, c1_ping_5.reserved_2() );
    EXPECT_EQ( 3, c1_ping_5.ethernet_interface_data_port_number() );

    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_1_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_2_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_3_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_4_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_5_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_6_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_7_failed_calibration() );
    EXPECT_EQ( false, c1_ping_5.calibration_error_bitmap.channel_8_failed_calibration() );

    EXPECT_EQ( 1, c1_ping_5.firmware_major() );
    EXPECT_EQ( 146, c1_ping_5.firmware_minor() );

    std::vector<uint8_t> newmsg (84, 0);
    for (int i = 0; i < 12; i++) {
        newmsg[i] = msg[i];
    }
    c1_ping_5.data_to_msg(newmsg, 12);
    EXPECT_EQ(msg, newmsg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, serializing_c1_stat) {
    std::vector<uint8_t> msg = {
        0XED, 0XC5, 0X6C, 0X98, 0XA9, 0X2, 0X1, 0XE8, 0, 0, 0, 0X3, 0, 0, 0, 0X51,
        0, 0, 0, 0XC5, 0, 0, 0, 0X5, 0X1D, 0XC5, 0X4, 0X63, 0, 0, 0, 0, 0X13, 0XCA,
        0XCA, 0XBF, 0X10, 0XF7, 0X5E, 0XD9, 0X1D, 0XC5, 0X4, 0X67, 0, 0, 0X2, 0X46,
        0, 0, 0, 0, 0, 0, 0X8, 0XD1, 0, 0, 0, 0X3, 0, 0, 0, 0, 0, 0XC3, 0X69, 0X8E,
        0, 0X15, 0X1, 0X5C, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5D, 0, 0, 0, 0, 0XB8, 0XEC,
        0X8D, 0X45, 0X1E, 0X1E, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5D, 0X20, 0X9, 0, 0X1,
        0X27, 0X49, 0XEE, 0XEA, 0X1, 0X1, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5D, 0, 0, 0,
        0, 0, 0XB, 0X4, 0X14, 0X8, 0X8, 0, 0X2, 0XFB, 0X5C, 0XD, 0X4C, 0X60, 0, 0,
        0, 0, 0XD, 0X11, 0XF4, 0X6, 0X6, 0, 0X2, 0XFB, 0X5C, 0XD, 0X44, 0X40, 0, 0,
        0, 0, 0XA, 0XDA, 0X54, 0X9, 0X9, 0, 0X2, 0XFB, 0X5C, 0XD, 0X34, 0X40, 0, 0,
        0, 0, 0, 0X22, 0X5C, 0X3, 0X3, 0, 0X2, 0XFB, 0X5B, 0XF0, 0X78, 0X40, 0, 0,
        0, 0, 0XD, 0XB2, 0X78, 0X7, 0X7, 0, 0X2, 0XFB, 0X5C, 0XD, 0X4C, 0X40, 0, 0,
        0, 0, 0X2C, 0X5B, 0XB3, 0XB, 0XB, 0, 0X2, 0XFB, 0X5C, 0XD, 0X43, 0X40, 0, 0,
        0, 0X5, 0X1B, 0X5B, 0X44, 0X14, 0X14, 0, 0X2, 0XFB, 0X5C, 0XC, 0X29, 0X20,
        0, 0, 0, 0, 0, 0X4C, 0XCE, 0XC, 0XC, 0, 0X2, 0XFB, 0X5C, 0XD, 0X42, 0X40, 0,
        0, 0, 0, 0X16, 0XE, 0X88, 0X12, 0X13, 0, 0X2, 0XFB, 0X5C, 0XC, 0X36, 0X20,
        0, 0, 0, 0X3, 0X5D, 0X74, 0XEC, 0X5, 0X5, 0, 0X2, 0XFB, 0X5C, 0XD, 0X5A,
        0X20, 0X1, 0, 0, 0, 0X53, 0X57, 0XB5, 0X10, 0X10, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X43, 0X40, 0, 0, 0, 0, 0, 0XAB, 0XC7, 0XF, 0XF, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X40, 0X40, 0, 0, 0, 0, 0, 0XD0, 0X92, 0XE, 0XE, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X40, 0X40, 0, 0, 0, 0, 0X1, 0XE1, 0X86, 0XD, 0XD, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X40, 0X40, 0, 0, 0, 0, 0X3, 0X9B, 0X9, 0X11, 0X11, 0, 0X2, 0XFB, 0X5C, 0XD,
        0X43, 0X60, 0, 0, 0, 0, 0, 0, 0, 0XA, 0XA, 0, 0, 0, 0, 0, 0, 0X20, 0, 0, 0,
        0, 0, 0, 0, 0XA, 0XA, 0, 0, 0, 0, 0, 0, 0X20, 0, 0, 0, 0, 0, 0, 0, 0XA, 0XA,
        0, 0, 0, 0, 0, 0, 0X20, 0, 0, 0, 0, 0, 0, 0, 0X2, 0X2, 0, 0, 0, 0, 0, 0,
        0X80, 0, 0, 0XA, 0, 0X54, 0, 0X5, 0, 0X6, 0X1, 0X3A, 0, 0X29, 0, 0X7, 0,
        0X38, 0X1, 0X48, 0, 0X35, 0, 0X8, 0, 0X33, 0, 0X3, 0, 0, 0, 0X9, 0, 0X4A,
        0, 0XC4, 0, 0X2D, 0, 0XB, 0, 0XA, 0, 0X91, 0, 0, 0, 0X10, 0, 0XB, 0, 0X2E,
        0, 0, 0, 0X17, 0, 0X2C, 0, 0X9C, 0, 0, 0, 0X1B, 0, 0X23, 0, 0X3C, 0, 0, 0,
        0X1C, 0, 0X14, 0, 0XEB, 0, 0X30, 0, 0X1E, 0, 0X20, 0X1, 0X2B, 0, 0X30};

    c1_stat.msg_to_data(msg, 12);

    std::vector<uint8_t> newmsg (msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        newmsg[i] = msg[i];
    }

    c1_stat.data_to_msg(newmsg, 12);
    EXPECT_EQ(msg, newmsg);
}

TEST_F(FixtureCommand, console_output) {
    //c1_cack.ack(4);
    //EXPECT_EQ(4, c1_cack.ack() );
/*
    auto test_console_output = [&](std::string intended, auto cf) {
        testing::internal::CaptureStdout();
        std::cout << cf;
        std::string const out = testing::internal::GetCapturedStdout();
        EXPECT_STREQ( intended.c_str(), out.c_str() );
    };
    */
}

TEST_F(FixtureCommand, c1_dsrv_test) {
    mzn::C1Dsrv cmd{};
    std::vector<uint8_t> msg = {0XFB, 0X1, 0X3, 0X10, 0X12, 0X2, 0, 0X8, 0, 0X4, 0, 0, 0X1, 0, 0, 0XB, 0X69, 0XBA, 0X17, 0XD3};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_rqsrv_test) {
    mzn::C1Rqsrv cmd{};
    std::vector<uint8_t> msg = {0XEA, 0X8A, 0XB0, 0X88, 0X10, 0X2, 0, 0X8, 0, 0X1, 0, 0, 0X1, 0, 0, 0XB, 0X69, 0XBA, 0X17, 0XD3};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_srvrsp_test) {
    mzn::C1Srvrsp cmd{};
    std::vector<uint8_t> msg = {0X4D, 0X9E, 0X1E, 0X98, 0X11, 0X2, 0, 0X30, 0, 0X2, 0, 0, 0X1, 0, 0, 0XB, 0X69, 0XBA, 0X17, 0XD3, 0XD, 0XCD, 0X21, 0XE9, 0XE, 0XC5, 0XA7, 0XF6, 0X88, 0XB1, 0X79, 0X5D, 0X8, 0, 0X1, 0XC4, 0, 0, 0, 0, 0, 0X7A, 0X9F, 0X51, 0X44, 0XC1, 0X75, 0X70, 0X72, 0X22, 0X6B, 0XAE, 0X47, 0X56, 0XEF, 0XA2, 0X86, 0X87, 0X5C, 0X56};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_rqstat_test) {
    mzn::C1Rqstat cmd{};
    std::vector<uint8_t> msg = {0XED, 0, 0X8, 0X68, 0X1F, 0X2, 0, 0X4, 0, 0X3, 0, 0, 0, 0, 0X80, 0X51};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_rqsc_test) {
    mzn::C1Rqsc cmd{};
    std::vector<uint8_t> msg = {0X3D, 0X90, 0X9A, 0X98, 0X2F, 0X2, 0, 0, 0, 0X3, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_sc_test) {
    mzn::C1Sc cmd{};
    std::vector<uint8_t> msg = {0XCF, 0XE8, 0X2, 0XC0, 0XAF, 0X2, 0, 0X20, 0, 0, 0, 0X3, 0, 0, 0X1, 0X2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X1, 0X1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X1, 0X4};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_vector(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_qcal_test) {
    mzn::C1Qcal cmd{};
    std::vector<uint8_t> msg = {0XE8, 0X6A, 0X8B, 0XA8, 0X23, 0X2, 0, 0X24, 0, 0X3, 0, 0, 0, 0, 0, 0, 0, 0X80, 0, 0X1, 0, 0XA, 0, 0X5, 0, 0X38, 0, 0X5, 0, 0X80, 0, 0X2, 0, 0X1, 0, 0, 0X72, 0X65, 0X73, 0X69, 0X73, 0X74, 0X69, 0X76, 0X65, 0, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_vector(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_cack_test) {
    mzn::C1Cack cmd{};
    std::vector<uint8_t> msg = {0XE8, 0X6A, 0X8B, 0XA8, 0X23, 0X2, 0, 0X24, 0, 0X3, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_vector(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_pollsn_test) {
    mzn::C1Cack cmd{};
    std::vector<uint8_t> msg = {0X43, 0XBA, 0, 0XA8, 0X14, 0X2, 0, 0X4, 0, 0X1, 0, 0, 0, 0, 0, 0};
    cmd.msg_to_data(msg, 20);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_vector(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

TEST_F(FixtureCommand, c1_rqdev_test) {
    mzn::C1Rqdev cmd{};
    std::vector<uint8_t> msg = {0X4D, 0XF9, 0X2E, 0X98, 0X36, 0X2, 0, 0, 0, 0X3, 0, 0};
    cmd.msg_to_data(msg, 12);

    std::vector<uint8_t> processed_msg ( msg.size(), 0);
    for (int i = 0; i < 12; i++) {
        processed_msg[i] = msg[i];
    }

    cmd.data_to_msg(processed_msg, 12);
    print_vector(msg, processed_msg);
    EXPECT_EQ(msg, processed_msg);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCommand, cmd_code) {
    /*std::cout << std::hex << std::endl;
    std::cout << "c1_cack : " << c1_cack.cmd_number << std::endl;
    std::cout << "c1_cerr : " << c1_cerr.cmd_number << std::endl;
    std::cout << "c1_ctrl : " << c1_ctrl.cmd_number << std::endl;
    std::cout << "c1_dev : " << c1_dev.cmd_number << std::endl;
    std::cout << "c1_dsrv : " << c1_dsrv.cmd_number << std::endl;
    std::cout << "c1_mysn : " << c1_mysn.cmd_number << std::endl;
    std::cout << "c1_ping_2 : " << c1_ping_2.cmd_number << std::endl;
    std::cout << "c1_ping_3 : " << c1_ping_3.cmd_number << std::endl;
    std::cout << "c1_ping_4 : " << c1_ping_4.cmd_number << std::endl;
    std::cout << "c1_ping_5 : " << c1_ping_5.cmd_number << std::endl;
    std::cout << "c1_pollsn : " << c1_pollsn.cmd_number << std::endl;
    std::cout << "c1_qcal : " << c1_qcal.cmd_number << std::endl;
    std::cout << "c1_rqdev : " << c1_rqdev.cmd_number << std::endl;
    std::cout << "c1_rqsc : " << c1_rqsc.cmd_number << std::endl;
    std::cout << "c1_rqsrv : " << c1_rqsrv.cmd_number << std::endl;
    std::cout << "c1_rqstat : " << c1_rqstat.cmd_number << std::endl;
    std::cout << "c1_sc : " << c1_sc.cmd_number << std::endl;
    std::cout << "c1_srvch : " << c1_srvch.cmd_number << std::endl;
    std::cout << "c1_srvrsp : " << c1_srvrsp.cmd_number << std::endl;
    std::cout << "c1_stat : " << c1_stat.cmd_number << std::endl;
    std::cout << "c1_stop : " << c1_stop.cmd_number << std::endl;
    std::cout << "c2_qv : " << c2_qv.cmd_number << std::endl;
    std::cout << "c2_rqqv : " << c2_rqqv.cmd_number << std::endl;
    std::cout << "cx_arp_status : " << cx_arp_status.cmd_number << std::endl;
    std::cout << "cx_auxiliary_board_status : " << cx_auxiliary_board_status.cmd_number << std::endl;
    std::cout << "cx_baler_status : " << cx_baler_status.cmd_number << std::endl;
    std::cout << "cx_boom_positions : " << cx_boom_positions.cmd_number << std::endl;
    std::cout << "cx_data_port_1_status : " << cx_data_port_1_status.cmd_number << std::endl;
    std::cout << "cx_data_port_2_status : " << cx_data_port_2_status.cmd_number << std::endl;
    std::cout << "cx_data_port_3_status : " << cx_data_port_3_status.cmd_number << std::endl;
    std::cout << "cx_data_port_4_status : " << cx_data_port_4_status.cmd_number << std::endl;
    std::cout << "cx_dev : " << cx_dev.cmd_number << std::endl;
    std::cout << "cx_dynamic_ip : " << cx_dynamic_ip.cmd_number << std::endl;
    std::cout << "cx_environmental_processor_status : " << cx_environmental_processor_status.cmd_number << std::endl;
    std::cout << "cx_ethernet_status : " << cx_ethernet_status.cmd_number << std::endl;
    std::cout << "cx_global_status : " << cx_global_status.cmd_number << std::endl;
    std::cout << "cx_gps_satellites : " << cx_gps_satellites.cmd_number << std::endl;
    std::cout << "cx_gps_status : " << cx_gps_status.cmd_number << std::endl;
    std::cout << "cx_pll_status : " << cx_pll_status.cmd_number << std::endl;
    std::cout << "cx_power_supply_status : " << cx_power_supply_status.cmd_number << std::endl;
    std::cout << "cx_qv : " << cx_qv.cmd_number << std::endl;
    std::cout << "cx_serial_interface_1_status : " << cx_serial_interface_1_status.cmd_number << std::endl;
    std::cout << "cx_serial_interface_2_status : " << cx_serial_interface_2_status.cmd_number << std::endl;
    std::cout << "cx_serial_interface_3_status : " << cx_serial_interface_3_status.cmd_number << std::endl;
    std::cout << "cx_serial_sensor_status : " << cx_serial_sensor_status.cmd_number << std::endl;
    std::cout << "cx_thread_status : " << cx_thread_status.cmd_number << std::endl;
    std::cout << "cy_arp_status : " << cy_arp_status.cmd_number << std::endl;
    std::cout << "cy_auxiliary_board_status : " << cy_auxiliary_board_status.cmd_number << std::endl;
    std::cout << "cy_environmental_processor_status : " << cy_environmental_processor_status.cmd_number << std::endl;
    std::cout << "cy_gps_satellites : " << cy_gps_satellites.cmd_number << std::endl;
    std::cout << "cy_serial_sensor_status : " << cy_serial_sensor_status.cmd_number << std::endl;
    std::cout << "cy_thread_status : " << cy_thread_status.cmd_number << std::endl;
    std::cout << "cz_humidity_and_external_temperature : " << cz_humidity_and_external_temperature.cmd_number << std::endl;
    std::cout << "cz_internal_temperature_measurement : " << cz_internal_temperature_measurement.cmd_number << std::endl;
    std::cout << "qdp_crc : " << qdp_crc.cmd_number << std::endl;
    std::cout << "qdp_header : " << qdp_header.cmd_number << std::endl;
    std::cout << std::hex << std::endl;*/
}



//TODO: c2_samass, c2_rqamass, c2_amass

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
