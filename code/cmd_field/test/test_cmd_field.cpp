#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <bitset>
#include <limits>
#include <cmath>

#include "gtest/gtest.h"
#include "cmd_field.h"
#include "cmd_field_bitmap.h"
#include "cmd_field_bitmap_types.h"
#include "cmd_field_array.h"
#include "cmd_field_array_byte.h"
#include "cmd_field_array_char.h"
#include "cmd_field_array_ubyte.h"
#include "cmd_field_container.h"
#include "cmd_field_duration.h"
#include "cmd_field_pstring.h"
#include "cmd_field_hex.h"
#include "cmd_field_ignore.h"
#include "cmd_field_ip.h"
#include "cmd_field_cal_amplitude.h"
#include "cmd_field_time.h"


#include "testing_bitmaps.h"

// -------------------------------------------------------------------------- //
TEST(test_mzn, invariants) {
    int i = 7;
    char * pi = reinterpret_cast<char *>(&i);
    ASSERT_EQ(pi[0],'\x7') << "*** architecture is not little endian";

    // TODO: check for the specific float/double IEEE coding
    ASSERT_EQ(4, sizeof(float) ) << "*** float is not 4 byte size";
    ASSERT_EQ(8, sizeof(double) ) << "*** double is not 8 byte size";
}

// -------------------------------------------------------------------------- //
class FixtureCmdField : public ::testing::Test {
public:
    mzn::CmdField<bool, 1> bl;
    mzn::CmdField<uint8_t, 1> uc;
    mzn::CmdField<int8_t, 1> c;
    mzn::CmdField<uint16_t, 2> ui16;
    mzn::CmdField<int16_t, 2> i16;
    mzn::CmdField<uint32_t, 4> ui32;
    mzn::CmdField<int32_t, 4> i32;
    mzn::CmdField<uint64_t, 8> ui64;
    mzn::CmdField<int64_t, 8> i64;
    mzn::CmdField<float, 4> f;

    mzn::CmdField<bool> tbl;
    mzn::CmdField<uint8_t> tuc;
    mzn::CmdField<int8_t> tc;
    mzn::CmdField<uint16_t> tui16;
    mzn::CmdField<int16_t> ti16;
    mzn::CmdField<uint32_t> tui32;
    mzn::CmdField<int32_t> ti32;
    mzn::CmdField<uint64_t> tui64;
    mzn::CmdField<int64_t> ti64;
    mzn::CmdField<float> tf;
    mzn::CmdField<uint64_t, 6> ui48;

    mzn::CmdField<std::bitset<16>, 2> raw_ui16bitset;
    mzn::CmdFieldBitmap<1> ui8bitmap;
    mzn::CmdFieldBitmap<2> ui16bitmap;
    mzn::CmdFieldBitmap<4> ui32bitmap;
    mzn::BmFgFix bm_fg_fix;

    mzn::CmdFieldPstring<6> pstring6;
    mzn::CmdFieldPstring<10> pstring10;
    mzn::CmdFieldPstring<12> pstring12;
    mzn::CmdFieldPstring<14> pstring14;

    mzn::CmdFieldHex<uint8_t> ui8hex;
    mzn::CmdFieldHex<uint16_t> ui16hex;
    mzn::CmdFieldHex<uint32_t> ui32hex;
    mzn::CmdFieldHex<uint64_t> ui64hex;

    mzn::CmdFieldIgnore<uint8_t> ui8ignore;
    mzn::CmdFieldIgnore<uint16_t> ui16ignore;
    mzn::CmdFieldIgnore<uint32_t> ui32ignore;
    mzn::CmdFieldIgnore<uint64_t> ui64ignore;

    mzn::CmdFieldIp ui32ip;

    mzn::CmdFieldTime<uint32_t> ui32time;
    mzn::CmdFieldTime<uint16_t> ui16time;

    mzn::CmdFieldDuration<uint16_t> ui16duration;
    mzn::CmdFieldDuration<uint16_t, std::deci> uint16_deci_duration;
    mzn::CmdFieldDuration<uint16_t, std::centi> uint16_centi_duration;
    mzn::CmdFieldDuration<uint32_t> ui32duration;
    mzn::CmdFieldDuration<float, std::micro> float_us_duration;
    mzn::CmdFieldDuration<float, std::milli> float_ms_duration;

