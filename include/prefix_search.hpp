/// @file  zcode/prefix_search.hpp
/// @brief Match the longest known token with a prefix of an input string.
#ifndef PREFIX_SEARCH_HPP_INCLUDED
#define PREFIX_SEARCH_HPP_INCLUDED

#include <memory>
#include <concepts>
#include <string_view>
#include <string>

#include <cassert> // TODO: replace with a more convenient tool.


namespace zcode::lex
{

  class Prefix_search;
  using Prefix_search_uptr = std::unique_ptr<Prefix_search>;

  /// @brief Register prefixes with some appointed int values and match the longest in the given input.
  class Prefix_search
  {
  private:
    static constexpr int const no_index = -1;
    static constexpr int const min_cap  =  8;

  public:
    /// @brief The special value signalizing that a prefix has no value i.e. it's unknown.
    static constexpr int const no_value = -1;

    /// @brief Create an empty prefix search tree.
    Prefix_search() noexcept = default;

    /// @brief Set the starting capacity, it must be greater than 0, less than 257 and be a power of two.
    explicit Prefix_search(int starting_cap)
    {
      assert(starting_cap <= 256);
      assert((starting_cap & (starting_cap - 1)) == 0);

      _keytable = std::make_unique<char[]>(starting_cap);
      _child    = std::make_unique<Prefix_search_uptr[]>(starting_cap);

      _cap = starting_cap;
    }

    /// @brief        Update (or insert) a value corresponding to the given C-string prefix.
    /// @param prefix not null NUL-terminated character array pointer
    /// @param value  the value to be attached to the prefix
    void update(char const* prefix, int value)
    {
      assert(prefix != nullptr);

      if (value == no_value)
        return forget(prefix);

      auto node = this;
      while (int const key = *prefix++)
      {
        int const index = node->_find_or_insert(key);
        node            = node->_child[index].get();
      }

      node->_value = value;
    }

    /// @brief        Remove the prefix value.
    /// @param prefix not null NUL-terminated character array pointer
    void forget(char const* prefix) noexcept
    {
      assert(prefix != nullptr);
      _forget(prefix);
    }

    /// @brief       Try to match the longest known prefix in the input string.
    /// @param input not null NUL-terminated character array pointer
    /// @return      found value, no_value if no known prefix has been found
    [[nodiscard]] int match(char const* input) const noexcept
    {
      assert(input != nullptr);

      auto node = this;
      int value = _value;

      while (int const key = *input++)
      {
        int const index = node->_find(key);
        if (index == no_index)
          break;
        
        node  = node->_child[index].get();
        value = node->_value;
      }

      return value;
    }

    /// @brief        Enumerate all known prefixes and their values.
    /// @param report the function to be called with every known prefix, value pair
    void list(std::invocable<std::string_view, int> auto report) const
    {
      std::string buffer;
      _list(buffer, report);
    }

    void swap(Prefix_search& other) noexcept
    {
      int const other_cap   = other._cap;
      int const other_size  = other._size;
      int const other_value = other._value;
      other._cap   = _cap;
      other._size  = _size;
      other._value = _value;
      _cap   = other_cap;
      _size  = other_size;
      _value = other._value;

      _keytable .swap(other._keytable);
      _child    .swap(other._child);
    }

    [[nodiscard]] int capacity() const noexcept
    {
      return _cap;
    }

    [[nodiscard]] int size() const noexcept
    {
      return _size;
    }

    [[nodiscard]] bool is_empty() const noexcept
    {
      return _size == 0;
    }

  private:
    int _cap   = 0;
    int _size  = 0;
    int _value = no_value;
    
    std::unique_ptr<char[]>               _keytable; // a hashtable.
    std::unique_ptr<Prefix_search_uptr[]> _child;

