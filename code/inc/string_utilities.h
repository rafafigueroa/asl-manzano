#ifndef _MZN_STRING_UTILITIES_H_
#define _MZN_STRING_UTILITIES_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

// --------------------------------------------------------------------- //
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

// --------------------------------------------------------------------- //
inline
void expect_string(std::string const & expected, std::string const & token) {

    if (token != expected) {
        std::stringstream ss;
        ss << "expected:" << expected << " got:" << token;
        throw std::logic_error{ss.str()};
    }
}

#endif // _MZN_STRING_UTILITIES_H_
