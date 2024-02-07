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
  using std::operator""sv;


  /// @brief Trim all ASCII space and control characters from the left of a string view.
  /// @param sv the string view to be trimmed
  /// @return trimmed string view
  [[nodiscard]] constexpr auto ltrim(u8string_view sv) noexcept
  {
    while (!sv.empty() && sv.front() <= u8' ')
      sv.remove_prefix(1);
    return sv;
  }


  /// @brief Trim all ASCII space and control characters from the right of a string view.
  /// @param sv the string view to be trimmed
  /// @return trimmed string view
  [[nodiscard]] constexpr auto rtrim(u8string_view sv) noexcept
  {
    while (!sv.empty() && sv.back() <= u8' ')
      sv.remove_suffix(1);
    return sv;
  }


  /// @brief Trim all ASCII space and control characters from both ends of a string view.
  /// @param sv the string view to be trimmed
  /// @return trimmed string view
  [[nodiscard]] constexpr auto trim(u8string_view sv) noexcept
  {
    return rtrim(ltrim(sv));
  }


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


  /// @brief Token representation.
  class Token
  {
  public:
    /// @brief Initializes a "none" token.
    constexpr Token() = default;

    /// @brief Initialize a token.
    /// @param kind    choose the kind of the token
    /// @param view    the view pointing to the source code, where this token is located
    /// @param src_pos the coordinates of the first character of the token
    Token(Token_kind kind, u8string_view view, Source_position src_pos);

    /// @brief Get the token kind.
    [[nodiscard]] constexpr auto kind() const noexcept
    {
      return _kind;
    }

    /// @brief Get the string view of the token as it is.
    [[nodiscard]] constexpr auto view() const noexcept
    {
      return _view;
    }

    /// @brief Get the source position of this token.
    [[nodiscard]] constexpr auto source_position() const noexcept
    {
      return _src_pos;
    }

    /// @brief Get the value of the token represented by a view into a string stored in the token object.
    [[nodiscard]] constexpr auto value() const noexcept
    {
      return u8string_view(_value);
    }

  private:
    Token_kind      _kind = Token_kind::none;
    u8string_view   _view;
    Source_position _src_pos;
    u8string        _value;

    void _make_comment_value();
    void _make_string_value();
    void _make_number_value();
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