    mzn::CmdFieldContainer<float> cfc_float;

    mzn::CmdFieldCalAmplitude amplitude;
    // the special ones:

    mzn::CmdFieldArray<uint8_t, 16> auc;
    mzn::CmdFieldArray<int8_t, 12> ac;

    mzn::CmdFieldArray<int8_t, 2>  cf_array_plain_int;
    mzn::CmdFieldArray<char, 2>    cf_array_plain_char;
    mzn::CmdFieldArray<uint8_t, 2> cf_array_plain_ubyte;
    mzn::CmdFieldArrayByte<2>      array_byte;
    mzn::CmdFieldArrayChar<2>      array_char;
    mzn::CmdFieldArrayUByte<2>     array_ubyte;

    virtual void SetUp() {
    }
    //virtual void TearDown() {};
};


// fails only if SetUp() fails
// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, cmd_field_setup) {}

TEST_F(FixtureCmdField, cmd_field_accessor_mutator) {

    // assign test values and run a
    // simple test to confirm right name/type
    // and function call operator

    //! POD types
    bl(42);                     EXPECT_EQ( true, bl() );
    bl(0);                      EXPECT_EQ( false, bl() );
    uc(255);                    EXPECT_EQ( 255, uc() );
    c(-128);                    EXPECT_EQ( -128, c() );
    ui16(65'535);               EXPECT_EQ( 65'535, ui16() );
    i16(-32'768);               EXPECT_EQ( -32'768, i16() );
    ui32(4'294'967'295);        EXPECT_EQ( 4'294'967'295, ui32() );
    i32(-2'147'483'648);        EXPECT_EQ( -2'147'483'648, i32() );

    ui64(18'446'744'073'709'551'615u);
    EXPECT_EQ( 18'446'744'073'709'551'615u, ui64() );

   // in my architecture, the 808 at the end of literal
    //makes it out of the signed integers range.
    i64(-9'223'372'036'854'775'807);
    EXPECT_EQ(-9'223'372'036'854'775'807, i64() );

    float v_fmin = std::numeric_limits<float>::min();
    f(v_fmin);          EXPECT_FLOAT_EQ( v_fmin, f() );
    f(3090.091582);     EXPECT_FLOAT_EQ( 3090.091582, f() );

    //double v_dmin = std::numeric_limits<double>::min();
    //d(v_dmin);          EXPECT_DOUBLE_EQ( v_dmin, d() );

    auc(std::array<uint8_t, 16> {{11, 12, 13, 14, 15, 16,
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1}});

    bool auc_eq = (auc.data() == std::array<uint8_t, 16> {{
        11, 12, 13, 14, 15, 16, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}});

    EXPECT_TRUE(auc_eq);

    ac(std::array<int8_t, 12> {{11, 12,
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1}});

    bool ac_eq = (ac.data() == std::array<int8_t, 12> {{
            11, 12, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}});

    EXPECT_TRUE(ac_eq);

    //TODO: gps status map

    //! Custom types
    ui8hex(0xFF);                       EXPECT_EQ( 0xFF, ui8hex() );
    ui16hex(0xFFFF);                    EXPECT_EQ( 0xFFFF, ui16hex() );
    ui32hex(0x10000215);                EXPECT_EQ( 0x10000215, ui32hex() );
    ui64hex(0xFFFFFFFFFFFFFFFF);        EXPECT_EQ( 0xFFFFFFFFFFFFFFFF, ui64hex() );

    ui8bitmap( std::bitset<8>(0xFF) );
    EXPECT_EQ( std::bitset<8>(0xFF), ui8bitmap() );

    ui16bitmap( std::bitset<16>(0xFFFF) );
    EXPECT_EQ( std::bitset<16>(0xFFFF), ui16bitmap() );

    ui32bitmap( std::bitset<32>(0x10000215) );
    EXPECT_EQ( std::bitset<32>(0x10000215), ui32bitmap() );

    ui8ignore(215);             EXPECT_EQ( 215, ui8ignore() );
    ui32ip( ui32hex() );        EXPECT_EQ( ui32hex(), ui32ip() );

    auto const tp =
            date::floor<std::chrono::seconds>(
                    std::chrono::system_clock::now() );

    ui32time(tp);               EXPECT_EQ( tp, ui32time() );

    //EXPECT_EQ(ui32time.data, 4294);

    // constructor default init
    EXPECT_EQ( 0, ui16ignore() );
    EXPECT_EQ( 0, ui32ignore() );
    EXPECT_EQ( 0, ui64ignore() );
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, time_and_duration) {

    using namespace mzn;
    using namespace mzn::Time;

    std::stringstream ss;

    ss << std::endl << sys_time_of_day();

    auto const now = SysClock::now();
    ss << std::endl << sys_time_of_day(now);

    auto const five_minutes = std::chrono::minutes(5);
    auto const five_minutes_from_now = now + five_minutes;
    ss << std::endl << sys_time_of_day(five_minutes_from_now);
    ss << std::endl;

    ui32time(now);
    ui32duration(five_minutes);

    // using the ostream<< operator in mzn::Time for cmd_field_time and duration
    ss << "\n" << ui32time;
    ss << "\n" << ui32duration;

    ss << "\nOne Second: " << std::chrono::seconds(1);

    ss << "\n\nsame duration, streamed consistently: ";
    ss << "\nFive Minutes from minutes: " << five_minutes;
    ss << "\nFive Minutes from seconds: " << std::chrono::seconds(60*5);

    ss << "\n\nsame duration, streamed differently using floats and time types: ";
    float_ms_duration( std::chrono::microseconds(20) );
    float_us_duration( std::chrono::microseconds(20) );

    ss << "\nTwenty MicroSeconds: " << float_ms_duration();
    ss << "\nTwenty MicroSeconds: " << float_us_duration();

    // these are equivalent
    auto dc = Minutes<int>(3);
    auto dm = Minutes<int>(1) * 3;
    EXPECT_EQ( dc.count(), dm.count() );

    // demostration of different year quantities (not really a test)

    // Can't use years directly without conversion
    // number of minutes/hours/days in a duration year (not calendar)
    // is not integral

    // all converted/floored to seconds to compare count
    auto calendar_common_year = Seconds<int>( Days<int>(365) );
    auto calendar_leap_year =   Seconds<int>( Days<int>(366) );

    // seconds is ok, since it doesn't take fractions of leap second
    // so there is an integral number of seconds in a year
    // no explicit time_cast needed (floor is a type of time_cast)
    auto duration_year = Seconds<int>( Years<int>(1) );

    // all different
    EXPECT_NE( calendar_common_year.count(), calendar_leap_year.count() );
    EXPECT_NE( calendar_common_year.count(), duration_year.count() );
    EXPECT_NE( duration_year.count(), calendar_leap_year.count() );

    // must cast it to convert Years to Hours/Minutes/Days
    auto duration_year_h   = date::floor< Hours<int> >( Years<int>(1) );
    auto duration_year_min = date::floor< Minutes<int> >( Years<int>(1) );
    auto duration_year_d   = date::floor< Days<int> >( Years<int>(1) );

    // converted to seconds for comparison
    auto seconds_year_h   = Seconds<int>(duration_year_h);
    auto seconds_year_min = Seconds<int>(duration_year_min);
    auto seconds_year_d   = Seconds<int>(duration_year_d);

    // all different, hence the need to cast it if h/min/d are needed from y
    EXPECT_NE( seconds_year_h.count() , seconds_year_min.count() );
    EXPECT_NE( seconds_year_h.count() , seconds_year_d.count() );
    EXPECT_NE( seconds_year_d.count() , seconds_year_min.count() );


    // streaming all together
    Minutes<int> d{}; // 0 minutes

    d += duration_year_min;
    d += Days<int>(2);
    d += Hours<int>(3);
    d += Minutes<int>(4);

    ss << "\n\ncombined approximately 1y_2d_3h_4min: " << d;
    // the combined value shows seconds. Although might be unexpected,
    // is consistent, since the 1y value here is not really a year, is a cast
    // that lost precision. Beware of casting unless is necessary.
    ss << "\ndifferent duration due to year in seconds in operator<<"
       << "\nyear cast to minutes: " << duration_year_min
       << "\nyear in seconds precision: " << duration_year;

    std::cout << ss.str() << "\n" << std::endl;
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, stream_output) {

    auto test_stream_output = [&](std::string intended, auto cf) {
        std::stringstream ss;
        ss << cf;
        EXPECT_STREQ( intended.c_str(), ss.str().c_str() );
    };

    f(3090.090315);             test_stream_output("3090.09", f);
    bl(true);                   test_stream_output("true", bl);
    bl(0);                      test_stream_output("false", bl);
    uc(150);                    test_stream_output("150", uc);

    //! Custom types
    ui8hex(0xFF);
    ui16hex(0xFFFF);
    ui32hex(0x10000215);
    ui64hex(0xFFFFFFFFFFFFFFFF);

    ui8bitmap( std::bitset<8>(0xFF) );
    ui16bitmap( std::bitset<16>(0xFFFF) );
    ui32bitmap( std::bitset<32>(0x10000215) );

    ui8ignore(215);
    ui32ip( ui32hex() );

    test_stream_output("11111111", ui8bitmap);
    test_stream_output("1111111111111111", ui16bitmap);
    test_stream_output("00010000000000000000001000010101", ui32bitmap);

    //test_stream_output("gps status map!", pstring);

    test_stream_output("0XFF", ui8hex);
    test_stream_output("0XFFFF", ui16hex);
    test_stream_output("0X10000215", ui32hex);
    test_stream_output("0XFFFFFFFFFFFFFFFF", ui64hex);

    test_stream_output("", ui8ignore);

    std::stringstream ss;

    ss << std::hex << std::showbase;
    ss << uc << std::endl;
    ss << c << std::endl;
    ss << ui16 << std::endl;
    ss << i16 << std::endl;
    ss << ui32 << std::endl;
    ss << i32 << std::endl;
    ss << f << std::endl;
    ss << i64 << std::endl;
    ss << ui64 << std::endl;
    ss << bl << std::endl;

    ss << std::dec << std::noshowbase;
    ss << uc << std::endl;
    ss << c << std::endl;
    ss << ui16 << std::endl;
    ss << i16 << std::endl;
    ss << ui32 << std::endl;
    ss << i32 << std::endl;
    ss << f << std::endl;
    ss << i64 << std::endl;
    ss << ui64 << std::endl;
    ss << bl << std::endl;
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_to_data_basic) {

    std::vector<uint8_t> msg = {12,
                                123,
                                13, 11,
                                53, 114,
                                124, 111, 234, 12,
                                23, 34, 57, 64,
                                0, 143, 122, 3, 1, 5, 7, 2,
                                234, 143, 122, 32, 81, 85, 70, 2,
                                11};
    auto mf_pos = 0;

    mf_pos = uc.msg_to_data(msg, mf_pos);
    mf_pos = c.msg_to_data(msg, mf_pos);
    mf_pos = ui16.msg_to_data(msg, mf_pos);
    mf_pos = i16.msg_to_data(msg, mf_pos);
    mf_pos = ui32.msg_to_data(msg, mf_pos);
    mf_pos = i32.msg_to_data(msg, mf_pos);
    mf_pos = i64.msg_to_data(msg, mf_pos);
    mf_pos = ui64.msg_to_data(msg, mf_pos);
    mf_pos = bl.msg_to_data(msg, mf_pos);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, data_to_msg_basic) {

    // this_cf_size / accumulation
    uc(255); // 1 / 1
    c(-128); // 1 / 2
    ui16(65'535); // 2 / 4
    i16(-32'768); // 2 / 6
    ui32(4'294); // 4 / 10
    i32(-2'147); // 4 / 14
    i64(234234); // 8 / 22
    ui64(1); // 8 / 30
    bl(false); // 1 / 31
    // total: 31

    std::vector<uint8_t> msg(31, 0);

    auto mf_pos = 0;

    mf_pos = uc.data_to_msg(msg, mf_pos);
    mf_pos = c.data_to_msg(msg, mf_pos);
    mf_pos = ui16.data_to_msg(msg, mf_pos);
    mf_pos = i16.data_to_msg(msg, mf_pos);
    mf_pos = ui32.data_to_msg(msg, mf_pos);
    mf_pos = i32.data_to_msg(msg, mf_pos);
    mf_pos = i64.data_to_msg(msg, mf_pos);
    mf_pos = ui64.data_to_msg(msg, mf_pos);
    mf_pos = bl.data_to_msg(msg, mf_pos);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_time) {

    using namespace mzn::Time;

    ui32time( SysClock::now() );

    auto copy_ui32time = ui32time;

    std::vector<uint8_t> before_msg(4, 1);
    std::vector<uint8_t> after_msg(4, 2);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = ui32time.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = ui32time.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = ui32time.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_ui32time, ui32time);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_unsigned_duration) {

    using namespace mzn::Time;

    uint16_centi_duration( Seconds<uint16_t>(25) );

    auto copy_uint16_centi_duration = uint16_centi_duration;

    std::vector<uint8_t> before_msg(2, 1);
    std::vector<uint8_t> after_msg(2, 2);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = uint16_centi_duration.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = uint16_centi_duration.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = uint16_centi_duration.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_uint16_centi_duration, uint16_centi_duration);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, time_epochs_constructors) {

    auto const now = std::chrono::system_clock::now();

    std::stringstream ss;

    // only the epoch is shifted, the time point is the same.
    // now is now in any epoch
    ss << "\n\nconstructor: time points with shift";

    auto const cft = mzn::CmdFieldTime<int>(now);
    auto const cft_shifted = mzn::CmdFieldTime<int, 86400>(now);

    ss << std::endl << cft << "  /  " << cft_shifted;

    // -----------
    ss << "\n\nwith mutator: time points with shift";

    auto cft2 = mzn::CmdFieldTime<int>{};
    auto cft2_shifted = mzn::CmdFieldTime<int, 86400>{};

    cft2(now);
    cft2_shifted(now);

    ss << std::endl << cft2 << "  /  " << cft2_shifted;
    ss << "\n\n";
    ss << cft2.data() << " / " << cft2_shifted.data();
    ss << "\n\n";

    // now is now, all show the same time point:
    EXPECT_EQ( cft(), cft_shifted() );
    EXPECT_EQ( cft2(), cft2_shifted() );
    EXPECT_EQ( cft(), cft2() );

    // equality operators are implemented
    EXPECT_EQ(cft, cft_shifted);
    EXPECT_EQ(cft2, cft2_shifted);
    EXPECT_EQ(cft, cft2);

    // same epoch, same internal value:
    EXPECT_EQ( cft.data(), cft2.data() );
    EXPECT_EQ( cft_shifted.data() , cft2_shifted.data() );

    // different epoch, different internal value:
    EXPECT_NE( cft.data() , cft_shifted.data() );
    EXPECT_NE( cft2.data() , cft2_shifted.data() );

    std::cout << ss.str();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, duration_constructors) {

    std::stringstream ss;

    auto const s5 = std::chrono::seconds(5);

    mzn::CmdFieldDuration<int> cfs5_(s5);

    mzn::CmdFieldDuration<int> cfs5_2;
    cfs5_2(s5);

    mzn::CmdFieldDuration<int> cfs5_3(cfs5_2);

    EXPECT_EQ( cfs5_(), cfs5_2() );
    EXPECT_EQ( cfs5_2(), cfs5_3() );

    // cfd has the equality/inequality operators overloaded
    EXPECT_EQ(cfs5_, cfs5_2);
    EXPECT_EQ(cfs5_2, cfs5_3);

    auto const m1 = std::chrono::minutes(5);
    mzn::CmdFieldDuration<int, std::ratio<60> > cfm1_(m1);

    // 1 minutes > 5 seconds, comparison using different Periods
    EXPECT_GT(cfm1_, cfs5_);

    auto const cfs60_ = mzn::CmdFieldDuration<int>(m1);
    ss << "\ninternal value: " << cfs60_.data();
    ss << "\ndirect stream: " << cfs60_;

    // 60 seconds == 1 minute, comparison using different Periods
    EXPECT_EQ(cfm1_(), cfs60_() );
    EXPECT_EQ(cfm1_, cfs60_);
    std::cout << ss.str();
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_from_float_duration) {

    std::chrono::duration<float> float_seconds(25.222);
    using CentiSeconds = std::chrono::duration<int, std::centi>;
    // need to floor since the number needs to be truncated
    uint16_centi_duration( date::floor<CentiSeconds>(float_seconds) );

    auto copy_uint16_centi_duration = uint16_centi_duration;

    std::vector<uint8_t> before_msg(2, 1);
    std::vector<uint8_t> after_msg(2, 2);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = uint16_centi_duration.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = uint16_centi_duration.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = uint16_centi_duration.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_uint16_centi_duration, uint16_centi_duration);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_floats) {

    f(1.234); // 4
    auto copy_f = f;

    std::vector<uint8_t> before_msg(4, 1);
    std::vector<uint8_t> after_msg(4, 2);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = f.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = f.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = f.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_f, f);

    // make sure it is the right number
    // this is 1.234 in big endian order
    std::vector<uint8_t> msg {0x3F, 0x9D, 0xF3, 0xB6};
    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(msg[i], after_msg[i]);
    }
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_tfloats) {

    tf(1.234); // 4
    auto copy_tf = tf;

    std::vector<uint8_t> before_msg(4, 1);
    std::vector<uint8_t> after_msg(4, 2);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = tf.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = tf.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = tf.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_tf, tf);

    // make sure it is the right number
    // this is 1.234 in big endian order
    std::vector<uint8_t> msg {0x3F, 0x9D, 0xF3, 0xB6};
    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(msg[i], after_msg[i]);
    }
}

