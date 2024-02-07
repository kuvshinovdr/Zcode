/// @file  test_token.cpp
/// @brief Tests for token.cpp.
#include "test_head.hpp"
#include <zcode/token.hpp>

TEST_SUITE("zcode/token")
{
  TEST_CASE("zcode::lex::trim")
  {
    using namespace zcode::lex;
    REQUIRE(ltrim(u8"abc rtc"sv) == u8"abc rtc"sv);
    REQUIRE(ltrim(u8"abc rtc \t\r\n"sv)  == u8"abc rtc \t\r\n"sv);
    REQUIRE(ltrim(u8" \t\r\n abc rtc"sv) == u8"abc rtc"sv);
    REQUIRE(rtrim(u8"abc rtc"sv) == u8"abc rtc"sv);
    REQUIRE(rtrim(u8" \t\r\n abc rtc"sv) == u8" \t\r\n abc rtc"sv);
    REQUIRE(rtrim(u8"abc rtc \t\r\n"sv)  == u8"abc rtc"sv);
    REQUIRE(trim(u8"abc rtc"sv) == u8"abc rtc"sv);
    REQUIRE(trim(u8"\r\n\t abc \tdaa \t\r\n") == u8"abc \tdaa");
  }

  TEST_CASE("zcode::lex::split_to_lines")
  {
    std::u8string const input = u8"several\nlines\nare\n\nhere";
    auto const lines = zcode::lex::split_to_lines(input); // input must live after the split
    REQUIRE(lines.size() == 5);
    int i = 0;
    for (auto word: { u8"several", u8"lines", u8"are", u8"", u8"here" })
      CHECK(lines[i++] == word);
  }
}
