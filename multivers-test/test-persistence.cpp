#include "test-persistence.h"
#include <gtest/gtest.h>
#include <multivers/multivers.h>

namespace mvs {

void testPersistence() { EXPECT_EQ(testMultivers(), 3); }
} // namespace mvs