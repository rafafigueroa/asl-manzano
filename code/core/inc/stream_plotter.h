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

    using Point = std::array<T, N>;

    std::vector<Point> data_;
    int plot_pos_ = 0;

public:

    void plot_line(int const i);

    void plot_lines();

    void plot_all();

    explicit
    StreamPlotter() : data_{} {};

    ~StreamPlotter();

    void show_summary() const;

    void add(Point const & point);
    void add(std::vector<Point> const & points);

    void reset_plot() {plot_pos_ = 0;}

private:

    //! winsize is an struct from C
    winsize terminal_window_size_;

    std::ostream & os = std::cout;

    void show_margins(std::vector<bool> const & margins) const;
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
    auto const plot_width = axis_stream_width - v_digits - 4;
    // round down to closest even number (clear last bit)
    int const plot_end = plot_width & ~1;
    int const plot_middle = plot_end / 2;
    auto constexpr v_max = std::numeric_limits<Tc>::max();
    auto constexpr v_max_float = static_cast<float>(v_max);

    os <<"\n";
    T sum;
    // char for plot
    char c;

    for (int axis = 0; axis < N; axis++) {

        // sum values
        sum = 0; for (int j = i; j < i + ppl; j++) sum += data_[j][axis];

        // calculate average coordinate value
        // allow for truncation, limit to range of Tc if Tc != T
        // stream as T, specially in the case of Tc int8_t
        T const v = static_cast<Tc>(sum / ppl);

        // stream average
        os  << std::setfill(' ') << std::setw(v_digits) << v;

        // is there space for the plot?
        if (plot_width < 4) continue;

        // plot the line
        // ----------------------------------------------------------------- //

        // choose char for plot
        c = '|';
        if (data_[i][axis] < v) c = '\\';
        if (data_[i][axis] > v) c = '/';

        // position in the plot
        int const v_pos = std::round( plot_middle * (1.0 + v/v_max_float) );
        if (v_pos < 0 or v_pos > plot_end) throw std::logic_error("v_pos");

        os << "[";

        int l_spaces;
        int m_spaces;
        int r_spaces;

        // relation to middle (:), plot_end is always even
        if (v_pos == plot_middle) {

            // [ l_spaces |  r_spaces ]
            os << std::string(v_pos, ' ') << c << std::string(v_pos, ' ');

        } else if (v_pos < plot_middle) {

            // [ l_spaces | m_spaces : r_spaces ]
            l_spaces = v_pos;
            m_spaces = plot_middle - v_pos - 1;
            r_spaces = plot_middle;

            os << std::string(l_spaces, ' ') << c
               << std::string(m_spaces, ' ') << ":"
               << std::string(r_spaces, ' ');

        } else {

            // [ l_spaces : m_spaces | r_spaces ]
            l_spaces = plot_middle;
            m_spaces = v_pos - plot_middle - 1;
            r_spaces = plot_end - v_pos;

            os << std::string(l_spaces, ' ') << ":"
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

    auto const points_to_plot = data_.size() - plot_pos_;

//    std::cout << std::flush << "<" << points_to_plot << ">" << std::flush;
    if (points_to_plot % ppl != 0) return;

    for (int i = plot_pos_; i < data_.size(); i += ppl) {
        plot_line(i);
    }

    plot_pos_ += points_to_plot;
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::plot_all() {

    if ( data_.empty() ) return;

    auto const temp = plot_pos_;
    plot_pos_ = 0;
    plot_lines();
    plot_pos_ = temp;
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
inline
void StreamPlotter<T, N, Tc, ppl>::show_summary() const {

}

// show margins
// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc, int ppl>
void StreamPlotter<T, N, Tc, ppl>::
show_margins(std::vector<bool> const & margins) const {

    // used in show_target
    for (auto const & m : margins) {
        if (m) {
            os << "│   ";
        } else {
            os << "    ";
        }
    }
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

} // end namespace

#endif
