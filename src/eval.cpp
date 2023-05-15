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
  eval_array(
    class context& context,
    const std::shared_ptr<value::array>& array,
    value::ref& slot
  )
  {
    const auto& elements = array->elements();
    value::array::container_type result;

    result.reserve(elements.size());
    for (const auto& element : elements)
    {
      value::ref element_slot;

      if (!context.eval(element, element_slot))
      {
        return false;
      }
      result.push_back(element_slot);
    }
    slot = value::new_array(result);

    return true;
  }

  static bool
  eval_object(
    class context& context,
    const std::shared_ptr<value::object>& object,
    value::ref& slot
  )
  {
    value::object::container_type result;

    for (const auto& property : object->properties())
    {
      value::ref value_slot;

      if (!context.eval(property.second, value_slot))
      {
        return false;
      }
      result[property.first] = value_slot;
    }
    slot = value::new_object(result);

    return true;
  }

  static bool
  eval_symbol(
    class context& context,
    const std::shared_ptr<value::symbol>& symbol,
    value::ref& slot
  )
  {
    const auto& id = symbol->id();

    if (!id.compare(U"null"))
    {
      slot = context.runtime()->null_instance();
    }
    else if (!id.compare(U"true"))
    {
      slot = context.runtime()->true_instance();
    }
    else if (!id.compare(U"false"))
    {
      slot = context.runtime()->false_instance();
    }
    else if (!id.compare(U"drop"))
    {
      return context.pop(slot);
    }
    else if (value::number::is_valid(id))
    {
      slot = std::make_shared<value::number>(id);
    } else {
      context.error(
        value::error::code::syntax,
        U"Unexpected `" + id + U"'; Missing value.",
        symbol->position()
      );

      return false;
    }

    return true;
  }

  static bool
  eval_value(
    class context& context,
    const value::ref& value,
    value::ref& slot
  )
  {
    slot = value;

    return true;
  }

  static bool
  eval_word(
    class context& context,
    const std::shared_ptr<value::word>& word,
    value::ref& slot
  )
  {
    context.error(
      value::error::code::syntax,
      U"Unexpected word declaration; Missing value."
    );

    return false;
  }

  bool
  context::eval(const value::ref& value, value::ref& slot)
  {
    if (!value)
    {
      slot = m_runtime->null_instance();

      return true;
    }
    switch (value->type())
    {
      case value::type::array:
        return eval_array(
          *this,
          std::static_pointer_cast<value::array>(value),
          slot
        );

      case value::type::object:
        return eval_object(
          *this,
          std::static_pointer_cast<value::object>(value),
          slot
        );

      case value::type::symbol:
        return eval_symbol(
          *this,
          std::static_pointer_cast<value::symbol>(value),
          slot
        );

      case value::type::word:
        return eval_word(
          *this,
          std::static_pointer_cast<value::word>(value),
          slot
        );

      default:
        return eval_value(*this, value, slot);
    }
  }
}
