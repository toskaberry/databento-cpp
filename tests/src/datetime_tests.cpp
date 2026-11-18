#include <gtest/gtest.h>

#include <chrono>
#include <locale>
#include <string>

#include "databento/datetime.hpp"

namespace databento::tests {
TEST(DateFromIso8601IntTests, TestValid) {
  ASSERT_EQ(databento::DateFromIso8601Int(20201110), "2020-11-10");
}

TEST(DateFromIso8601IntTests, TestPadding) {
  ASSERT_EQ(databento::DateFromIso8601Int(20190801), "2019-08-01");
}

namespace {
struct GroupThousands : std::numpunct<char> {
  std::string do_grouping() const override { return "\3"; }
  char do_thousands_sep() const override { return ','; }
};

class GroupedLocaleTests : public ::testing::Test {
 protected:
  void SetUp() override {
    std::locale::global(std::locale{std::locale::classic(), new GroupThousands});
  }
  void TearDown() override { std::locale::global(original_); }

 private:
  std::locale original_;
};
}  // namespace

TEST_F(GroupedLocaleTests, CalendarDateUsesUngroupedYear) {
  EXPECT_EQ(DateFromIso8601Int(20201110), "2020-11-10");
}

TEST_F(GroupedLocaleTests, TimestampUsesNineUngroupedFractionalDigits) {
  EXPECT_EQ(ToIso8601(UnixNanos{std::chrono::nanoseconds{123456789}}),
            "1970-01-01T00:00:00.123456789Z");
}
}  // namespace databento::tests
