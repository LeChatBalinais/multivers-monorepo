#pragma once
#include "sandbox-describe/nested-struct.hh"
#include <istream>
#include <ostream>

namespace schema {

MaybeElement parse(std::istream &is);

void marshall(std::ostream &os, const Element &el);

} // namespace schema