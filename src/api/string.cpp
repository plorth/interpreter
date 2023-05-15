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
#include <plorth/parser/utils.hpp>

#include <plorth/context.hpp>

namespace plorth::api
{
  /**
   * Word: >quote
   * Prototype: string
   *
   * Takes:
   * - string
   *
   * Gives:
   * - quote
   *
   * Compiles given string of source code into a quote.
   */
  static void
  w_to_quote(class context& context)
  {
    value::string::value_type source;

    if (context.pop_string(source))
    {
      const auto quote = value::quote::compile(context, source);

      if (quote)
      {
        context << quote;
      }
    }
  }

  /**
   * Word: >symbol
   * Prototype: string
   *
   * Takes:
   * - string
   *
   * Gives:
   * - symbol
   *
   * Converts given string into symbol. Value error will be thrown if the string
   * is empty or contains whitespace or non-symbolic characters such as separators.
   */
  static void
  w_to_symbol(class context& context)
  {
    value::string::value_type string;

    if (context.pop_string(string))
    {
      const auto length = string.length();

      if (!length)
      {
        context.error(
          value::error::code::value,
          U"Cannot construct empty symbol."
        );
        return;
      }
      for (value::string::size_type i = 0; i < length; ++i)
      {
        if (!parser::utils::isword(string[i]))
        {
          context.error(
            value::error::code::value,
            U"Cannot convert " +
            value::string::format_json(string) +
            U" into symbol."
          );
          return;
        }
      }
      context << value::new_symbol(string);
    }
  }

  extern "C" const runtime::dictionary_definition string =
  {
    // Converters.
    { U">quote", w_to_quote },
    { U">symbol", w_to_symbol }
  };
}
