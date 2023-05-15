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
  static bool
  exec_symbol(
    class context& context,
    const std::shared_ptr<value::symbol>& symbol
  )
  {
    const auto& position = symbol->position();
    const auto& id = symbol->id();

    // Update source code position of the context, if the symbol has such
    // information.
    if (position)
    {
      context.position() = *position;
    }

    // Look up prototype of the current item.
    {
      const auto& stack = context.data();

      if (!stack.empty())
      {
        const auto prototype = stack.back()->prototype(context.runtime());
        value::ref slot;

        if (prototype && prototype->property(context.runtime(), id, slot))
        {
          if (slot->type() == value::type::quote)
          {
            return std::static_pointer_cast<value::quote>(slot)->call(context);
          }
          context << slot;

          return true;
        }
      }
    }

    // Look for a word from dictionary of current context.
    {
      const auto& dictionary = context.dictionary();
      const auto word = dictionary.find(symbol->id());

      if (word != std::end(dictionary))
      {
        const auto& value = word->second;

        if (value->type() == value::type::quote)
        {
          return std::static_pointer_cast<value::quote>(value)->call(context);
        }
        context << value;

        return true;
      }
    }

    // Look from global dictionary.
    {
      const auto& dictionary = context.runtime()->dictionary();
      const auto word = dictionary.find(symbol->id());

      if (word != std::end(dictionary))
      {
        const auto& value = word->second;

        if (value->type() == value::type::quote)
        {
          return std::static_pointer_cast<value::quote>(value)->call(context);
        }
        context << value;

        return true;
      }
    }

    // If the name can be converted into a number, then just do that.
    if (value::number::is_valid(id))
    {
      context << std::make_shared<value::number>(id);

      return true;
    }

    // Otherwise it's an reference error.
    context.error(
      value::error::code::reference,
      U"Unrecognized word: `" + id + U"'",
      position
    );

    return false;
  }

  static bool
  exec_word(class context& context, const std::shared_ptr<value::word>& word)
  {
    value::ref value;

    if (!context.pop(value))
    {
      return false;
    }
    context.dictionary()[word->symbol()->id()] = value;

    return true;
  }

  static bool
  exec_value(class context& context, const value::ref& value)
  {
    value::ref slot;

    if (!context.eval(value, slot))
    {
      return false;
    }
    context << slot;

    return true;
  }

  bool
  context::exec(const value::ref& value)
  {
    if (!value)
    {
      m_data.push_back(m_runtime->null_instance());

      return true;
    }
    switch (value->type())
    {
      case value::type::symbol:
        return exec_symbol(
          *this,
          std::static_pointer_cast<value::symbol>(value)
        );

      case value::type::word:
        return exec_word(
          *this,
          std::static_pointer_cast<value::word>(value)
        );

      default:
        return exec_value(*this, value);
    }
  }
}
