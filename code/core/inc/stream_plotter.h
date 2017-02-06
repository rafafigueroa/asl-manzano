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

    std::pair<T, T> minmax(std::vector<Point> const & vec) const;

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
        // ppl is even, this is half of the sum index
        auto const hsi = i + ppl/2;
        // sum first and second halfs
        int j;
        T sum_fh = 0; for (j = i  ; j < hsi    ; j++) sum_fh += data_[j][axis];
        T sum_sh = 0; for (j = hsi; j < i + ppl; j++) sum_sh += data_[j][axis];

        // calculate average coordinate value
        // ----------------------------------------------------------------- //
        // allow for truncation, limit to range of Tc if Tc != T
        // stream as T, specially in the case of Tc int8_t
        T v = static_cast<Tc>( (sum_fh + sum_sh) / ppl);

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
            m_spaces = pos - plot_middle - 1; r_spaces = plot_end - pos;

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

    auto round_down_to_multiple = [](auto const & n, auto const & multiple) {

        auto const r = n % multiple;
        if (r == 0) return n;
        return n - r;
    };

    // make sure we have points multiple of ppl to plot
    auto points_to_plot = data_.size() - plot_pos_;
    points_to_plot = round_down_to_multiple(points_to_plot, ppl);

    if (points_to_plot == 0) return;

    for (int i = plot_pos_; i < data_.size(); i += ppl) plot_line(i);

    plot_pos_ += points_to_plot;
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
    auto const result = minmax(data_);
    min_limit = result.first;
    max_limit = result.second;

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
std::pair<T, T>
StreamPlotter<T, N, Tc, ppl>::minmax(std::vector<Point> const & vec) const {

    if ( vec.empty() ) throw std::logic_error{"minmax empty vector"};

    // std::min_element might need one more min/max than necessary
    // since it would provide the Point from data_, but the min/max inside
    // the point, taking one more go.
    // So far this is only used for N == 1, for which there is an specialization
    // if this is used in a more general way, this should be rewritten

    // there is also the minmax_element but that one does not seem to fit our
    // caser where min is the min of the next one, max is the max of the next
    // one, so different comparisons are used (Point a > Point b would make
    // no sense).

    // another way to improve performance of this operation is having the axis
    // separated std::array<std::vector<T>, N> but that would make the rest
    // of the code less readable.

    // modify min / max
    // compare points functions
    auto comp_min = [](Point const & a, Point const & b) {
        auto const min_a = *std::min_element(a.begin(), a.end());
        auto const min_b = *std::min_element(b.begin(), b.end());
        return  min_a < min_b;
    };

    auto comp_max = [](Point const & a, Point const & b) {
        auto const max_a = *std::max_element(a.begin(), a.end());
        auto const max_b = *std::max_element(b.begin(), b.end());
        return  max_a < max_b;
    };

    auto const min_p = *std::min_element(data_.begin(), data_.end(), comp_min);
    auto const max_p = *std::max_element(data_.begin(), data_.end(), comp_max);

    return std::pair<T, T>( *std::min_element(min_p.begin(), min_p.end()),
                            *std::max_element(max_p.begin(), max_p.end()) );

    // TODO specialize for N == 1
    /*

    if (comp(*first, *result.first)) {
        result.first = first;
    } else {
        result.second = first;
    }
    while (++first != last) {
        ForwardIt i = first;
        if (++first == last) {
            if (comp(*i, *result.first)) result.first = i;
            else if (!(comp(*i, *result.second))) result.second = i;
            break;
        } else {
            if (comp(*first, *i)) {
                if (comp(*first, *result.first)) result.first = first;
                if (!(comp(*i, *result.second))) result.second = i;
            } else {
                if (comp(*i, *result.first)) result.first = i;
                if (!(comp(*first, *result.second))) result.second = first;
            }
        }
    }
    */
}



} // end namespace

#endif