// -------------------------------------------------------------------------- //
template<typename T>
void print_vector(T v) {
    std::cout << "[";
    for (auto const & b : v) {
        std::cout << static_cast<int>(b) << ", ";
    }
    std::cout << "]";
}

// fails only if SetUp() of CmdFieldBitmaps fails
// auto generated basic test for all bitmaps, checks for operator() consistency
// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdFieldBitmaps, cmd_field_bitmap_types_setup) {}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_bitmap) {

    raw_ui16bitset(1160); // x0488
    auto copy_raw_ui16bitset = raw_ui16bitset;
    //mzn::CmdField<std::bitset<16>, 2> raw_ui16bitset;
    //mzn::CmdFieldBitmap<1> ui8bitmap;
    //mzn::CmdFieldBitmap<2> ui16bitmap;
    //mzn::CmdFieldBitmap<4> ui32bitmap;

    // some initial values different than 0
    std::vector<uint8_t> before_msg(2, 5);
    std::vector<uint8_t> after_msg(2, 15);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = raw_ui16bitset.data_to_msg(before_msg, mf_pos);
    // make sure the data is being set in the msg_to_data function
    // and that it is actually changing
    raw_ui16bitset(32);

    mf_pos = 0; mf_pos = raw_ui16bitset.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = raw_ui16bitset.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_raw_ui16bitset, raw_ui16bitset);

    // make sure messages are consistent with the original value
    std::vector<uint8_t> msg {0x04, 0x88};
    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(msg[i], after_msg[i]);
    }
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, full_example_bitmap) {

    // example of a bitmap, this is from BmFgFix which is the first bitmap
    // tested on the commands tests from C1Ping5

    bm_fg_fix(31); // x001F
    auto copy_bm_fg_fix = bm_fg_fix;
    //mzn::CmdField<std::bitset<16>, 2> bm_fg_fix;
    //mzn::CmdFieldBitmap<1> ui8bitmap;
    //mzn::CmdFieldBitmap<2> ui16bitmap;
    //mzn::CmdFieldBitmap<4> ui32bitmap;

    // some initial values different than 0
    std::vector<uint8_t> before_msg(2, 5);
    std::vector<uint8_t> after_msg(2, 15);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = bm_fg_fix.data_to_msg(before_msg, mf_pos);
    // make sure the data is being set in the msg_to_data function
    // and that it is actually changing
    bm_fg_fix(32);

    mf_pos = 0; mf_pos = bm_fg_fix.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = bm_fg_fix.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_bm_fg_fix, bm_fg_fix);

    // make sure messages are consistent with the original value
    std::vector<uint8_t> msg {0x00, 0x1F};
    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(msg[i], after_msg[i]);
    }

    // same as in the command test, many bitmaps tested in commands
    // this is here to make sure this test would fail before the commands test
    // in case of a CmdField type of problem
    EXPECT_EQ( true, bm_fg_fix.ethernet_installed() );
    EXPECT_EQ( true, bm_fg_fix.can_statreq_dynamic_ip() );
    EXPECT_EQ( true, bm_fg_fix.can_statreq_auxiliary_board() );
    EXPECT_EQ( true, bm_fg_fix.can_expanded_c1_web_cmds() );
    EXPECT_EQ( true, bm_fg_fix.can_statreq_serial_sensor() );
    EXPECT_EQ( false, bm_fg_fix.can_report_255maplus_supply_current() );
    EXPECT_EQ( false, bm_fg_fix.at_least_one_environmental_proc_configured() );
    EXPECT_EQ( false, bm_fg_fix.is_Q330S() );
}

