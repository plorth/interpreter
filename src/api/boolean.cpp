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
   * Word: and
   * Prototype: boolean
   *
   * Takes:
   * - boolean
   * - boolean
   *
   * Gives:
   * - boolean
   *
   * Logical AND. Returns true if both values are true.
   */
  static void
  w_and(class context& context)
  {
    bool a;
    bool b;

    if (context.pop_boolean(a) && context.pop_boolean(b))
    {
      context << (a && b);
    }
  }

  /**
   * Word: or
   * Prototype: boolean
   *
   * Takes:
   * - boolean
   * - boolean
   *
   * Gives:
   * - boolean
   *
   * Logical OR. Returns true if either one of the values are true.
   */
  static void
  w_or(class context& context)
  {
    bool a;
    bool b;

    if (context.pop_boolean(a) && context.pop_boolean(b))
    {
      context << (a || b);
    }
  }

  /**
   * Word: xor
   * Prototype: boolean
   *
   * Takes:
   * - boolean
   * - boolean
   *
   * Gives:
   * - boolean
   *
   * Exclusive OR.
   */
  static void
  w_xor(class context& context)
  {
    bool a;
    bool b;

    if (context.pop_boolean(a) && context.pop_boolean(b))
    {
      context << (b != a && (b || a));
    }
  }

  /**
   * Word: not
   * Prototype: boolean
   *
   * Takes:
   * - boolean
   *
   * Gives:
   * - boolean
   *
   * Negates given boolean value.
   */
  static void
  w_not(class context& context)
  {
    bool a;

    if (context.pop_boolean(a))
    {
      context << !a;
    }
  }

  /**
   * Word: ?
   * Prototype: boolean
   *
   * Takes:
   * - any
   * - any
   * - boolean
   *
   * Gives:
   * - any
   *
   * Selects between two values based on the boolean value. First value is
   * returned when the boolean value is true and the second one is returned
   * when it's false.
   *
   *     "greater" "less" 5 6 > ?  #=> "less"
   */
  static void
  w_select(class context& context)
  {
    value::ref true_value;
    value::ref false_value;
    bool condition;

    if (
      context.pop_boolean(condition) &&
      context.pop(false_value) &&
      context.pop(true_value)
    )
    {
      context << (condition ? true_value : false_value);
    }
  }


  extern "C" const runtime::dictionary_definition boolean =
  {
    { U"and", w_and },
    { U"or", w_or },
    { U"xor", w_xor },
    { U"not", w_not },
    { U"?", w_select }
  };
}
