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
    @tparam pps = points_per_second, could be generalized to period
    @tparam Tc type to cast, when the value is more limited than the range of T
    @author rfigueroa@usgs.gov
 */
// -------------------------------------------------------------------------- //
template <typename T, int N = 3, int pps = 2, typename Tc = T>
class StreamPlotter {

    static_assert(N > 0, "StreamPlotter N should be positive");

    using Point = std::array<T, N>;
    std::vector<Point> data_;
    int ppl_ = 2;
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
    void set_ppl(int const ppl) {
        if (ppl % 2 != 0) throw std::logic_error{"@StreamPlotter odd ppl"};
        if (ppl <= 0) throw std::logic_error{"@StreamPlotter ppl not positive"};
        ppl_ = ppl;
    }

    // ---------------------------------------------------------------------- //
    explicit
    StreamPlotter(int const ppl) : data_{} {
        auto constexpr v_max = std::numeric_limits<Tc>::max();
        auto constexpr v_min = std::numeric_limits<Tc>::min();
        max_limit = static_cast<T>(v_max);
        min_limit = static_cast<T>(v_min);
        set_ppl(ppl);
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
template <typename T, int N, int pps, typename Tc>
void StreamPlotter<T, N, pps, Tc>::add(Point const & point) {
    data_.push_back(point);
}

// -------------------------------------------------------------------------- //
template <typename T, int N, int pps, typename Tc>
inline
void StreamPlotter<T, N, pps, Tc>::add(std::vector<Point> const & points) {
    data_.insert( data_.end(), points.begin(), points.end() );
}

// -------------------------------------------------------------------------- //
template <typename T, int N, int pps, typename Tc>
inline
void StreamPlotter<T, N, pps, Tc>::plot_line(int const i) {

    // get terminal window size
    ioctl(0, TIOCGWINSZ, &terminal_window_size_);
    auto const axis_stream_width = terminal_window_size_.ws_col / N;
    // +1 to move from 1..99 to 1..255, +1 to include space for sign
    // +1 to include space for a space in between (+3 total)
    auto constexpr v_digits = std::numeric_limits<Tc>::digits10 + 3;
    // -2 chars for [], -2 for scroll bars and wiggle
    // -4 for comments (seconds on  qview) (-8 total)
    auto const plot_width = axis_stream_width - v_digits - 8;
    // round down to closest even number (clear last bit)
    int const plot_end = plot_width & ~1;
    int const plot_middle = plot_end / 2;

    T const v_plot_range = std::abs(max_limit - min_limit);
    float const pos_scalar = plot_end / static_cast<float>(v_plot_range);

    os <<"\n";

    for (int axis = 0; axis < N; axis++) {

        // divide sum in two to get a rough trend, used in choosing plot c
        // ----------------------------------------------------------------- //
        // ppl_ is even, this is half of the sum index
        auto const hsi = i + ppl_/2;
        // sum first and second halfs
        int j;
        T sum_f = 0; for (j = i  ; j < hsi     ; j++) sum_f += data_[j][axis];
        T sum_s = 0; for (j = hsi; j < i + ppl_; j++) sum_s += data_[j][axis];

        // calculate average coordinate value
        // ----------------------------------------------------------------- //
        // limit to range of Tc if Tc != T
        // stream as T, specially in the case of Tc int8_t
        T v = static_cast<Tc>( (sum_f + sum_s) / static_cast<float>(ppl_) );

        // stream average
        os  << std::setfill(' ') << std::setw(v_digits) << v;

        // is there space for the plot?
        if (plot_width < 4) continue;

        // plot the line
        // ----------------------------------------------------------------- //
        // choose char for plot
        char c = '|';
        if (sum_f > sum_s) c = '\\';
        if (sum_f < sum_s) c = '/';

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
                      << " sf: " << sum_f
                      << " ss: " << sum_s
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
template <typename T, int N, int pps, typename Tc>
inline
void StreamPlotter<T, N, pps, Tc>::plot_lines() {

    // put a marker _# roughly every second, but plot_pos_ values
    // might not be a multiple of one second, so it gets approximated
    // to multiples of ppl_
    auto const lines_per_second = static_cast<int>(pps / ppl_);
    auto const pos_marker = ppl_ * lines_per_second;

    for ( ; plot_pos_ + ppl_ <= data_.size(); plot_pos_ += ppl_) {
        plot_line(plot_pos_);
        // put marker roughly every second
        if (plot_pos_ % pos_marker == 0) {
            auto const pos_seconds = plot_pos_ / pps;
            std::cout << "_" << pos_seconds;
        }
    }
}

// -------------------------------------------------------------------------- //
template <typename T, int N, int pps, typename Tc>
inline
void StreamPlotter<T, N, pps, Tc>::plot_all() {

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
template <typename T, int N, int pps, typename Tc>
inline
void StreamPlotter<T, N, pps, Tc>::show_summary() const {

}

// -------------------------------------------------------------------------- //
template <typename T, int N, int pps, typename Tc>
StreamPlotter<T, N, pps, Tc>::~StreamPlotter() {

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
template <typename T, int N, int pps, typename Tc>
inline
T
StreamPlotter<T, N, pps, Tc>::abs_max(std::vector<Point> const & vec) const {

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
