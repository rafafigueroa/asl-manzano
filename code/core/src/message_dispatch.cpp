// Manzano software

#include "message_dispatch.h"

namespace mzn {

// utility private methods
// -------------------------------------------------------------------------- //
template <typename M>
std::array<uint8_t, 4>
MessageDispatch::calc_crc(M const & msg) {

    uint32_t constexpr mask_lsbyte(0xFF);

    uint32_t crc_msbyte = 0;
    uint32_t crc = 0;
    uint32_t crc_table_index = 0;

    // for every byte on the message, create/update the
    // cyclic redundancy code
    // to be sent to the digitizers
    // the code follows the digitizer's communication protocol manual
    // and dp writer's manual

    for (int i = qdp_begin_header_; i < msg.size(); i++) {

        uint32_t const msg_byte( msg[i] );
        crc_table_index = (crc_msbyte ^ msg_byte) & mask_lsbyte;
        crc = (crc << 8) ^ crc_table_[crc_table_index];
        crc_msbyte = crc >> 24;
    }

    std::array<uint8_t, 4> crc_array;
    crc_array[0] = static_cast<uint8_t>( (crc >> 24) & mask_lsbyte );
    crc_array[1] = static_cast<uint8_t>( (crc >> 16) & mask_lsbyte );
    crc_array[2] = static_cast<uint8_t>( (crc >> 8)  & mask_lsbyte );
    crc_array[3] = static_cast<uint8_t>(crc & mask_lsbyte);

    return crc_array;
}

// -------------------------------------------------------------------------- //
Qdp::Message MessageDispatch::create_qdp_msg(ConnectionHandler & q_port,
                                             Command const & cmd,
                                             bool const to_send /*yes*/) const {

    // ----------- setup header ----------- //
    if (to_send) q_port.inc_sequence_number();

    QdpHeader qdp_header;

    qdp_header.command_number( cmd.cmd_number() );
    qdp_header.firmware_version(q_port.protocol_version);
    qdp_header.length( cmd.cmd_data_size() );
    qdp_header.sequence_number( q_port.sequence_number() );
    qdp_header.acknowledge_number( q_port.acknowledge_number() );

    // ----------- create an empty message ----------- //
    Qdp::Message msg_send(qdp_begin_cmd_data_ + qdp_header.length());

    // ----------- put the header into the msg ----------- //
    qdp_header.data_to_msg(msg_send, qdp_begin_header_);

    // ----------- put the data into the msg ----------- //
    cmd.data_to_msg(msg_send, qdp_begin_cmd_data_);

    // ----------- do & save the crc calculation ----------- //
    QdpCrc qdp_crc;
    qdp_crc.crc( calc_crc(msg_send) );

    // ----------- put the crc calc into the msg ----------- //
    qdp_crc.data_to_msg(msg_send, qdp_begin_crc_);

    return msg_send;
}

// -------------------------------------------------------------------------- //
void MessageDispatch::send_recv(ConnectionHandler & q_port,
                                Command const & cmd_send,
                                Command & cmd_recv,
                                bool const print /*= true*/) {

    // connects if not connected, has own lock_guard
    q_port.connect();

    // lock this function for one thread at a time
    std::lock_guard<std::mutex> send_recv_lock(send_recv_mutex_);

    auto constexpr max_retry = 3;
    // only retry when there is an error with the send_recv at the uc
    // if the msg gets received and there is an error with it, throws
    for (int retry = 0; retry < max_retry; retry++) {

        // ------------------- preparation SEND ----------------------------- //
        if (print) {
            std::cout << "\n --------- sending cmd: --------- \n";
            std::cout << cmd_send;
        }

        Qdp::Message msg_send = create_qdp_msg(q_port, cmd_send);

        // ----------------- UdpConnection msg return size ------------------ //
        uint16_t msg_return_size;

        if ( cmd_recv.multi_cmd() ) {
            // most multi commands size are not known at run time.
            // mtu : maximum transmittable unit in the digitizer.
            msg_return_size = mtu_;

        } else if (cmd_recv.cmd_data_size() == 0) {

            // the expected recv msg is c1_ack, more space is needed
            // just in case a c1_cerr
            auto constexpr c1_cerr_cmd_data_size = 2;
            msg_return_size = qdp_begin_cmd_data_ + c1_cerr_cmd_data_size ;

        } else {

            // calculation for commands where the data size is known
            msg_return_size = qdp_begin_cmd_data_ + cmd_recv.cmd_data_size();
        }

        // ----------------- UdpConnection send and receive ----------------- //
        Qdp::Message msg_recv;
        msg_recv.resize(msg_return_size);

        try {

            q_port.uc.send_recv(msg_send, msg_recv);

        } catch(InfoException & e) {

            std::cerr << std::endl << "caught @MessageDispatch::send_recv";
            std::cerr << e.what();

            if (retry + 1 != max_retry) {
                continue; // try again
            } else {
                throw WarningException(
                    "MessageDispatch",
                    "send_recv",
                    "could not send_recv msg on udp connection");
            }
        }

        // check all the things

        // ----------------- has minimum size for qdp packet? --------------- //
        if (msg_recv.size() < qdp_begin_cmd_data_) {

            std::stringstream ss;
            stream_msg(msg_recv, ss);

            throw WarningException("MessageDispatch",
                                   "send_recv",
                                   std::string("expected a cmd, got: ")
                                   + ss.str() );
        }

        // ------------------- create header from msg ----------------------- //
        QdpHeader qdp_header;
        qdp_header.msg_to_data(msg_recv, qdp_begin_header_);

        // ----------------------- received cerr? --------------------------- //
        C1Cerr cmd_cerr;
        if ( cmd_cerr.cmd_number() == qdp_header.command_number() ) {

            cmd_cerr.msg_to_data(msg_recv, qdp_begin_cmd_data_);

            std::stringstream ss;
            ss << cmd_cerr.error_code;

            throw WarningException( "MessageDispatch",
                                    "send_recv",
                                     std::string("received c1_cerr")
                                     + ss.str() );
        }

        // ----------------------- wrong command? --------------------------- //
        if ( cmd_recv.cmd_number() != qdp_header.command_number() ) {

            std::stringstream ss;
            ss << std::endl << std::showbase << "wrong command received"
               << "\nexpected: " << std::hex << cmd_recv.cmd_number()
               << " got: " << qdp_header.command_number() << std::dec
               << std::noshowbase << "msg recv: " << std::endl;

            stream_msg(msg_recv, ss);

            throw WarningException( "MessageDispatch",
                                    "send_recv",
                                    ss.str() );
        }

        // ------------------------- wrong crc? ----------------------------- //
        QdpCrc qdp_crc;
        qdp_crc.msg_to_data(msg_recv, qdp_begin_crc_);

        auto const crc_recv_calc = calc_crc(msg_recv);

        if ( crc_recv_calc != qdp_crc.crc() ) {

            std::stringstream ss;
            ss << std::endl << std::showbase << "wrong crc on command: "
               << qdp_header.command_number() << std::dec
               << std::noshowbase << "msg recv: " << std::endl;

            stream_msg(msg_recv, ss);

            throw WarningException( "MessageDispatch",
                                    "send_recv",
                                    ss.str() );
        }

        // got a message indicating compatibility with the expected cmd_recv
        cmd_recv.msg_to_data(msg_recv, qdp_begin_cmd_data_);

        if (print) {
            std::cout << "\n --------- received cmd: --------- \n";
            std::cout << cmd_recv;
        }

        // success
        break;

    } // for retry

}

// -------------------------------------------------------------------------- //
void MessageDispatch::data_recv(Digitizer & q,
                                DtOpen const & cmd_send,
                                Qdp::Message & data_msg,
                                bool const print /*= true*/) const {

    /*
    TODO: rewrite with data command

    if (not q.port_control.status.connected) q.port_control.connect();
    if (not q.port_data.status.connected) q.port_data.connect();

    std::cout << "\nlets get some data";

    if (print) {
        std::cout << "\n --------- sending cmd: --------- \n";
        std::cout << cmd_send;
    }

    Qdp::Message msg_send = create_qdp_msg(q.port_data, cmd_send);

    // send it in the udp connection to the digitizer
    std::cout << std::endl << "sending to control" << std::endl;
    q.port_data.uc.send(msg_send);

    std::cout << std::endl << "checking for cerr" << std::endl;
    // some send only cmds can receive a cerr if there is a problem
    Qdp::Message msg_recv;
    q.port_data.uc.recv(msg_recv, cerr_qdp_message_size_);

    if (msg_recv.size() > 0) {

        // -------------- save msg to header -------------- //
        QdpHeader qdp_header;
        qdp_header.msg_to_data(msg_recv, qdp_begin_header_);

        // -------------- received ack? -------------- //
        C1Cack cmd_cack;
        if ( cmd_cack.cmd_number() == qdp_header.command_number() ) {
            if (print) std::cout << "\n ### cack recv ### \n";
        }

        // -------------- received cerr? -------------- //
        C1Cerr cmd_cerr;
        if ( cmd_cerr.cmd_number() == qdp_header.command_number() ) {

            cmd_cerr.msg_to_data(msg_recv, qdp_begin_cmd_data_);
            std::cerr << cmd_cerr;

            throw message_received_error("received c1_cerr after sent only msg",
                                         cmd_send.cmd_number());
        }
    }   // -------------- UdpConnection msg return size -------------- //

    std::cout << std::endl << "____receiving from data port_____" << std::endl;
    uint16_t msg_return_size = mtu_;
    data_msg = q.port_data.uc.recv(msg_return_size);
    */
}

// initialization of a static constexpr variable
// -------------------------------------------------------------------------- //
std::array<uint32_t const, 256> const MessageDispatch::crc_table_ {{
        0, // 0
        1443300200, // 1
        2886600400, // 2
        4194895288, // 3
        236654280, // 4
        1478233504, // 5
        2719287320, // 6
        4094823280, // 7
        473308560, // 8
        1244733176, // 9
        2956467008, // 10
        3862894632, // 11
        304943960, // 12
        1143607344, // 13
        3189970312, // 14
        3894679264, // 15
        946617120, // 16
        1852520520, // 17
        2489466352, // 18
        3261415064, // 19
        913782248, // 20
        1617966720, // 21
        2591634232, // 22
        3430821968, // 23
        609887920, // 24
        1918707160, // 25
        2287214688, // 26
        3729990408, // 27
        708913272, // 28
        2084973328, // 29
        2253336232, // 30
        3494391232, // 31
        1893234240, // 32
        652178728, // 33
        3705041040, // 34
        2328982520, // 35
        2126739592, // 36
        683965408, // 37
        3536682584, // 38
        2227862832, // 39
        1827564496, // 40
        988375224, // 41
        3235933440, // 42
        2531749480, // 43
        1660249368, // 44
        888301168, // 45
        3472581576, // 46
        2566676640, // 47
        1219775840, // 48
        515067400, // 49
        3837414320, // 50
        2998749400, // 51
        1185891240, // 52
        279462080, // 53
        3936437624, // 54
        3165013520, // 55
        1417826544, // 56
        42292120, // 57
        4169946656, // 58
        2928366920, // 59
        1520000568, // 60
        211705168, // 61
        4137113832, // 62
        2693815168, // 63
        3786468480, // 64
        3082285032, // 65
        1304357456, // 66
        465168696, // 67
        4021019208, // 68
        3115114784, // 69
        1134945432, // 70
        362997744, // 71
        4253479184, // 72
        2877420152, // 73
        1367930816, // 74
        126874792, // 75
        4087218136, // 76
        2778397872, // 77
        1603533064, // 78
        160758368, // 79
        3655128992, // 80
        2413548744, // 81
        1976750448, // 82
        601215512, // 83
        3620198760, // 84
        2176899584, // 85
        2076827576, // 86
        768531664, // 87
        3320498736, // 88
        2481834328, // 89
        1776602336, // 90
        1071894408, // 91
        3421619448, // 92
        2650195856, // 93
        1744814632, // 94
        838385984, // 95
        2439551680, // 96
        3345979816, // 97
        1030134800, // 98
        1801559928, // 99
        2675151880, // 100
        3379861344, // 101
        863867608, // 102
        1702531504, // 103
        2371782480, // 104
        3680076856, // 105
        558924160, // 106
        2002223848, // 107
        2202372504, // 108
        3577907952, // 109
        793481032, // 110
        2035059744, // 111
        2835653088, // 112
        4278428296, // 113
        84584240, // 114
        1393402968, // 115
        2803871528, // 116
        4044926016, // 117
        185707000, // 118
        1561766544, // 119
        3040001136, // 120
        3811950360, // 121
        423410336, // 122
        1329314248, // 123
        3140072120, // 124
        3979260368, // 125
        388478056, // 126
        1092663040, // 127
        2506545768, // 128
        3277969664, // 129
        963173560, // 130
        1869602768, // 131
        2608714912, // 132
        3447379912, // 133
        930337392, // 134
        1635045656, // 135
        2303772664, // 136
        3747071120, // 137
        626966824, // 138
        1935262272, // 139
        2269890864, // 140
        3511470680, // 141
        725995488, // 142
        2101529736, // 143
        2903171400, // 144
        4211991072, // 145
        17098648, // 146
        1459873008, // 147
        2735861632, // 148
        4111920360, // 149
        253749584, // 150
        1494805048, // 151
        2973564120, // 152
        3879468976, // 153
        489880072, // 154
        1261828448, // 155
        3207066128, // 156
        3911250296, // 157
        321516736, // 158
        1160705960, // 159
        3854478376, // 160
        3015290688, // 161
        1236314872, // 162
        532130192, // 163
        3953500896, // 164
        3181552008, // 165
        1202431024, // 166
        296527704, // 167
        4186485176, // 168
        2945430224, // 169
        1434892136, // 170
        58831872, // 171
        4153655152, // 172
        2710879256, // 173
        1537063328, // 174
        228244168, // 175
        3721565960, // 176
        2346030176, // 177
        1910280664, // 178
        668701360, // 179
        3553204672, // 180
        2244909736, // 181
        2143788816, // 182
        700488824, // 183
        3252980376, // 184
        2548271600, // 185
        1844087880, // 186
        1005424416, // 187
        3489629264, // 188
        2583201592, // 189
        1676771968, // 190
        905347560, // 191
        1960195816, // 192
        584136064, // 193
        3638046776, // 194
        2396992336, // 195
        2060269600, // 196
        751450952, // 197
        3603119856, // 198
        2160344472, // 199
        1759521656, // 200
        1055336464, // 201
        3303943592, // 202
        2464755392, // 203
        1727735216, // 204
        821831384, // 205
        3405063008, // 206
        2633113608, // 207
        1287261640, // 208
        448597664, // 209
        3769895704, // 210
        3065186416, // 211
        1117848320, // 212
        346423400, // 213
        4004447696, // 214
        3098019512, // 215
        1351356504, // 216
        109777712, // 217
        4236383880, // 218
        2860848608, // 219
        1586962064, // 220
        143662584, // 221
        4070119488, // 222
        2761825064, // 223
        68042920, // 224
        1376338880, // 225
        2818590328, // 226
        4261889296, // 227
        169168480, // 228
        1544703240, // 229
        2786805936, // 230
        4028386264, // 231
        406347064, // 232
        1312775760, // 233
        3023461352, // 234
        3794884736, // 235
        371414000, // 236
        1076121752, // 237
        3123533088, // 238
        3962197576, // 239
        1013087112, // 240
        1785034976, // 241
        2423029080, // 242
        3328933424, // 243
        846820672, // 244
        1686009384, // 245
        2658628496, // 246
        3362812152, // 247
        542402072, // 248
        1985176944, // 249
        2354733256, // 250
        3663553440, // 251
        776956112, // 252
        2018012088, // 253
        2185326080, // 254
        3561385320 // 255
    }}; // <- crc_table

} // << mzn

