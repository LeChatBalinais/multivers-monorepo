#include "run-all-tests.h"
#include <gtest/gtest.h>
#include <multivers/multivers.h>

namespace mltvs_test {
TEST(TestName, Foo) { testMultivers(); }

int runAllTests() {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
} // namespace mltvs_test