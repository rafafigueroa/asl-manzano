// ** Exceptions ** //
// Manzano software

#include "mzn_except.h"
namespace mzn {

// -------------------------------------------------------------------------- //
std::string Exception::core_what() const noexcept {

    return std::string(std::string("  class : ") + e_class +
                       std::string("\n  func  : ") + e_function +
                       std::string("\n  msg   : ") + e_msg);
}

// -------------------------------------------------------------------------- //
Exception::Exception(std::string const & in_e_header,
                     std::string const & in_e_class,
                     std::string const & in_e_function,
                     std::string const & in_e_msg) :
        e_header(in_e_header),
        e_class(in_e_class),
        e_function(in_e_function),
        e_msg(in_e_msg),
        e_what(

            std::string(
                "\n_______________________________________________________") +

            e_header +

            core_what() +

            std::string(
                "\n!_____________________________________________________!\n")
        ) {}

// -------------------------------------------------------------------------- //
char const * Exception::what() const noexcept {
    return e_what.c_str();
}

// -------------------------------------------------------------------------- //
std::string InfoException::header() const {

    std::string const header(
        "\n!                  * ERROR (info) *                   !\n");
    return header;
}

// -------------------------------------------------------------------------- //
std::string WarningException::header() const {

    std::string const header(
        "\n!               ** ERROR (warning) **                 !\n");
    return header;
}

// -------------------------------------------------------------------------- //
std::string FatalException::header() const {

    std::string const header(
        "\n!             ***** ERROR (fatal) *****               !\n");
    return header;
}

} // <- mzn

