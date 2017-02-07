// Manzano Software //
#ifndef _MZN_STREAM_PLOTTER_H_
#define _MZN_STREAM_PLOTTER_H_

#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <future>
#include <numeric>
#include <limits>
//! console functions
#include <cstdlib>
#include <sys/ioctl.h>
//! round
#include <cmath>

#include "mzn_except.h"

namespace mzn {


//! Plots points in time on a stream
/*!
    @tparam T Point type.
    @tparam N number coordinates on a point
    @tparam Tc type to cast, when the value is more limited than the range of T
    @tparam ppl = points_per_line
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename T, int N = 3, typename Tc = T, int ppl = 2>
class StreamPlotter {

    static_assert(ppl % 2 == 0, "StreamPlotter ppl should be even");
    static_assert(ppl > 0, "StreamPlotter ppl should be positivie");
    static_assert(N > 0, "StreamPlotter N should be positivie");

    using Point = std::array<T, N>;
    std::vector<Point> data_;
    int plot_pos_ = 0;

public:

    // ---------------------------------------------------------------------- //
    T min_limit;
    T max_limit;
    // ---------------------------------------------------------------------- //
    void show_summary() const;
    void add(Point const & point);
    void add(std::vector<Point> const & points);
    void reset_plot() {plot_pos_ = 0;}
    void plot_line(int const i);
    void plot_lines();
    void plot_all();

    // ---------------------------------------------------------------------- //
    explicit
    StreamPlotter() : data_{} {
        auto constexpr v_max = std::numeric_limits<Tc>::max();
        auto constexpr v_min = std::numeric_limits<Tc>::min();
        max_limit = static_cast<T>(v_max);
        min_limit = static_cast<T>(v_min);
    };

    ~StreamPlotter();

private:

    //! winsize is an struct from C
    // ---------------------------------------------------------------------- //
    winsize terminal_window_size_;

    T abs_max(std::vector<Point> const & vec) const;

    std::ostream & os = std::cout;
};


// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
void StreamPlotter<T, N, Tc, ppl>::add(Point const & point) {
    data_.push_back(point);
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::add(std::vector<Point> const & points) {
    data_.insert( data_.end(), points.begin(), points.end() );
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::plot_line(int const i) {

    // get terminal window size
    ioctl(0, TIOCGWINSZ, &terminal_window_size_);
    auto const axis_stream_width = terminal_window_size_.ws_col / N;
    // +1 to move from 1..99 to 1..255, +1 to include space for sign
    // +1 to include space for a space in between (+3 total)
    auto constexpr v_digits = std::numeric_limits<Tc>::digits10 + 3;
    // 2 chars for [], other 2 for scroll bars and wiggle (-4 total)
    auto const plot_width = axis_stream_width - v_digits - 12;
    // round down to closest even number (clear last bit)
    int const plot_end = plot_width & ~1;
    int const plot_middle = plot_end / 2;

    T const v_plot_range = std::abs(max_limit - min_limit);
    float const pos_scalar = plot_end / static_cast<float>(v_plot_range);

    os <<"\n";

    for (int axis = 0; axis < N; axis++) {

        // divide sum in two to get a rough trend, used in choosing plot c
        // ----------------------------------------------------------------- //
        // ppl is even, this is half of the sum index
        auto const hsi = i + ppl/2;
        // sum first and second halfs
        int j;
        T sum_fh = 0; for (j = i  ; j < hsi    ; j++) sum_fh += data_[j][axis];
        T sum_sh = 0; for (j = hsi; j < i + ppl; j++) sum_sh += data_[j][axis];

        // calculate average coordinate value
        // ----------------------------------------------------------------- //
        // limit to range of Tc if Tc != T
        // stream as T, specially in the case of Tc int8_t
        T v = static_cast<Tc>( (sum_fh + sum_sh) / static_cast<float>(ppl) );
        os << "i." << i << "ds." << data_.size() << "x." << axis
           << "hsi." << hsi;

        // stream average
        os  << std::setfill(' ') << std::setw(v_digits) << v;

        // is there space for the plot?
        if (plot_width < 4) continue;

        // plot the line
        // ----------------------------------------------------------------- //
        // choose char for plot
        char c = '|';
        if (sum_fh > sum_sh) c = '\\';
        if (sum_fh < sum_sh) c = '/';

        // only applies with user provided min_ max_ limits, allow plot loss
        // v real value already streamed, this is only for the plot
        // ----------------------------------------------------------------- //
        if (v < min_limit) {v = min_limit; c = '<';}
        if (v > max_limit) {v = max_limit; c = '>';}

        // from real values (v) to char position (pos) in the plot
        // ----------------------------------------------------------------- //
        int const pos = std::round( pos_scalar * (v - min_limit) );

        if (pos < 0  or pos > plot_end) {

            std::cout << "\npos: " << pos
                      << " v: " << v
                      << " sfh: " << sum_fh
                      << " ssh: " << sum_sh
                      << " ps: " << pos_scalar;

            throw std::logic_error("plot pos");
        }

        // ready to stream
        // ----------------------------------------------------------------- //
        int l_spaces;
        int m_spaces;
        int r_spaces;

        char middle_char = ':';

        os << "[";

        // relation to middle, plot_end is always even
        if (pos == plot_middle) {

            // [ l_spaces |  r_spaces ]
            os << std::string(pos, ' ') << c << std::string(pos, ' ');

        } else if (pos < plot_middle) {

            // [ l_spaces | m_spaces : r_spaces ]
            l_spaces = pos;
            m_spaces = plot_middle - pos - 1;
            r_spaces = plot_middle;

            os << std::string(l_spaces, ' ') << c
               << std::string(m_spaces, ' ') << middle_char
               << std::string(r_spaces, ' ');

        } else {

            // [ l_spaces : m_spaces | r_spaces ]
            l_spaces = plot_middle;
            m_spaces = pos - plot_middle - 1;
            r_spaces = plot_end - pos;

            os << std::string(l_spaces, ' ') << middle_char
               << std::string(m_spaces, ' ') << c
               << std::string(r_spaces, ' ');
        }

        os << "]";
    }
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::plot_lines() {

    for (; plot_pos_ + ppl <= data_.size(); plot_pos_ += ppl) {
        plot_line(plot_pos_);
    }
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::plot_all() {

    if ( data_.empty() ) return;

    // save current settings just in case used in the middle of incoming data
    auto const temp_pp = plot_pos_;
    auto const temp_min_limit = min_limit;
    auto const temp_max_limit = max_limit;

    auto reset_settings = [&,this]() { plot_pos_ = temp_pp;
                                       min_limit = temp_min_limit;
                                       max_limit = temp_max_limit; };

    // plot from the beginning of the data
    plot_pos_ = 0;

    // modify min / max
    auto const result = abs_max(data_) - 1;
    min_limit = -result;
    max_limit = result;

    std::cout << std::endl << "Plot and data range: min " << min_limit
              << " max: " << max_limit;

    try {plot_lines();}
    catch (std::exception const & e) {reset_settings(); throw e;}
    reset_settings();
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::show_summary() const {

}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
StreamPlotter<T, N, Tc, ppl>::~StreamPlotter() {

    /*
    // catch everything
    try {

        auto constexpr wait_duration = std::chrono::milliseconds(500);

        auto future_wait_result = future_.wait_for(wait_duration);

        if (future_wait_result == std::future_status::timeout) {
            future_.wait();
        } else if (future_wait_result == std::future_status::ready) {
            std::cout << std::endl << "end stream_plotter, bye!\n";
        }

    } catch (std::exception const & e) {
        std::cerr << std::endl << "Caught @StreamPlotter destructor";
        std::cerr << std::endl << e.what();
        // destructors shall not throw
    }
    */
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
T
StreamPlotter<T, N, Tc, ppl>::abs_max(std::vector<Point> const & vec) const {

    if ( vec.empty() ) throw std::logic_error{"minmax empty vector"};

    T am = std::abs(vec[0][0]);

    for (auto const & point : vec) {
        for (auto const & coordinate : point) {
            if (std::abs(coordinate) > am) am = std::abs(coordinate);
        }
    }

    return am;
}

} // end namespace

#endif
