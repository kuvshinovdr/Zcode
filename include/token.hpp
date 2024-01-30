/// @file  zcode/token.hpp
/// @brief Token definition and auxiliary operations.
#ifndef ZCODE_TOKEN_HPP_INCLUDED
#define ZCODE_TOKEN_HPP_INCLUDED

#include <string>
#include <vector>

#include <string_view>
#include <span>


/// @brief Zcode lexer namespace
namespace zcode::lex
{

  using std::u8string;
  using std::u8string_view;

  /// @brief       Split an input string into \n-separated lines.
  /// @param input An input string that must persist while the resulting vector is being used.
  /// @return      a vector of string views each holding one line of the input
  [[nodiscard]] auto split_to_lines(u8string const& input)
    -> std::vector<u8string_view>;

}

#endif//ZCODE_TOKEN_HPP_INCLUDED
