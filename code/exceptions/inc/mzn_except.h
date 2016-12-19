// ** error Object ** //
// Manzano Software

#ifndef _MZN_EXCEPTIONS_H_
#define _MZN_EXCEPTIONS_H_

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

namespace mzn {

// -------------------------------------------------------------------------- //
class Exception : public std::exception {

public:
    Exception(std::string const & in_e_header,
              std::string const & in_e_class,
              std::string const & in_e_function,
              std::string const & in_e_msg);

    ~Exception() = default;

    std::string core_what() const noexcept;
    char const * what() const noexcept override;

    std::string const e_header;
    std::string const e_class;
    std::string const e_function;
    std::string const e_msg;
    std::string const e_what;
};

// -------------------------------------------------------------------------- //
class InfoException : public Exception {

public:
    InfoException(std::string const & in_e_class,
                  std::string const & in_e_function,
                  std::string const & in_e_msg) :
            Exception( header(), in_e_class, in_e_function, in_e_msg ) {}

    std::string header() const;
};

// -------------------------------------------------------------------------- //
class WarningException : public Exception {

public:
    WarningException(std::string const & in_e_class,
                     std::string const & in_e_function,
                     std::string const & in_e_msg) :
            Exception( header(), in_e_class, in_e_function, in_e_msg ) {}

    std::string header() const;
};

// -------------------------------------------------------------------------- //
class FatalException : public Exception {

public:
    FatalException(std::string const & in_e_class,
                   std::string const & in_e_function,
                   std::string const & in_e_msg) :
            Exception( header(), in_e_class, in_e_function, in_e_msg ) {}

    std::string header() const;
};

} // << mzn
#endif // _MZN_EXCEPTIONS_H_
