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
#pragma once

#include <plorth/value/array.hpp>
#include <plorth/value/boolean.hpp>
#include <plorth/value/error.hpp>
#include <plorth/value/null.hpp>
#include <plorth/value/number.hpp>
#include <plorth/value/object.hpp>
#include <plorth/value/quote.hpp>
#include <plorth/value/string.hpp>
#include <plorth/value/symbol.hpp>
#include <plorth/value/word.hpp>

namespace plorth::value
{
  /**
   * Implementation of visitor pattern for Plorth values.
   */
  template<class... Args>
  class visitor
  {
  public:
    virtual void visit_array(
      const std::shared_ptr<array>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_boolean(
      const std::shared_ptr<boolean>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_error(
      const std::shared_ptr<error>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_null(
      const std::shared_ptr<null>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_number(
      const std::shared_ptr<number>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_object(
      const std::shared_ptr<object>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_quote(
      const std::shared_ptr<quote>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_string(
      const std::shared_ptr<quote>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_symbol(
      const std::shared_ptr<quote>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_word(
      const std::shared_ptr<quote>& value,
      Args... args
    ) const
    {
      visit_value(value, args...);
    }

    virtual void visit_value(const ref& value, Args... args) {}

    void visit(const ref& value, Args... args) const
    {
      if (!value)
      {
        visit_null(nullptr);
        return;
      }

      switch (value->type())
      {
        case type::array:
          visit_array(std::static_pointer_cast<array>(value), args...);
          break;

        case type::boolean:
          visit_boolean(std::static_pointer_cast<boolean>(value), args...);
          break;

        case type::error:
          visit_error(std::static_pointer_cast<error>(value), args...);
          break;

        case type::null:
          visit_null(std::static_pointer_cast<null>(value), args...);
          break;

        case type::number:
          visit_number(std::static_pointer_cast<number>(value), args...);
          break;

        case type::object:
          visit_object(std::static_pointer_cast<object>(value), args...);
          break;

        case type::quote:
          visit_quote(std::static_pointer_cast<quote>(value), args...);
          break;

        case type::string:
          visit_string(std::static_pointer_cast<string>(value), args...);
          break;

        case type::symbol:
          visit_symbol(std::static_pointer_cast<symbol>(value), args...);
          break;

        case type::word:
          visit_word(std::static_pointer_cast<word>(value), args...);
          break;
      }
    }
  }
}
