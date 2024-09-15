#pragma once
#include <boost/json.hpp>
#include <istream>
#include <ostream>

namespace tst {
boost::json::value parse_stream(std::istream &is, boost::json::error_code &ec);
boost::json::value parse_stream(std::istream &is);
} // namespace tst