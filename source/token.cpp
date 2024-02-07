/// @file  token.cpp
/// @brief Implementation of zcode/token.hpp
#include <zcode/token.hpp>

#include <iterator>
#include <algorithm>
#include <ranges>

namespace zcode::lex
{

  /////////////////////////////////////////////////////////////////////////////
  // Auxiliaries

  auto split_to_lines(u8string const& input)
    -> std::vector<u8string_view>
  {
    std::vector<u8string_view> result(std::ranges::count(input, u8'\n') + 1);
    std::ranges::transform(
      std::views::split(input, u8'\n'),
      result.begin(), [](auto&& r) { return u8string_view(r); });
    return result;
  }


  /////////////////////////////////////////////////////////////////////////////
  // Token

  Token::Token(Token_kind kind, u8string_view view, Source_position src_pos)
    : _kind(kind), _view(view), _src_pos(src_pos) 
  {
    // Fill _value corresponding to the token kind.
    switch (kind)
    {
    case zcode::lex::Token_kind::comment:
      _make_comment_value();
      break;
    case zcode::lex::Token_kind::number:
      _make_number_value();
      break;
    case zcode::lex::Token_kind::string:
      _make_string_value();
      break;
    default:
      // Do nothing leaving _value empty.
      break;
    }
  }


  void Token::_make_comment_value()
  {
    if (_view.starts_with(u8"//"sv))
      _value = trim(_view.substr(2));
    else
      _kind = Token_kind::invalid;
  }
  

  void Token::_make_string_value()
  {
    // TBD
  }
  

  void Token::_make_number_value()
  {
    // TBD
  }

}
