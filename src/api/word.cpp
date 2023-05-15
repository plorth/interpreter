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
   * Word: symbol
   * Prototype: word
   *
   * Takes:
   * - word
   *
   * Gives:
   * - word
   * - symbol
   *
   * Extracts symbol from the word and places it onto top of the stack.
   */
  static void
  w_symbol(class context& context)
  {
    std::shared_ptr<value::word> word;

    if (context.pop_word(word))
    {
      context << word << word->symbol();
    }
  }

  /**
   * Word: define
   * Prototype: word
   *
   * Takes:
   * - any
   * - word
   *
   * Inserts given word into current local dictionary with given value.
   */
  static void
  w_define(class context& context)
  {
    std::shared_ptr<value::word> word;
    value::ref value;

    if (context.pop_word(word) && context.pop(value))
    {
      context.dictionary()[word->symbol()->id()] = value;
    }
  }

  /**
   * Word: delete
   * Prototype: word
   *
   * Takes:
   * - word
   *
   * Deletes value from local dictionary defined by the given word. Reference
   * error will be thrown if the local dictionary does not contain such value.
   */
  static void
  w_delete(class context& context)
  {
    std::shared_ptr<value::word> word;

    if (context.pop_word(word))
    {
      auto& dictionary = context.dictionary();
      auto it = dictionary.find(word->symbol()->id());

      if (it == std::end(dictionary))
      {
        context.error(
          value::error::code::reference,
          U"Unrecognized word: `" + word->symbol()->id() + U"'"
        );
      } else {
        dictionary.erase(it);
      }
    }
  }

  extern "C" const runtime::dictionary_definition word =
  {
    { U"symbol", w_symbol },
    { U"define", w_define },
    { U"delete", w_delete }
  };
}
