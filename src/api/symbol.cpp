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

namespace plorth::api
{
  /**
   * Word: position
   * Prototype: symbol
   *
   * Takes:
   * - symbol
   *
   * Gives:
   * - symbol
   * - object|null
   *
   * Returns position in source code where the symbols was encountered, or null
   * if no such information is available. If symbol caching has been enabled in
   * the interpreter, source code position is not stored in symbols.
   *
   * Position is returnedd as object with `filename`, `line` and `column`
   * properties.
   */
  static void
  w_position(class context& context)
  {
    std::shared_ptr<value::symbol> symbol;

    if (!context.pop_symbol(symbol))
    {
      return;
    }
    if (const auto position = symbol->position())
    {
      context << value::new_object({
        { U"filename", value::new_string(position->file) },
        { U"line", value::new_int(position->line) },
        { U"column", value::new_int(position->column) }
      });
    } else {
      context << nullptr;
    }
  }

  /**
   * Word: call
   * Prototype: symbol
   *
   * Takes:
   * - symbol
   *
   * Resolves given symbol into word or value, depending on the contents of the
   * data stack, local dictionary and global dictionary and executes it. If the
   * symbol does not resolve into any kind of word or value, number conversion
   * is attempted on it. If that also fails, reference error will be thrown.
   */
  static void
  w_call(class context& context)
  {
    std::shared_ptr<value::symbol> symbol;

    if (context.pop_symbol(symbol))
    {
      context.exec(symbol);
    }
  }

  /**
   * Word: >word
   * Prototype: symbol
   *
   * Takes:
   * - symbol
   *
   * Gives:
   * - word
   *
   * Converts given symbol into word.
   */
  static void
  w_to_word(class context& context)
  {
    std::shared_ptr<value::symbol> symbol;

    if (context.pop_symbol(symbol))
    {
      context << value::new_word(symbol);
    }
  }

  extern "C" const runtime::dictionary_definition symbol =
  {
    // Getters.
    { U"position", w_position },

    // Methods.
    { U"call", w_call },

    // Converters.
    { U">word", w_to_word }
  };
}
