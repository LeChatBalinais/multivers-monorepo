#include "sandbox-describe/json-serialization-utilities.hh"

namespace tst {
boost::json::value parse_stream(std::istream &is, boost::json::error_code &ec) {
  try {
    boost::json::stream_parser p;
    std::string ret;
    char buffer[4096];
    while (is.read(buffer, sizeof(buffer)))
      ret.append(buffer, sizeof(buffer));
    ret.append(buffer, is.gcount());
    p.write(ret);
    p.finish(ec);
    if (ec)
      return {};
    return p.release();
  } catch (boost::exception &exp) {
    return {};
  }
}
boost::json::value parse_stream(std::istream &is) {
  boost::json::error_code ec;
  return parse_stream(is, ec);
}
} // namespace tst