    // Find position of the key, return -1 if no key has been found.
    [[nodiscard]] int _find(char const key) const noexcept
    {
      int const bit_mask = _cap - 1;
      int       index    = key & bit_mask;
      
      auto const keys = _keytable.get();
      if (keys[index] == key)
        return index;

      for (int i = index;;)
      {
        i = (i + 1) & bit_mask;
        auto const key_i = keys[i];
        if (key_i == key)
          return i;
        if (key_i == '\0' || i == index)
          return no_index;
      }
    }

    // Find position of the key or insert it.
    [[nodiscard]] int _find_or_insert(char const key)
    {
    restart:
      int const bit_mask = _cap - 1;
      int       index    = key & bit_mask;

      auto const keys      = _keytable.get();
      auto const key_index = keys[index];
      
      for (int i = index;;)
      {
        auto const key_i = keys[i];
        if (key_i == key)
          return i;

        if (key_i == '\0')
        {
          keys[i]   = key;
          _child[i] = std::make_unique<Prefix_search>();
          return i;
        }

        i = (i + 1) & bit_mask;
        if (i == index)
        {
          _grow_step();
          goto restart;
        }
      }
    }

    // Find the first empty position after the given one.
    [[nodiscard]] static int _find_next_empty(
        char const* const keytable, 
        int               index, 
        int const         bit_mask
      ) noexcept
    {
      // Linear open address character hashtable.
      // Eternal loop if the table is full.
      while (keytable[index] != '\0')
        index = (index + 1) & bit_mask;
      return index;
    }

    [[nodiscard]] int _find_next_empty(int index) const noexcept
    {
      return _find_next_empty(_keytable.get(), index, _cap - 1);
    }

    // Rehash into this node (it must be empty).
    Prefix_search& _rehash_into_this(Prefix_search& from) noexcept
    {
      assert(_size == 0);
      assert(_cap >= from._size);
      assert(this != &from);

      auto const old_cap = from._cap;
      auto const new_cap = _cap;

      auto const old_keytable_ptr = from._keytable.get();
      auto const new_keytable_ptr = _keytable.get();
      
      auto const old_child_ptr    = from._child.get();
      auto const new_child_ptr    = _child.get();

      auto const bit_mask = new_cap - 1;
      for (int i = 0; i < old_cap; ++i)
      {
        int const key = old_keytable_ptr[i];
        if (key == '\0')
          continue;

        int const new_key_index = _find_next_empty(new_keytable_ptr, key & bit_mask, bit_mask);
        new_keytable_ptr [new_key_index] = key;
        new_child_ptr    [new_key_index] = std::move(old_child_ptr[i]);
      }

      _size  = from._size;
      _value = from._value;
      return *this;
    }

    // Double capacity and rehash.
    void _grow_step()
    {
      assert(_cap < 256);
      Prefix_search(_cap << 1)._rehash_into_this(*this).swap(*this);
    }

    // Halve capacity and rehash.
    // Do nothing if there are too many elements.
    void _shrink_step()
    {
      if (_cap <= min_cap || _cap < (_size << 1))
        return;

      Prefix_search(_cap >> 1)._rehash_into_this(*this).swap(*this);
    }

    bool _forget(char const* prefix)
    {
      auto const key = *prefix;
      if (key == '\0')
      {
        if (_value != no_value)
          return false;

        _value = no_value;
        return true;
      }

      auto const index = _find(key);
      if (index == no_index)
        return false;

      auto& child = *_child[index];
      if (!child._forget(prefix + 1))
        return false;

      if (child.is_empty())
      {
        _child[index].reset();
        if ((--_size << 2) < _cap)
          _shrink_step();
      }

      return true;
    }

    void _list(std::string& buffer, auto report) const
    {
      if (_value != no_value)
        report(std::string_view{buffer}, _value);

      for (int i = 0; i < _cap; ++i)
      {
        if (auto const key = _keytable[i]; key != '\0')
        {
          buffer.push_back(key);
          _child[i]->_list(buffer, report);
          buffer.pop_back();
        }
      }
    }
  };

}

#endif//PREFIX_SEARCH_HPP_INCLUDED
