// Manzano Software //
#ifndef _MZN_STREAM_OUTPUT_H_
#define _MZN_STREAM_OUTPUT_H_

#include <vector>
#include <string>
#include <sstream>

#include "seismic_network.h"
#include "instruction_map.h"

namespace mzn {

// -------------------------------------------------------------------------- //
class StreamOutput {

    SeismicNetwork const & sn_;

public:

    explicit
    StreamOutput(SeismicNetwork const & sn) :
            sn_(sn) {}

    ~StreamOutput() = default;

    template <Kind kind>
    void show(TargetAddress const & ta) const {
        auto constexpr ui_id = UserInstruction::hash(Action::show, kind);
        std::cerr << "\n___SHOW general case___: " << ui_id;
        throw std::logic_error("no show function for this kind");
    }

    void show_prompt(TargetAddress const & ta) const;

    void show_tree(std::vector<std::string> const & items,
                   std::vector<bool> const & margins = {}) const;

    void show_num_tree(std::vector<std::string> const & items) const;

    void show_help() const;

private:

    std::ostream & os = std::cout;

    void show_margins(std::vector<bool> const & margins) const;

    void show_target() const;

    void show_target(Station const & st,
                     int const target_index,
                     std::vector<bool> const & margins = {}) const;

    void show_target(Digitizer const & q,
                     int const target_index,
                     std::vector<bool> const & margins = {}) const;

    void show_target(DataProcessor const & dp,
                     int const target_index,
                     std::vector<bool> const & margins = {}) const;

    void show_target(Sensor const & s,
                     int const target_index,
                     std::vector<bool> const & margins = {}) const;


};

} // end namespace

#endif
