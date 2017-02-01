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

namespace mzn {

// -------------------------------------------------------------------------- //
template <typename T, int N = 3, typename Tc = T>
class StreamPlotter {

    using Point = std::array<T, N>;

    std::vector<Point> data_;
    int plot_pos_ = 0;

    unsigned int const points_per_line_;

public:

    void plot_line(int const & i);

    void plot_lines();

    void plot_all();

    explicit
    StreamPlotter(unsigned int const points_per_line = 2) :

            data_{},
            points_per_line_(points_per_line) {
    };

    ~StreamPlotter();

    void show_summary() const;

    void add(Point const & point);
    void add(std::vector<Point> const & points);

    void reset_plot() {plot_pos_ = 0;}

private:

    std::ostream & os = std::cout;

    void show_margins(std::vector<bool> const & margins) const;
};


// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::add(Point const & point) {
    data_.push_back(point);
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::add(std::vector<Point> const & points) {
    data_.insert( data_.end(), points.begin(), points.end() );
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::plot_line(int const & i) {

    os << std::setfill(' ') << "\n";

    for (int axis = 0; axis < N; axis++) {
        T sum = 0;
        for (int j = i; j < i + points_per_line_; j++) sum += data_[j][axis];
        // allow for truncation, potentially 2 casts if T != Tc
        T const coordinate_average = static_cast<Tc>(sum / points_per_line_);
        os  << coordinate_average << " : ";
    }
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::plot_lines() {

    auto const points_to_plot = data_.size() - plot_pos_;

    std::cout << std::flush << "<" << points_to_plot << ">" << std::flush;
    if (points_to_plot % points_per_line_ != 0) return;
    std::cout << std::flush << "[" << std::flush;

    for (int i = plot_pos_; i < data_.size(); i += points_per_line_) {
        std::cout << std::flush << "*" << std::flush;
        plot_line(i);
    }

    plot_pos_ += points_to_plot;
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::plot_all() {

    if ( data_.empty() ) return;

    auto const temp = plot_pos_;
    plot_pos_ = 0;
    plot_lines();
    plot_pos_ = temp;
}

// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::show_summary() const {

}

// show margins
// -------------------------------------------------------------------------- //
template <typename T, int N, typename Tc>
void StreamPlotter<T, N, Tc>::
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
template <typename T, int N, typename Tc>
StreamPlotter<T, N, Tc>::~StreamPlotter() {

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
