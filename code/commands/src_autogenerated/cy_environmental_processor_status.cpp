#include "cy_environmental_processor_status.h"

namespace mzn {
CyEnvironmentalProcessorStatus::CyEnvironmentalProcessorStatus():
    Command(0, 232),
    initial_vco(),
    time_error(),
    best_vco(),
    ticks_since_last_update(),
    km(),
    state(),
    spare_1(),
    ep_serial_number(),
    processor_id(),
    seconds_since_boot(),
    seconds_since_resync(),
    number_of_resyncs(),
    q330_communication_errors(),
    ep_communication_errors(),
    spare_2(),
    sdi_devices_active(),
    firmware_version(),
    environmental_flags(),
    analog_channels(),
    ep_model(),
    ep_revision(),
    gains(),
    input_voltage_to_ep("x0.1V"),
    internal_humidity("%"),
    built_in_pressure("micro bar"),
    internal_temperature("x0.1C"),
    ad_input_channel_1("counts"),
    ad_input_channel_2("counts"),
    ad_input_channel_3("counts"),
    spare_3(),
    sdi1_sdi_address(),
    sdi1_phase(),
    sdi1_driver_id(),
    sdi1_spare_1(),
    sdi1_sensor_model(),
    sdi1_serial_number(),
    sdi1_spare_3(),
    sdi1_sensor_version(),
    sdi1_spare_4(),
    sdi2_sdi_address(),
    sdi2_phase(),
    sdi2_driver_id(),
    sdi2_spare_1(),
    sdi2_sensor_model(),
    sdi2_serial_number(),
    sdi2_spare_3(),
    sdi2_sensor_version(),
    sdi2_spare_4(),
    sdi3_sdi_address(),
    sdi3_phase(),
    sdi3_driver_id(),
    sdi3_spare_1(),
    sdi3_sensor_model(),
    sdi3_serial_number(),
    sdi3_spare_3(),
    sdi3_sensor_version(),
    sdi3_spare_4(),
    sdi4_sdi_address(),
    sdi4_phase(),
    sdi4_driver_id(),
    sdi4_spare_1(),
    sdi4_sensor_model(),
    sdi4_serial_number(),
    sdi4_spare_3(),
    sdi4_sensor_version(),
    sdi4_spare_4(),
    adc_serial_number(),
    adc_model(),
    adc_revision(),
    adc_spare_1(),
    adc_spare_2(),
    adc_spare_3(),
    adc_spare_4(),
    adc_spare_5() { }

uint16_t CyEnvironmentalProcessorStatus::msg_to_data(std::vector<uint8_t> const & msg,
                                                     uint16_t mf_begin) {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyEnvironmentalProcessorStatus",
            "msg_to_data",
            "msg size " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = initial_vco.msg_to_data(msg, mf_begin);
    mf_begin = time_error.msg_to_data(msg, mf_begin);
    mf_begin = best_vco.msg_to_data(msg, mf_begin);
    mf_begin = ticks_since_last_update.msg_to_data(msg, mf_begin);
    mf_begin = km.msg_to_data(msg, mf_begin);
    mf_begin = state.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = ep_serial_number.msg_to_data(msg, mf_begin);
    mf_begin = processor_id.msg_to_data(msg, mf_begin);
    mf_begin = seconds_since_boot.msg_to_data(msg, mf_begin);
    mf_begin = seconds_since_resync.msg_to_data(msg, mf_begin);
    mf_begin = number_of_resyncs.msg_to_data(msg, mf_begin);
    mf_begin = q330_communication_errors.msg_to_data(msg, mf_begin);
    mf_begin = ep_communication_errors.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sdi_devices_active.msg_to_data(msg, mf_begin);
    mf_begin = firmware_version.msg_to_data(msg, mf_begin);
    mf_begin = environmental_flags.msg_to_data(msg, mf_begin);
    mf_begin = analog_channels.msg_to_data(msg, mf_begin);
    mf_begin = ep_model.msg_to_data(msg, mf_begin);
    mf_begin = ep_revision.msg_to_data(msg, mf_begin);
    mf_begin = gains.msg_to_data(msg, mf_begin);
    mf_begin = input_voltage_to_ep.msg_to_data(msg, mf_begin);
    mf_begin = internal_humidity.msg_to_data(msg, mf_begin);
    mf_begin = built_in_pressure.msg_to_data(msg, mf_begin);
    mf_begin = internal_temperature.msg_to_data(msg, mf_begin);
    mf_begin = ad_input_channel_1.msg_to_data(msg, mf_begin);
    mf_begin = ad_input_channel_2.msg_to_data(msg, mf_begin);
    mf_begin = ad_input_channel_3.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = sdi1_sdi_address.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_phase.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_driver_id.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_spare_1.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_sensor_model.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_serial_number.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_spare_3.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_sensor_version.msg_to_data(msg, mf_begin);
    mf_begin = sdi1_spare_4.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_sdi_address.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_phase.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_driver_id.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_spare_1.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_sensor_model.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_serial_number.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_spare_3.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_sensor_version.msg_to_data(msg, mf_begin);
    mf_begin = sdi2_spare_4.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_sdi_address.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_phase.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_driver_id.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_spare_1.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_sensor_model.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_serial_number.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_spare_3.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_sensor_version.msg_to_data(msg, mf_begin);
    mf_begin = sdi3_spare_4.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_sdi_address.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_phase.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_driver_id.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_spare_1.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_sensor_model.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_serial_number.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_spare_3.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_sensor_version.msg_to_data(msg, mf_begin);
    mf_begin = sdi4_spare_4.msg_to_data(msg, mf_begin);
    mf_begin = adc_serial_number.msg_to_data(msg, mf_begin);
    mf_begin = adc_model.msg_to_data(msg, mf_begin);
    mf_begin = adc_revision.msg_to_data(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type

    return mf_begin;
}

uint16_t CyEnvironmentalProcessorStatus::data_to_msg(std::vector<uint8_t> & msg,
                                                     uint16_t mf_begin) const {

    if ( msg.size()  < cmd_data_size_ + mf_begin) {
        throw FatalException(
            "CyEnvironmentalProcessorStatus",
            "msg_to_data",
            "msg size: " + std::to_string( msg.size() )
            + ", mf_begin " + std::to_string(mf_begin)
            + ", cmd_data_size_ " + std::to_string(cmd_data_size_)
        );
    }

    mf_begin = initial_vco.data_to_msg(msg, mf_begin);
    mf_begin = time_error.data_to_msg(msg, mf_begin);
    mf_begin = best_vco.data_to_msg(msg, mf_begin);
    mf_begin = ticks_since_last_update.data_to_msg(msg, mf_begin);
    mf_begin = km.data_to_msg(msg, mf_begin);
    mf_begin = state.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = ep_serial_number.data_to_msg(msg, mf_begin);
    mf_begin = processor_id.data_to_msg(msg, mf_begin);
    mf_begin = seconds_since_boot.data_to_msg(msg, mf_begin);
    mf_begin = seconds_since_resync.data_to_msg(msg, mf_begin);
    mf_begin = number_of_resyncs.data_to_msg(msg, mf_begin);
    mf_begin = q330_communication_errors.data_to_msg(msg, mf_begin);
    mf_begin = ep_communication_errors.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = sdi_devices_active.data_to_msg(msg, mf_begin);
    mf_begin = firmware_version.data_to_msg(msg, mf_begin);
    mf_begin = environmental_flags.data_to_msg(msg, mf_begin);
    mf_begin = analog_channels.data_to_msg(msg, mf_begin);
    mf_begin = ep_model.data_to_msg(msg, mf_begin);
    mf_begin = ep_revision.data_to_msg(msg, mf_begin);
    mf_begin = gains.data_to_msg(msg, mf_begin);
    mf_begin = input_voltage_to_ep.data_to_msg(msg, mf_begin);
    mf_begin = internal_humidity.data_to_msg(msg, mf_begin);
    mf_begin = built_in_pressure.data_to_msg(msg, mf_begin);
    mf_begin = internal_temperature.data_to_msg(msg, mf_begin);
    mf_begin = ad_input_channel_1.data_to_msg(msg, mf_begin);
    mf_begin = ad_input_channel_2.data_to_msg(msg, mf_begin);
    mf_begin = ad_input_channel_3.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = sdi1_sdi_address.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_phase.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_driver_id.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_spare_1.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_sensor_model.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_serial_number.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_spare_3.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_sensor_version.data_to_msg(msg, mf_begin);
    mf_begin = sdi1_spare_4.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_sdi_address.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_phase.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_driver_id.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_spare_1.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_sensor_model.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_serial_number.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_spare_3.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_sensor_version.data_to_msg(msg, mf_begin);
    mf_begin = sdi2_spare_4.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_sdi_address.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_phase.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_driver_id.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_spare_1.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_sensor_model.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_serial_number.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_spare_3.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_sensor_version.data_to_msg(msg, mf_begin);
    mf_begin = sdi3_spare_4.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_sdi_address.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_phase.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_driver_id.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_spare_1.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_sensor_model.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_serial_number.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_spare_3.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_sensor_version.data_to_msg(msg, mf_begin);
    mf_begin = sdi4_spare_4.data_to_msg(msg, mf_begin);
    mf_begin = adc_serial_number.data_to_msg(msg, mf_begin);
    mf_begin = adc_model.data_to_msg(msg, mf_begin);
    mf_begin = adc_revision.data_to_msg(msg, mf_begin);
    mf_begin = mf_begin + 2; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type
    mf_begin = mf_begin + 4; // ignore type

    return mf_begin;
}

std::ostream & CyEnvironmentalProcessorStatus::os_print(std::ostream & cmd_os) const {
    cmd_os << "\n --- CY_ENVIRONMENTAL_PROCESSOR_STATUS ---  \n";

    cmd_os << "\ninitial_vco: "; cmd_os << initial_vco;

    cmd_os << "\ntime_error: "; cmd_os << time_error;

    cmd_os << "\nbest_vco: "; cmd_os << best_vco;

    cmd_os << "\nticks_since_last_update: "; cmd_os << ticks_since_last_update;

    cmd_os << "\nkm: "; cmd_os << km;

    cmd_os << "\nstate: "; cmd_os << state;

    cmd_os << "\nep_serial_number: "; cmd_os << ep_serial_number;

    cmd_os << "\nprocessor_id: "; cmd_os << processor_id;

    cmd_os << "\nseconds_since_boot: "; cmd_os << seconds_since_boot;

    cmd_os << "\nseconds_since_resync: "; cmd_os << seconds_since_resync;

    cmd_os << "\nnumber_of_resyncs: "; cmd_os << number_of_resyncs;

    cmd_os << "\nq330_communication_errors: "; cmd_os << q330_communication_errors;

    cmd_os << "\nep_communication_errors: "; cmd_os << ep_communication_errors;

    cmd_os << "\nsdi_devices_active: "; cmd_os << sdi_devices_active;

    cmd_os << "\nfirmware_version: "; cmd_os << firmware_version;

    cmd_os << "\nenvironmental_flags: "; cmd_os << environmental_flags;

    cmd_os << "\nanalog_channels: "; cmd_os << analog_channels;

    cmd_os << "\nep_model: "; cmd_os << ep_model;

    cmd_os << "\nep_revision: "; cmd_os << ep_revision;

    cmd_os << "\ngains: "; cmd_os << gains;

    cmd_os << "\ninput_voltage_to_ep: "; cmd_os << input_voltage_to_ep;

    cmd_os << "\ninternal_humidity: "; cmd_os << internal_humidity;

    cmd_os << "\nbuilt_in_pressure: "; cmd_os << built_in_pressure;

    cmd_os << "\ninternal_temperature: "; cmd_os << internal_temperature;

    cmd_os << "\nad_input_channel_1: "; cmd_os << ad_input_channel_1;

    cmd_os << "\nad_input_channel_2: "; cmd_os << ad_input_channel_2;

    cmd_os << "\nad_input_channel_3: "; cmd_os << ad_input_channel_3;

    cmd_os << "\nsdi1_sdi_address: "; cmd_os << sdi1_sdi_address;

    cmd_os << "\nsdi1_phase: "; cmd_os << sdi1_phase;

    cmd_os << "\nsdi1_driver_id: "; cmd_os << sdi1_driver_id;

    cmd_os << "\nsdi1_spare_1: "; cmd_os << sdi1_spare_1;

    cmd_os << "\nsdi1_sensor_model: "; cmd_os << sdi1_sensor_model;

    cmd_os << "\nsdi1_serial_number: "; cmd_os << sdi1_serial_number;

    cmd_os << "\nsdi1_spare_3: "; cmd_os << sdi1_spare_3;

    cmd_os << "\nsdi1_sensor_version: "; cmd_os << sdi1_sensor_version;

    cmd_os << "\nsdi1_spare_4: "; cmd_os << sdi1_spare_4;

    cmd_os << "\nsdi2_sdi_address: "; cmd_os << sdi2_sdi_address;

    cmd_os << "\nsdi2_phase: "; cmd_os << sdi2_phase;

    cmd_os << "\nsdi2_driver_id: "; cmd_os << sdi2_driver_id;

    cmd_os << "\nsdi2_spare_1: "; cmd_os << sdi2_spare_1;

    cmd_os << "\nsdi2_sensor_model: "; cmd_os << sdi2_sensor_model;

    cmd_os << "\nsdi2_serial_number: "; cmd_os << sdi2_serial_number;

    cmd_os << "\nsdi2_spare_3: "; cmd_os << sdi2_spare_3;

    cmd_os << "\nsdi2_sensor_version: "; cmd_os << sdi2_sensor_version;

    cmd_os << "\nsdi2_spare_4: "; cmd_os << sdi2_spare_4;

    cmd_os << "\nsdi3_sdi_address: "; cmd_os << sdi3_sdi_address;

    cmd_os << "\nsdi3_phase: "; cmd_os << sdi3_phase;

    cmd_os << "\nsdi3_driver_id: "; cmd_os << sdi3_driver_id;

    cmd_os << "\nsdi3_spare_1: "; cmd_os << sdi3_spare_1;

    cmd_os << "\nsdi3_sensor_model: "; cmd_os << sdi3_sensor_model;

    cmd_os << "\nsdi3_serial_number: "; cmd_os << sdi3_serial_number;

    cmd_os << "\nsdi3_spare_3: "; cmd_os << sdi3_spare_3;

    cmd_os << "\nsdi3_sensor_version: "; cmd_os << sdi3_sensor_version;

    cmd_os << "\nsdi3_spare_4: "; cmd_os << sdi3_spare_4;

    cmd_os << "\nsdi4_sdi_address: "; cmd_os << sdi4_sdi_address;

    cmd_os << "\nsdi4_phase: "; cmd_os << sdi4_phase;

    cmd_os << "\nsdi4_driver_id: "; cmd_os << sdi4_driver_id;

    cmd_os << "\nsdi4_spare_1: "; cmd_os << sdi4_spare_1;

    cmd_os << "\nsdi4_sensor_model: "; cmd_os << sdi4_sensor_model;

    cmd_os << "\nsdi4_serial_number: "; cmd_os << sdi4_serial_number;

    cmd_os << "\nsdi4_spare_3: "; cmd_os << sdi4_spare_3;

    cmd_os << "\nsdi4_sensor_version: "; cmd_os << sdi4_sensor_version;

    cmd_os << "\nsdi4_spare_4: "; cmd_os << sdi4_spare_4;

    cmd_os << "\nadc_serial_number: "; cmd_os << adc_serial_number;

    cmd_os << "\nadc_model: "; cmd_os << adc_model;

    cmd_os << "\nadc_revision: "; cmd_os << adc_revision;
    cmd_os << std::endl;

    return cmd_os;
}

} // end namespace