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
  inline std::shared_ptr<array>
  new_array(const std::initializer_list<value::ref>& elements)
  {
    return std::make_shared<array>(elements);
  }

  inline std::shared_ptr<array>
  new_array(const array::container_type& elements)
  {
    return std::make_shared<array>(elements);
  }

  inline std::shared_ptr<boolean>
  new_boolean(boolean::value_type value)
  {
    return std::make_shared<boolean>(value);
  }

  inline std::shared_ptr<error>
  new_error(
    enum error::code code,
    const std::u32string& message,
    const std::optional<parser::position>& position = std::nullopt
  )
  {
    return std::make_shared<error>(code, message, position);
  }

  inline std::shared_ptr<null>
  new_null()
  {
    return std::make_shared<null>();
  }

  inline std::shared_ptr<number>
  new_int(number::int_type value)
  {
    return std::make_shared<number>(value);
  }

  inline std::shared_ptr<number>
  new_real(number::real_type value)
  {
    return std::make_shared<number>(value);
  }

  inline std::shared_ptr<object>
  new_object(const std::initializer_list<object::value_type>& properties)
  {
    return std::make_shared<object>(properties);
  }

  inline std::shared_ptr<object>
  new_object(const object::container_type& properties)
  {
    return std::make_shared<object>(properties);
  }

  inline std::shared_ptr<quote>
  new_compiled_quote(const std::vector<ref>& values)
  {
    return std::make_shared<quote>(values);
  }

  inline std::shared_ptr<quote>
  new_native_quote(const quote::callback_type& callback)
  {
    return std::make_shared<quote>(callback);
  }

  inline std::shared_ptr<string>
  new_string(value::string::const_reference value)
  {
    return std::make_shared<string>(value);
  }

  inline std::shared_ptr<symbol>
  new_symbol(
    const std::u32string& id,
    const std::optional<parser::position>& position = std::nullopt
  )
  {
    return std::make_shared<symbol>(id, position);
  }

  inline std::shared_ptr<word>
  new_word(const std::shared_ptr<class symbol>& symbol)
  {
    return std::make_shared<word>(symbol);
  }
}