// -------------------------------------------------------------------------- //
template<typename From, typename To, std::size_t N>
void array_cast_and_copy(From from,
                         mzn::CmdFieldArray<To, N> & to) {

     std::array<To, N> to_data_temp;
    for (int i = 0; i < N; i++) {
        to_data_temp[i] = static_cast<To>(from[i]);
    }

    to(to_data_temp);
};

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_cf_array_plain_int) {

    // in CmdField<T,N>, N = number of bytes
    // in Arrays<T,N>, N = number of elements
    // this is the same when sizeof(T) == 1

    constexpr auto N = 2;
    std::array<int8_t, N> test_data{3, 5};
    array_cast_and_copy(test_data, cf_array_plain_int);

    auto copy_cf_array_plain_int   = cf_array_plain_int;

    std::vector<uint8_t> before_msg(N, 5);
    std::vector<uint8_t> after_msg(N, -77);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = cf_array_plain_int.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = cf_array_plain_int.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = cf_array_plain_int.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_cf_array_plain_int, cf_array_plain_int);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_cf_array_plain_char) {

    // in CmdField<T,N>, N = number of bytes
    // in Arrays<T,N>, N = number of elements
    // this is the same when sizeof(T) == 1

    constexpr auto N = 2;
    std::array<uint8_t, N> test_data{3, 155};
    array_cast_and_copy(test_data, cf_array_plain_char);

    auto copy_cf_array_plain_char   = cf_array_plain_char;

    std::vector<uint8_t> before_msg(N, 5);
    std::vector<uint8_t> after_msg(N, 177);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = cf_array_plain_char.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = cf_array_plain_char.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = cf_array_plain_char.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_cf_array_plain_char, cf_array_plain_char);
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_array_char) {

    // in CmdField<T,N>, N = number of bytes
    // in Arrays<T,N>, N = number of elements
    // this is the same when sizeof(T) == 1

    constexpr auto N = 2;
    std::array<uint8_t, N> test_data{3, 155};
    array_cast_and_copy(test_data, array_char);

    auto copy_array_char   = array_char;

    std::vector<uint8_t> before_msg(N, 5);
    std::vector<uint8_t> after_msg(N, 177);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = array_char.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = array_char.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = array_char.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_array_char, array_char);
}


// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_arrays) {

    // in CmdField<T,N>, N = number of bytes
    // in Arrays<T,N>, N = number of elements
    // this is the same when sizeof(T) == 1

    /*
    constexpr auto N = 2;
    std::array<uint8_t, 2> test_data{3, 155};

    array_cast(test_data, cf_array_plain_int.data() );
    array_cast(test_data, cf_array_plain_char.data() );
    array_cast(test_data, cf_array_plain_ubyte.data() );
    array_cast(test_data, array_byte.data() );
    array_cast(test_data, array_char.data() );
    array_cast(test_data, array_ubyte.data() );

    f(1.234); // 4
    auto copy_f = f;

    auto copy_cf_array_plain_int   = cf_array_plain_int;
    auto copy_cf_array_plain_char  = cf_array_plain_char;
    auto copy_cf_array_plain_ubyte = cf_array_plain_ubyte;
    auto copy_array_byte   = array_byte;
    auto copy_array_char  = array_char;
    auto copy_array_ubyte = array_ubyte;

    std::vector<uint8_t> before_msg(2, 5);
    std::vector<uint8_t> after_msg(2, 177);

    std::size_t mf_pos;

    mf_pos = 0; mf_pos = f.data_to_msg(before_msg, mf_pos);
    mf_pos = 0; mf_pos = f.msg_to_data(before_msg, mf_pos);
    mf_pos = 0; mf_pos = f.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_f, f);

    // make sure it is the right number
    // this is 1.234 in big endian order
    std::vector<uint8_t> msg {0x3F, 0x9D, 0xF3, 0xB6};
    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(msg[i], after_msg[i]);
    }
    */
}

// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, msg_and_data_pod) {

    // check values go back to same
    // this_cf_size / accumulation
    uc(255); // 1 / 1
    c(-128); // 1 / 2
    ui16(65'535); // 2 / 4
    i16(-32'768); // 2 / 6
    ui32(4'294); // 4 / 10
    i32(-2'147); // 4 / 14
    i64(234234); // 8 / 22
    ui64(1); // 8 / 30
    bl(false); // 1 / 31
    // total: 31

    auto copy_uc = uc;
    auto copy_c = c;
    auto copy_ui16 = ui16;
    auto copy_i16 = i16;
    auto copy_ui32 = ui32;
    auto copy_i32 = i32;
    auto copy_i64 = i64;
    auto copy_ui64 = ui64;
    auto copy_bl = bl;

    // start with different values
    std::vector<uint8_t> before_msg(31, 1);
    std::vector<uint8_t> after_msg(31, 2);

    auto mf_pos = 0;

    mf_pos = uc.data_to_msg(before_msg, mf_pos);
    mf_pos = c.data_to_msg(before_msg, mf_pos);
    mf_pos = ui16.data_to_msg(before_msg, mf_pos);
    mf_pos = i16.data_to_msg(before_msg, mf_pos);
    mf_pos = ui32.data_to_msg(before_msg, mf_pos);
    mf_pos = i32.data_to_msg(before_msg, mf_pos);
    mf_pos = i64.data_to_msg(before_msg, mf_pos);
    mf_pos = ui64.data_to_msg(before_msg, mf_pos);
    mf_pos = bl.data_to_msg(before_msg, mf_pos);

    mf_pos = 0;

    mf_pos = uc.msg_to_data(before_msg, mf_pos);
    mf_pos = c.msg_to_data(before_msg, mf_pos);
    mf_pos = ui16.msg_to_data(before_msg, mf_pos);
    mf_pos = i16.msg_to_data(before_msg, mf_pos);
    mf_pos = ui32.msg_to_data(before_msg, mf_pos);
    mf_pos = i32.msg_to_data(before_msg, mf_pos);
    mf_pos = i64.msg_to_data(before_msg, mf_pos);
    mf_pos = ui64.msg_to_data(before_msg, mf_pos);
    mf_pos = bl.msg_to_data(before_msg, mf_pos);

    mf_pos = 0;

    mf_pos = uc.data_to_msg(after_msg, mf_pos);
    mf_pos = c.data_to_msg(after_msg, mf_pos);
    mf_pos = ui16.data_to_msg(after_msg, mf_pos);
    mf_pos = i16.data_to_msg(after_msg, mf_pos);
    mf_pos = ui32.data_to_msg(after_msg, mf_pos);
    mf_pos = i32.data_to_msg(after_msg, mf_pos);
    mf_pos = i64.data_to_msg(after_msg, mf_pos);
    mf_pos = ui64.data_to_msg(after_msg, mf_pos);
    mf_pos = bl.data_to_msg(after_msg, mf_pos);

    for (int i = 0; i < after_msg.size(); i++) {
        EXPECT_EQ(before_msg[i], after_msg[i]);
    }

    EXPECT_EQ(copy_uc, uc);
    EXPECT_EQ(copy_c, c);
    EXPECT_EQ(copy_ui16, ui16);
    EXPECT_EQ(copy_i16, i16);
    EXPECT_EQ(copy_ui32, ui32);
    EXPECT_EQ(copy_i32, i32);
    EXPECT_EQ(copy_i64, i64);
    EXPECT_EQ(copy_ui64, ui64);
    EXPECT_EQ(copy_bl, bl);
}



// -------------------------------------------------------------------------- //
TEST_F(FixtureCmdField, baler_status) {

/*
 "bm_balerstatus":{
    "bm_fields":[

      {"bmf_name": "timeout_counts", "bmf_val_begin": 0, "bmf_val_mask": "0xFFF"},
      {"bmf_name": "baler_is_currently_forced_off_for_the_minimum_off_time", "bmf_bit": 12},
      {"bmf_name": "set_to_one", "bmf_bit": 13},
      {"bmf_name": "baler_power_status", "bmf_code_full_range": false,
        "bmf_code_begin": 14, "bmf_code_mask": "0x3",
        "bmf_codes": [
              {"bmf_code_name": "baler_off",  "bmf_code": 0},
              {"bmf_code_name": "baler_power_turned_on_by_a_command", "bmf_code": 1},
              {"bmf_code_name": "baler_power_turned_on_automatically", "bmf_code": 2},
              {"bmf_code_name": "baler_power_is_continuous", "bmf_code": 3}
        ]
      }
    ]

,   "mf_size": 2, "bm_originator": "q"
},
*/

}

// -------------------------------------------------------------------------- //
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
