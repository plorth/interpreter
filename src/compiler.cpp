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
#include <plorth/parser.hpp>

#include <plorth/context.hpp>

namespace plorth::value
{
  static value::ref compile_token(const std::shared_ptr<parser::ast::token>&);

  std::shared_ptr<quote>
  quote::compile(
    class context& context,
    const std::u32string& source,
    const std::u32string& filename,
    int line,
    int column
  )
  {
    auto begin = std::begin(source);
    const auto end = std::end(source);
    parser::position position = { filename, line, column };
    const auto result = parser::parse(begin, end, position);
    std::vector<ref> values;

    if (!result)
    {
      auto error_message = result.error().message;

      if (error_message.empty())
      {
        error_message = U"Unknown error.";
      }
      context.error(
        error::code::syntax,
        error_message, result.error().position
      );

      return nullptr;
    }
    values.reserve(result.value().size());
    for (const auto& token : result.value())
    {
      if (const auto value = compile_token(token))
      {
        values.push_back(value);
      }
    }

    return value::new_compiled_quote(values);
  }

  static std::shared_ptr<array>
  compile_array_token(const std::shared_ptr<parser::ast::array>& token)
  {
    const auto& elements = token->elements();
    const auto size = elements.size();
    array::container_type result;

    result.reserve(size);
    for (std::size_t i = 0; i < size; ++i)
    {
      if (const auto element = compile_token(elements[i]))
      {
        result.push_back(element);
      }
    }

    return value::new_array(result);
  }

  static std::shared_ptr<object>
  compile_object_token(const std::shared_ptr<parser::ast::object>& token)
  {
    const auto& properties = token->properties();
    const auto size = properties.size();
    object::container_type result;

    for (std::size_t i = 0; i < size; ++i)
    {
      const auto& property = properties[i];

      if (const auto value = compile_token(property.second))
      {
        result[property.first] = value;
      }
    }

    return value::new_object(result);
  }

  static std::shared_ptr<quote>
  compile_quote_token(const std::shared_ptr<parser::ast::quote>& token)
  {
    const auto& children = token->children();
    const auto size = children.size();
    array::container_type result;

    result.reserve(size);
    for (std::size_t i = 0; i < size; ++i)
    {
      if (const auto value = compile_token(children[i]))
      {
        result.push_back(value);
      }
    }

    return value::new_compiled_quote(result);
  }

  static std::shared_ptr<string>
  compile_string_token(const std::shared_ptr<parser::ast::string>& token)
  {
    return value::new_string(token->value());
  }

  static std::shared_ptr<symbol>
  compile_symbol_token(const std::shared_ptr<parser::ast::symbol>& token)
  {
    return value::new_symbol(token->id(), token->position());
  }

  static std::shared_ptr<word>
  compile_word_token(const std::shared_ptr<parser::ast::word>& token)
  {
    return value::new_word(compile_symbol_token(token->symbol()));
  }

  static value::ref
  compile_token(const std::shared_ptr<parser::ast::token>& token)
  {
    if (!token)
    {
      return nullptr;
    }
    switch (token->type())
    {
      case parser::ast::token::type::array:
        return compile_array_token(
          std::static_pointer_cast<parser::ast::array>(token)
        );

      case parser::ast::token::type::object:
        return compile_object_token(
          std::static_pointer_cast<parser::ast::object>(token)
        );

      case parser::ast::token::type::quote:
        return compile_quote_token(
          std::static_pointer_cast<parser::ast::quote>(token)
        );

      case parser::ast::token::type::string:
        return compile_string_token(
          std::static_pointer_cast<parser::ast::string>(token)
        );

      case parser::ast::token::type::symbol:
        return compile_symbol_token(
          std::static_pointer_cast<parser::ast::symbol>(token)
        );

      case parser::ast::token::type::word:
        return compile_word_token(
          std::static_pointer_cast<parser::ast::word>(token)
        );
    }

    return nullptr;
  }
}
