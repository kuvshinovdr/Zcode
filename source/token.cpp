/// @file  token.cpp
/// @brief Implementation of zcode/token.hpp
#include <zcode/token.hpp>

#include <iterator>
#include <algorithm>
#include <ranges>

namespace zcode::lex
{

  auto split_to_lines(u8string const& input)
    -> std::vector<u8string_view>
  {
    std::vector<u8string_view> result(std::ranges::count(input, u8'\n') + 1);
    std::ranges::transform(
      std::views::split(input, u8'\n'),
      result.begin(), [](auto&& r) { return u8string_view(r); });
    return result;
  }

}
