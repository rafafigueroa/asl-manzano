#ifndef _MZN_STRING_UTILITIES_H_
#define _MZN_STRING_UTILITIES_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include "mzn_except.h"
#include <chrono>
#include <cstdio>
#include <future>

namespace mzn {

namespace Utility {

// -------------------------------------------------------------------------- //
inline
std::vector<std::string> get_tokens(std::string const & line,
                                    char const delimiter = ',') {

    std::vector<std::string> tokens;
    std::stringstream ss;
    ss.str(line);
    std::string token;

    while ( std::getline(ss, token, delimiter) ) tokens.push_back(token);

    return tokens;
}

// -------------------------------------------------------------------------- //
inline
void expect_string(std::string const & expected,
                   std::string const & token) {

    if (token != expected) {
        std::stringstream ss;
        ss << "expected:" << expected << " got:" << token;
        throw std::logic_error{ss.str()};
    }
}

// used to underline the problematic token in the error message
// -------------------------------------------------------------------------- //
inline
std::string underline_error(std::string const & token, int e_index) {

    std::stringstream token_underlined;

    if (token.size() > e_index) {
        token_underlined << "\n    " << token
                         << "\n    " << std::string(e_index, '~')
                         << "^"
                         << std::string(token.size() - e_index - 1, '~');
    }

    return token_underlined.str();
}

// -------------------------------------------------------------------------- //
inline
int match_positive_number(std::string const & token,
                          std::size_t & token_index) {

    std::string token_number;

    for (int i = token_index; i < token.size(); i++) {
        if (std::isdigit(token[i])) {
            token_number.push_back(token[i]);
        } else {
            break;
        }
    }

    if (token_number.size() == 0) {

        std::stringstream error_msg;

        error_msg << "expected positive number in token \'"
                  << token << "\' \n";

        if (token.size() > 1) {
            error_msg << "at:" << underline_error(token, token_index);
        }

        throw WarningException("Manzano",
                               "match_positive_number",
                               error_msg.str() );
    }

    token_index += token_number.size();

    return std::stoi(token_number);
}

// -------------------------------------------------------------------------- //
template <typename Period>
using DurationInt = std::chrono::duration<int, Period>;

// -------------------------------------------------------------------------- //
inline
std::chrono::seconds match_duration(std::string const & token) {

    std::size_t token_index = 0;

    auto match_duration_char = [&]() {

        if ( token_index >= token.size() ) {
            throw WarningException("Utility",
                                   "match_duration",
                                   "provide s, m or h with number");
        }

        char c;

        if (token.find("s", token_index) == token_index) c = 's'; else
        if (token.find("m", token_index) == token_index) c = 'm'; else
        if (token.find("h", token_index) == token_index) c = 'h';
        else throw WarningException("Utility",
                                    "match_duration",
                                    "s m h not found");

        token_index += 1;
        return c;
    };

    // return seconds only
    std::chrono::seconds d(0);

    // allow for composite 2m30s for example
    while (token_index < token.size() ) {

        auto const d_count = match_positive_number(token, token_index);
        auto const d_char = match_duration_char();

        switch (d_char) {
            case 's': {d += DurationInt<std::ratio<1>>(d_count);    break;}
            case 'm': {d += DurationInt<std::ratio<60>>(d_count);   break;}
            case 'h': {d += DurationInt<std::ratio<3600>>(d_count); break;}
        }
    }

    return d;
}

} // <- Utility
} // <- mzn

#endif // _MZN_STRING_UTILITIES_H_
