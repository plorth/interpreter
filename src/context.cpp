/*
 * Copyright (c) 2017-2023, Rauli Laine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <plorth/context.hpp>

namespace plorth
{
  context::context(const std::shared_ptr<class runtime>& runtime)
    : m_runtime(runtime) {}

  void
  context::error(
    enum value::error::code code,
    const std::u32string& message,
    const std::optional<parser::position>& position
  )
  {
    m_error = new_error(
      code,
      message,
      !position && (m_position.file.empty() || m_position.line > 0)
        ? m_position
        : position
    );
  }

  bool
  context::pop()
  {
    if (!m_data.empty())
    {
      m_data.pop_back();

      return true;
    }
    error(value::error::code::range, U"Stack underflow.");

    return false;
  }

  bool
  context::pop(enum value::type type)
  {
    if (!m_data.empty())
    {
      const auto& value = m_data.back();

      if (value->type() != type)
      {
        error(
          value::error::code::type,
          U"Expected " +
          value::to_string(type) +
          U", got " +
          value::to_string(value->type()) +
          U" instead."
        );

        return false;
      }
      m_data.pop_back();

      return true;
    }
    error(value::error::code::range, U"Stack underflow.");

    return false;
  }

  bool
  context::pop(value_type& slot)
  {
    if (!m_data.empty())
    {
      slot = m_data.back();
      m_data.pop_back();

      return true;
    }
    error(value::error::code::range, U"Stack underflow.");

    return false;
  }

  bool
  context::pop(value_type& slot, enum value::type type)
  {
    if (!m_data.empty())
    {
      const auto& value = m_data.back();

      if (value->type() != type)
      {
        error(
          value::error::code::type,
          U"Expected " +
          value::to_string(type) +
          U", got " +
          value::to_string(value->type()) +
          U" instead."
        );

        return false;
      }
      slot = value;
      m_data.pop_back();

      return true;
    }
    error(value::error::code::range, U"Stack underflow.");

    return false;
  }

  template<class T>
  static bool
  typed_pop(
    class context* context,
    std::shared_ptr<T>& slot,
    enum value::type type
  )
  {
    value::ref value_slot;

    if (context->pop(value_slot, type))
    {
      slot = std::static_pointer_cast<T>(value_slot);

      return true;
    }

    return false;
  }

  bool
  context::pop_array(std::shared_ptr<value::array>& slot)
  {
    return typed_pop(this, slot, value::type::array);
  }

  bool
  context::pop_boolean(bool& slot)
  {
    value::ref value_slot;

    if (pop(value_slot, value::type::boolean))
    {
      slot = std::static_pointer_cast<value::boolean>(value_slot)->value();

      return true;
    }

    return false;
  }

  bool
  context::pop_error(std::shared_ptr<value::error>& slot)
  {
    return typed_pop(this, slot, value::type::error);
  }

  bool
  context::pop_number(std::shared_ptr<value::number>& slot)
  {
    return typed_pop(this, slot, value::type::number);
  }

  bool
  context::pop_object(std::shared_ptr<value::object>& slot)
  {
    return typed_pop(this, slot, value::type::object);
  }

  bool
  context::pop_quote(std::shared_ptr<value::quote>& slot)
  {
    return typed_pop(this, slot, value::type::quote);
  }

  bool
  context::pop_string(value::string::reference slot)
  {
    value::ref value_slot;

    if (pop(value_slot, value::type::string))
    {
      slot = std::static_pointer_cast<value::string>(value_slot)->value();

      return true;
    }

    return false;
  }

  bool
  context::pop_symbol(std::shared_ptr<value::symbol>& slot)
  {
    return typed_pop(this, slot, value::type::symbol);
  }

  bool
  context::pop_word(std::shared_ptr<value::word>& slot)
  {
    return typed_pop(this, slot, value::type::word);
  }
}
