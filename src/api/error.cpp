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
   * Word: code
   * Prototype: error
   *
   * Takes:
   * - error
   *
   * Gives:
   * - error
   * - number
   *
   * Returns error code extracted from the error in numeric form.
   */
  static void
  w_code(class context& context)
  {
    std::shared_ptr<value::error> error;

    if (context.pop_error(error))
    {
      context << error << static_cast<value::number::int_type>(error->code());
    }
  }

  /**
   * Word: message
   * Prototype: error
   *
   * Takes:
   * - error
   *
   * Gives:
   * - error
   * - string|null
   *
   * Returns error message extracted from the error, or null if the error does
   * not have any error message.
   */
  static void
  w_message(class context& context)
  {
    std::shared_ptr<value::error> error;

    if (context.pop_error(error))
    {
      const auto& message = error->message();

      context << error;
      if (message.empty())
      {
        context << nullptr;
      } else {
        context << message;
      }
    }
  }

  /**
   * Word: position
   * Prototype: error
   *
   * Takes
   * - error
   *
   * Gives:
   * - error
   * - object|null
   *
   * Returns position in the source code where the error occurred, or null if
   * no such information is available.
   *
   * Position is returned as object with `file`, `line` and `column`
   * properties.
   */
  static void
  w_position(class context& context)
  {
    std::shared_ptr<value::error> error;

    if (context.pop_error(error))
    {
      const auto& position = error->position();

      context << error;
      if (position)
      {
        context << value::new_object({
          { U"file", value::new_string(position->file) },
          { U"line", value::new_int(position->line) },
          { U"column", value::new_int(position->column) }
        });
      } else {
        context << nullptr;
      }
    }
  }

  /**
   * Word: throw
   * Prototype: error
   *
   * Takes:
   * - error
   *
   * Sets given error as current error of the execution context.
   */
  static void
  w_throw(class context& context)
  {
    std::shared_ptr<value::error> error;

    if (context.pop_error(error))
    {
      context.error() = error;
    }
  }

  extern "C" const runtime::dictionary_definition error =
  {
    { U"code", w_code },
    { U"message", w_message },
    { U"position", w_position },
    { U"throw", w_throw }
  };
}
