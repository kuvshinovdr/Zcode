/// @file  zcode/token.hpp
/// @brief Token definition and auxiliary operations.
#ifndef ZCODE_TOKEN_HPP_INCLUDED
#define ZCODE_TOKEN_HPP_INCLUDED

#include <cstdint>

#include <iterator>
#include <string>
#include <vector>

#include <string_view>
#include <span>

#include <format>


/// @brief Zcode lexer namespace.
namespace zcode::lex
{

  using std::u8string;
  using std::u8string_view;

  /// @brief       Split an input string into \n-separated lines.
  /// @param input an input string that must persist while the resulting vector is being used
  /// @return      a vector of string views each holding one line of the input
  [[nodiscard]] auto split_to_lines(u8string const& input)
    -> std::vector<u8string_view>;

  /// @brief Position of a character (maybe the first character of a token) in a source file (line:column).
  struct Source_position
  {
    std::uint32_t line_index   = 0;
    std::uint32_t column_index = 0;
  };

  /// @brief Possible token kinds.
  enum class Token_kind
  {
    none,
    invalid,
    comment,
    identifier,
    punctuation,
    number,
    string,
    size
  };

}


namespace std
{
  template <>
  struct formatter<zcode::lex::Token_kind> : formatter<char const*>
  {
    auto format(zcode::lex::Token_kind tk, format_context& ctx)
    {
      constexpr static char const* tk_str[]
      {
        "<none>",
        "<invalid>",
        "comment",
        "identifier",
        "punctuation",
        "number",
        "string",
        "<size>"
      };

      static_assert(size(tk_str) - 1 == static_cast<int>(zcode::lex::Token_kind::size));
      return formatter<char const*>::format(tk_str[static_cast<int>(tk)], ctx);
    }
  };
}

#endif//ZCODE_TOKEN_HPP_INCLUDED
