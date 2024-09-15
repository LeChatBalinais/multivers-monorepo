#include "run-all-tests.h"
#include <gtest/gtest.h>

// TEST(HelloTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);

//   EXPECT_EQ(multivers::calculate(), 3);
// }

int main() { mltvs_test::runAllTests(); }