#include "stream_output.h"

namespace mzn {

// -------------------------------------------------------------------------- //
template<>
void StreamOutput::show<Kind::target>(TargetAddress const & ta) const {

    // this function is called only with full targets
    Target const t = ta.target();

    switch (t.scope) {

        case Scope::seismic_network:    show_target(); break;

        case Scope::station: {
            auto & st = sn_.st_const_ref(ta); show_target(st, t.index); break;
        }
        case Scope::digitizer: {
            auto & q  = sn_.q_const_ref(ta);  show_target(q, t.index);  break;
        }
        case Scope::sensor: {
            auto & s  = sn_.s_const_ref(ta);  show_target(s, t.index);  break;
        }
        case Scope::data_processor: {
            auto & dp = sn_.dp_const_ref(ta); show_target(dp, t.index); break;
        }
        default: break;
    }
}

// -------------------------------------------------------------------------- //
void StreamOutput::show_target() const {

    os << "\nsn : seismic_network";

    for (int i = 0; i < sn_.st.size(); i++) {

        os << "\n│\n";

        if (i + 1 == sn_.st.size() ) {
            os << "└── ";
            show_target(sn_.st[i], i, std::vector<bool> {false});
        } else {
            os << "├── ";
            show_target(sn_.st[i], i, std::vector<bool> {true});
        }
    }

}

// -------------------------------------------------------------------------- //
void StreamOutput::show_target(Station const & st,
                               int const target_index,
                               std::vector<bool> const & margins/*={}*/) const {

    if (margins.size() == 0) os << "\n";

    os << "st[" << target_index << "] : " << "station" << "(" << st << ")";

    auto margins_end = margins;
    auto margins_mid = margins;

    margins_end.push_back(false);
    margins_mid.push_back(true);

    for (int i = 0; i < st.q.size(); i++) {

        os << "\n";
        show_margins(margins);

        if (i + 1 == st.q.size() ) {
            os << "└── ";

            if (st.dp.size() == 0) {
                show_target(st.q[i], i, margins_end);
            } else {
                show_target(st.q[i], i, margins_mid);
            }

        } else {
            os << "├── ";
            show_target(st.q[i], i, margins_mid);
        }
    }

    for (int i = 0; i < st.dp.size(); i++) {

        os << "\n";
        show_margins(margins);

        if (i + 1 == st.dp.size() ) {
            os << "└── ";
            show_target(st.dp[i], i, margins_end);
        } else {
            os << "├── ";
            show_target(st.dp[i], i, margins_mid);
        }
    }
}

// -------------------------------------------------------------------------- //
void StreamOutput::show_target(Digitizer const & q,
                               int const target_index,
                               std::vector<bool> const & margins/*={}*/) const {

    if (margins.size() == 0) os << "\n";

    os << "q[" << target_index << "] : " << q;

    for (int i = 0; i < q.s.size(); i++) {

        os << "\n";
        show_margins(margins);

        if (i + 1 == q.s.size() ) {
            os << "└── ";
        } else {
            os << "├── ";
        }

        auto margins_end = margins;
        margins_end.push_back(false);

        show_target(q.s[i], i, margins_end);
    }
}

// -------------------------------------------------------------------------- //
void StreamOutput::show_target(Sensor const & s,
                               int const target_index,
                               std::vector<bool> const & margins/*={}*/) const {

    if (margins.size() == 0) os << "\n";

    os << "s[" << target_index << "] : " << s;
}

// -------------------------------------------------------------------------- //
void StreamOutput::show_target(DataProcessor const & dp,
                               int const target_index,
                               std::vector<bool> const & margins/*={}*/) const {

    if (margins.size() == 0) os << "\n";

    os  << "dp[" << target_index << "] : " << dp;
}

// -------------------------------------------------------------------------- //
template<>
void StreamOutput::show<Kind::config>(TargetAddress const & ta) const {

    auto scope = ta.scope();

    using S = Scope;
    os << "\nConfig:\n";
    switch (scope) {
        case S::seismic_network: sn_.stream_config(os);                  break;
        case S::digitizer:       sn_.q_const_ref(ta).stream_config(os);  break;
        case S::station:         sn_.st_const_ref(ta).stream_config(os); break;
        case S::data_processor:  sn_.dp_const_ref(ta).stream_config(os); break;
        case S::sensor:          sn_.s_const_ref(ta).stream_config(os);  break;

        default: throw std::logic_error{"@StreamOutput::show_config"};
    }
}

// -------------------------------------------------------------------------- //
template<>
void StreamOutput::show<Kind::status>(TargetAddress const & ta) const {

    auto scope = ta.scope();

    using S = Scope;
    os << "\nStatus:\n";

    switch (scope) {
        case S::seismic_network: sn_.stream_status(os);                  break;
        case S::digitizer:       sn_.q_const_ref(ta).stream_status(os);  break;
        case S::station:         sn_.st_const_ref(ta).stream_status(os); break;
        case S::data_processor:  sn_.dp_const_ref(ta).stream_status(os); break;
        case S::sensor:          sn_.s_const_ref(ta).stream_status(os);  break;

        default: throw std::logic_error{"@StreamOutput::show_status"};
    }

}

// show command
// -------------------------------------------------------------------------- //
template<>
void StreamOutput::show<Kind::command>(TargetAddress const & ta) const {

    auto const actions = InstructionMap::filter_actions(ta);

    for (auto const action : actions) {

        os << "\n\n " << action;

        auto const kinds = InstructionMap::filter_kinds(ta, action);
        auto const n = kinds.size();

        for(int i = 0; i < n; i++) {

            // -------- KINDS --------- //
            os << "\n ";
            if (i + 1 == n) os << "└── "; else os << "├── ";

            os << kinds[i];

            // ------- OPTIONS --------- //
            auto const options =
                InstructionMap::filter_options(action, kinds[i]);

            auto const m = options.size();

            if (m > 0) {

                // this command can be ran without setting an option
                if ( InstructionMap::has_empty_option(action, kinds[i]) ) {
                    os << " []";
                }

                for (int j = 0; j < m; j++) {
                    os << " [:" << options[j] << "]";
                }

            }
        }
    }
}

// show tree
// -------------------------------------------------------------------------- //
void StreamOutput::show_tree(std::vector<std::string> const & items,
                             std::vector<bool> const & margins/*={}*/) const {

    for(int i = 0; i < items.size(); i++) {
        os << "\n ";
        show_margins(margins);
        if ( i + 1 == items.size() ) os << "└── "; else os << "├── ";
        os << items[i];
    }
}

// -------------------------------------------------------------------------- //
void StreamOutput::show_num_tree(std::vector<std::string> const & items) const {

    for(int i = 0; i < items.size(); i++) {
        os << "\n ├──<" << i << "> " << items[i];
    }

    os << "\n ├──<" << items.size() << "> new..."
       << "\n └──<" << items.size() + 1 << "> cancel"
       << "\n\n enter number of desired live option: ";
}


/*

// ---------------- show config ----------------------- //
void StreamOutput::show_config(SeismicNetwork const & sn) {

    TargetAddress ta;

    for (int i = 0; i < sn_.st.size(); i++) {
        ta.sn_child = Target(Scope::station, i);
        show_config(sn_.st[i], ta);
    }
}

void StreamOutput::show_config(Station const & st,
                               TargetAddress ta) {

    os << "\n\n" << ta
       << "\n" << st.config;

    for (int i = 0; i < st.q.size(); i++) {
        ta.st_child = Target(Scope::digitizer, i);
        show_config(st.q[i], ta);
    }

    for (int i = 0; i < st.dp.size(); i++) {
        ta.st_child = Target(Scope::data_processor, i);
        show_config(st.dp[i], ta);
    }
}

void StreamOutput::show_config(Digitizer const & q,
                               TargetAddress ta) {

    os << "\n\n" << ta
       << "\n" << q.config;

    for (int i = 0; i < q.s.size(); i++) {
        ta.q_child = Target(Scope::sensor, i);
        show_config(q.s[i], ta);
    }
}

void StreamOutput::show_config(DataProcessor const & dp,
                           TargetAddress ta) {
    os << "\n\n" << ta
       << "\n" << dp.config;
}

void StreamOutput::show_config(Sensor const & s,
                               TargetAddress ta) {

    os << "\n\n" << ta
       << "\n" << s.config;
}

// ---------------- show status ----------------------- //
void StreamOutput::show_status() {

    TargetAddress ta;

    for (int i = 0; i < sn_.st.size(); i++) {
        ta.sn_child = Target(Scope::station, i);
        show_status(sn_.st[i], ta);
    }
}

void StreamOutput::show_status(Station const & st,
                           TargetAddress ta) {

    os << "\n\n" << ta
       << "\n" << st.status;

    for (int i = 0; i < st.q.size(); i++) {
        ta.st_child = Target(Scope::digitizer, i);
        show_status(st.q[i], ta);
    }

    for (int i = 0; i < st.dp.size(); i++) {
        ta.st_child = Target(Scope::data_processor, i);
        show_status(st.dp[i], ta);
    }
}

void StreamOutput::show_status(Digitizer const & q,
                           TargetAddress ta) {

    os << "\n\n" << ta
       << "\n" << q.status;

    for (int i = 0; i < q.s.size(); i++) {
        ta.q_child = Target(Scope::sensor, i);
        show_status(q.s[i], ta);
    }
}

void StreamOutput::show_status(DataProcessor const & dp,
                           TargetAddress ta) {
    os << "\n\n" << ta
       << "\n" << dp.status;
}

void StreamOutput::show_status(Sensor const & s,
                           TargetAddress ta) {

    os << "\n\n" << ta
       << "\n" << s.status;
}
*/

// show margins
// -------------------------------------------------------------------------- //
void StreamOutput::show_margins(std::vector<bool> const & margins) const {
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
void StreamOutput::show_prompt(TargetAddress const & ta) const {

    if (ta.sn_child.scope == Scope::station) {

        os << std::endl << "\n"
            << sn_.st[ta.sn_child.index] << ":" << ta << " » ";

    } else {
        os << std::endl << "\n" << ta << " » ";
    }
}

// -------------------------------------------------------------------------- //
void StreamOutput::show_help() const {
    std::cout << std::endl << "HELP\n";
}

} // <- mzn
