#ifndef _MZN_BASIC_JSON_H_
#define _MZN_BASIC_JSON_H_

#include "nlohmann_json.hpp"

//! use nlohmann library for the Json object
namespace mzn {
using Json = nlohmann::json;
} // <- mzn

#endif
