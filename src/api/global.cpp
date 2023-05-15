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
#include <chrono>
#include <random>

#include <plorth/context.hpp>

namespace plorth::api
{
  /**
   * Word: true
   *
   * Gives:
   * - boolean
   *
   * Pushes the boolean value true onto stack.
   */
  static void
  w_true(class context& context)
  {
    context << true;
  }

  /**
   * Word: false
   *
   * Gives:
   * - boolean
   *
   * Pushes the boolean value false onto stack.
   */
  static void
  w_false(class context& context)
  {
    context << false;
  }

  /**
   * Word: null
   *
   * Gives:
   * - null
   *
   * Pushes the null value onto stack.
   */
  static void
  w_null(class context& context)
  {
    context << nullptr;
  }

  /**
   * Word: e
   *
   * Gives:
   * - number
   *
   * Pushes Euler's number onto stack.
   */
  static void
  w_e(class context& context)
  {
    context << M_E;
  }

  /**
   * Word: pi
   *
   * Gives:
   * - number
   *
   * Pushes the value of pi onto stack.
   */
  static void
  w_pi(class context& context)
  {
    context << M_PI;
  }

  /**
   * Word: inf
   *
   * Gives:
   * - number
   *
   * Pushes the value of positive infinity onto stack.
   */
  static void
  w_inf(class context& context)
  {
    context << INFINITY;
  }

  /**
   * Word: -inf
   *
   * Gives:
   * - number
   *
   * Pushes the value of negative infinity onto stack.
   */
  static void
  w_negative_inf(class context& context)
  {
    context << -INFINITY;
  }

  /**
   * Word: nan
   *
   * Gives:
   * - number
   *
   * Pushes the value of NaN (not a number) onto stack.
   */
  static void
  w_nan(class context& context)
  {
    context << NAN;
  }

  /**
   * Word: nop
   *
   * Does nothing. Can be used to construct empty quotes.
   */
  static void
  w_nop(context&) {}

  /**
   * Word: if
   *
   * Takes:
   * - boolean
   * - quote
   *
   * Executes quote if the boolean value is true.
   */
  static void
  w_if(class context& context)
  {
    bool condition;
    std::shared_ptr<value::quote> quote;

    if (context.pop_quote(quote) && context.pop_boolean(condition) && condition)
    {
      quote->call(context);
    }
  }

  /**
   * Word: if-else
   *
   * Takes:
   * - boolean
   * - quote
   * - quote
   *
   * Calls first quote if boolean value is true, second quote otherwise.
   */
  static void
  w_if_else(class context& context)
  {
    bool condition;
    std::shared_ptr<value::quote> then_quote;
    std::shared_ptr<value::quote> else_quote;

    if (
      !context.pop_quote(else_quote) ||
      !context.pop_quote(then_quote) ||
      !context.pop_boolean(condition)
    )
    {
      return;
    }

    if (condition)
    {
      then_quote->call(context);
    } else {
      else_quote->call(context);
    }
  }

  /**
   * Word: while
   *
   * Takes:
   * - quote
   * - quote
   *
   * Executes second quote as long as the first quote returns true.
   */
  static void
  w_while(class context& context)
  {
    std::shared_ptr<value::quote> test;
    std::shared_ptr<value::quote> body;

    if (!context.pop_quote(body) || !context.pop_quote(test))
    {
      return;
    }

    for (;;)
    {
      bool b;

      if (
        !test->call(context) ||
        !context.pop_boolean(b) ||
        !b ||
        !body->call(context)
      )
      {
        return;
      }
    }
  }

  /**
   * Word: try
   *
   * Takes:
   * - quote
   * - quote
   *
   * Executes first quote and if it throws an error, calls second quote with
   * the error on top of the stack.
   */
  static void
  w_try(class context& context)
  {
    std::shared_ptr<value::quote> try_quote;
    std::shared_ptr<value::quote> catch_quote;

    if (!context.pop_quote(catch_quote) || !context.pop_quote(try_quote))
    {
      return;
    }

    if (!try_quote->call(context))
    {
      context << context.error();
      context.error().reset();
      catch_quote->call(context);
    }
  }

  /**
   * Word: try-else
   *
   * Takes:
   * - quote
   * - quote
   * - quote
   *
   * Executes first quote and if it throws an error, calls second quote with
   * the error on top of the stack. If no error was thrown, third quote will
   * be called instead.
   */
  static void
  w_try_else(class context& context)
  {
    std::shared_ptr<value::quote> try_quote;
    std::shared_ptr<value::quote> catch_quote;
    std::shared_ptr<value::quote> else_quote;

    if (
      !context.pop_quote(else_quote) ||
      !context.pop_quote(catch_quote) ||
      !context.pop_quote(try_quote)
    )
    {
      return;
    }

    if (!try_quote->call(context))
    {
      context << context.error();
      context.error().reset();
      catch_quote->call(context);
    } else {
      else_quote->call(context);
    }
  }

  /**
   * Word: depth
   *
   * Gives:
   * - number
   *
   * Pushes current depth of the stack onto stack.
   */
  static void
  w_depth(class context& context)
  {
    context << static_cast<value::number::int_type>(context.data().size());
  }

  /**
   * Word: clear
   *
   * Clears the entire stack of current context.
   */
  static void
  w_clear(class context& context)
  {
    context.data().clear();
  }

  /**
   * Word: drop
   *
   * Takes:
   * - any
   *
   * Discards topmost value from the stack.
   *
   *     1 drop #=> empty stack
   */
  static void
  w_drop(class context& context)
  {
    context.pop();
  }

  /**
   * Word: dup
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - any
   *
   * Duplicates the topmost value of the stack.
   *
   *     1 dup #=> 1 1
   */
  static void
  w_dup(class context& context)
  {
    value::ref value;

    if (context.pop(value))
    {
      context << value << value;
    }
  }

  /**
   * Word: nip
   *
   * Takes:
   * - any
   * - any
   *
   * Gives:
   * - any
   *
   * Drops the first value and pushes the second value onto stack.
   *
   *     1 2 nip #=> 2
   */
  static void
  w_nip(class context& context)
  {
    value::ref value;

    if (context.pop(value) && context.pop())
    {
      context << value;
    }
  }

  /**
   * Word: over
   *
   * Takes:
   * - any
   * - any
   *
   * Gives:
   * - any
   * - any
   * - any
   *
   * Copies second topmost value of the stack into topmost value of the
   * stack.
   *
   *     1 2 over #=> 1 2 1
   */
  static void
  w_over(class context& context)
  {
    value::ref a;
    value::ref b;

    if (context.pop(a) && context.pop(b))
    {
      context << b << a << b;
    }
  }

  /**
   * Word: rot
   *
   * Takes:
   * - any
   * - any
   * - any
   *
   * Gives:
   * - any
   * - any
   * - any
   *
   * Rotates the three topmost values on the stack.
   *
   *     1 2 3 rot #=> 2 3 1
   */
  static void
  w_rot(class context& context)
  {
    value::ref a;
    value::ref b;
    value::ref c;

    if (context.pop(a) && context.pop(b) && context.pop(c))
    {
      context << b << a << c;
    }
  }

  /**
   * Word: swap
   *
   * Takes:
   * - any
   * - any
   *
   * Gives:
   * - any
   * - any
   *
   * Swaps positions of the two topmost values on the stack.
   *
   *     1 2 swap #=> 2 1
   */
  static void
  w_swap(class context& context)
  {
    value::ref a;
    value::ref b;

    if (context.pop(a) && context.pop(b))
    {
      context << a << b;
    }
  }

  /**
   * Word: tuck
   *
   * Takes:
   * - any
   * - any
   *
   * Gives:
   * - any
   * - any
   * - any
   *
   * Copies the topmost value of the stack as the third topmost value of the
   * stack.
   *
   *     1 2 tuck #=> 2 1 2
   */
  static void
  w_tuck(class context& context)
  {
    value::ref a;
    value::ref b;

    if (context.pop(a) && context.pop(b))
    {
      context << a << b << a;
    }
  }

  static inline void
  type_test(class context& context, enum value::type type)
  {
    value::ref value;

    if (context.pop(value))
    {
      context << value << (value->type() == type);
    }
  }

  /**
   * Word: array?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is an array.
   */
  static void
  w_is_array(class context& context)
  {
    type_test(context, value::type::array);
  }

  /**
   * Word: boolean?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is a boolean.
   */
  static void
  w_is_boolean(class context& context)
  {
    type_test(context, value::type::boolean);
  }

  /**
   * Word: error?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is an error.
   */
  static void
  w_is_error(class context& context)
  {
    type_test(context, value::type::error);
  }

  /**
   * Word: null?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is null.
   */
  static void
  w_is_null(class context& context)
  {
    type_test(context, value::type::null);
  }

  /**
   * Word: number?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is a number.
   */
  static void
  w_is_number(class context& context)
  {
    type_test(context, value::type::null);
  }

  /**
   * Word: object?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is an object.
   */
  static void
  w_is_object(class context& context)
  {
    type_test(context, value::type::object);
  }

  /**
   * Word: quote?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is a quote.
   */
  static void
  w_is_quote(class context& context)
  {
    type_test(context, value::type::quote);
  }

  /**
   * Word: string?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is a string.
   */
  static void
  w_is_string(class context& context)
  {
    type_test(context, value::type::string);
  }

  /**
   * Word: symbol?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is symbol.
   */
  static void
  w_is_symbol(class context& context)
  {
    type_test(context, value::type::symbol);
  }

  /**
   * Word: word?
   *
   * Takes:
   * - any
   *
   * Gives:
   * - any
   * - boolean
   *
   * Returns true if the topmost value of the stack is word.
   */
  static void
  w_is_word(class context& context)
  {
    type_test(context, value::type::word);
  }

  static inline void
  dictionary_to_object(
    class context& context,
    const runtime::dictionary_type& dictionary
  )
  {
    value::object::container_type result;

    for (const auto& word : dictionary)
    {
      result[word.first] = word.second;
    }
    context << result;
  }

  /**
   * Word: globals
   *
   * Gives:
   * - object
   *
   * Returns the global dictionary as an object.
   */
  static void
  w_globals(class context& context)
  {
    dictionary_to_object(context, context.runtime()->dictionary());
  }

  /**
   * Word: locals
   *
   * Gives:
   * - object
   *
   * Returns the local dictionary of current execution context as an object.
   */
  static void
  w_locals(class context& context)
  {
    dictionary_to_object(context, context.dictionary());
  }

  /**
   * Word: write
   *
   * Takes:
   * - any
   *
   * Writes given value to the output. Throws I/O error if no output is
   * available or an I/O error occurs.
   */
  static void
  w_write(class context& context)
  {
    auto& output = context.runtime()->output();
    value::ref value;

    if (!context.pop(value))
    {
      return;
    }
    if (output)
    {
      if (!output->write(value))
      {
        context.error(value::error::code::io, U"Writing to output failed.");
      }
    } else {
      context.error(value::error::code::io, U"No output available.");
    }
  }

  /**
   * Word: read
   *
   * Gives:
   * - any
   *
   * Reads an value from the input. Throws I/O error if no input is available
   * or an I/O error occurs.
   */
  static void
  w_read(class context& context)
  {
    auto& input = context.runtime()->input();

    if (input)
    {
      if (const auto value = input->read())
      {
        context << value;
      } else {
        context.error(value::error::code::io, U"Reading from input failed.");
      }
    } else {
      context.error(value::error::code::io, U"No input available.");
    }
  }

  /**
   * Word: rand
   *
   * Gives:
   * - number
   *
   * Generates random integer number and pushes it into the stack.
   */
  static void
  w_rand(class context& context)
  {
    std::random_device os_seed;
    std::mt19937 generator(os_seed());
    std::uniform_int_distribution<value::number::int_type> distribute(
      value::number::int_min,
      value::number::int_max
    );

    context << distribute(generator);
  }

  /**
   * Word: now
   *
   * Gives:
   * - number
   *
   * Returns the number of seconds that have elapsed since the  Unix epoch
   * (1 January 1970 00:00:00 UTC) rounded to the nearest integer.
   */
  static void
  w_now(class context& context)
  {
    const auto timestamp = std::chrono::system_clock::now().time_since_epoch();

    context << value::new_int(
      std::chrono::duration_cast<std::chrono::seconds>(timestamp).count()
    );
  }

  /**
   * Word: =
   *
   * Takes:
   * - any
   * - any
   *
   * Gives:
   * - boolean
   *
   * Tests whether the two topmost values of the stack are equal.
   */
  static void
  w_eq(class context& context)
  {
    value::ref a;
    value::ref b;

    if (context.pop(a) && context.pop(b))
    {
      context << a->equals(b);
    }
  }

  /**
   * Word: !=
   *
   * Takes:
   * - any
   * - any
   *
   * Gives:
   * - boolean
   *
   * Tests whether the two topmost values of the stack are not equal.
   */
  static void
  w_ne(class context& context)
  {
    value::ref a;
    value::ref b;

    if (context.pop(a) && context.pop(b))
    {
      context << !a->equals(b);
    }
  }

  /**
   * Word: >boolean
   *
   * Takes:
   * - any
   *
   * Gives:
   * - boolean
   *
   * Converts the topmost value of the stack into a boolean. Null and false
   * will become false while everything else will become true.
   */
  static void
  w_to_boolean(class context& context)
  {
    value::ref value;

    if (!context.pop(value))
    {
      return;
    }
    else if (value->type() == value::type::boolean)
    {
      context << value;
    } else {
      context << (value->type() != value::type::null);
    }
  }

  /**
   * Word: >string
   *
   * Takes:
   * - any
   *
   * Gives:
   * - string
   *
   * Converts the topmost value of the stack into a string. Null will become
   * an empty string.
   */
  static void
  w_to_string(class context& context)
  {
    value::ref value;

    if (context.pop(value))
    {
      context << value->to_string();
    }
  }

  /**
   * Word: >source
   *
   * Takes:
   * - any
   *
   * Gives:
   * - string
   *
   * Converts the topmost value of the stack into a string that most accurately
   * represents what the value would look like in source code.
   */
  static void
  w_to_source(class context& context)
  {
    value::ref value;

    if (context.pop(value))
    {
      context << value->to_source();
    }
  }

  static inline void
  construct_error(class context& context, enum value::error::code code)
  {
    value::ref value;

    if (context.pop(value))
    {
      std::u32string message;

      if (value->type() == value::type::string)
      {
        message = std::static_pointer_cast<value::string>(value)->value();
      }
      else if (value->type() != value::type::null)
      {
        context.error(
          value::error::code::type,
          U"Expected a string, got " +
          value::to_string(value->type()) +
          U" instead."
        );
        return;
      }
      context << value::new_error(code, message, context.position());
    }
  }

  /**
   * Word: syntax-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of syntax error with given optional error message
   * and places it on top of the stack.
   */
  static void
  w_syntax_error(class context& context)
  {
    construct_error(context, value::error::code::syntax);
  }

  /**
   * Word: reference-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of reference error with given optional error
   * message and places it on top of the stack.
   */
  static void
  w_reference_error(class context& context)
  {
    construct_error(context, value::error::code::reference);
  }

  /**
   * Word: type-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Construct an instance of type error with given optional error message
   * and places it on top of the stack.
   */
  static void
  w_type_error(class context& context)
  {
    construct_error(context, value::error::code::type);
  }

  /**
   * Word: value-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of value error with given optional error
   * message and places it on top of the stack.
   */
  static void
  w_value_error(class context& context)
  {
    construct_error(context, value::error::code::value);
  }

  /**
   * Word: range-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of range error with given optional error
   * message and places it on top of the stack.
   */
  static void
  w_range_error(class context& context)
  {
    construct_error(context, value::error::code::range);
  }

  /**
   * Word: import-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of import error with given optional error
   * message and places it on top of the stack.
   */
  static void
  w_import_error(class context& context)
  {
    construct_error(context, value::error::code::import);
  }

  /**
   * Word: io-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of I/O error with given optional error
   * message and places it on top of the stack.
   */
  static void
  w_io_error(class context& context)
  {
    construct_error(context, value::error::code::io);
  }

  /**
   * Word: unknown-error
   *
   * Takes:
   * - string|null
   *
   * Gives:
   * - error
   *
   * Constructs an instance of unknown error with given optional error
   * message and places it on top of the stack.
   */
  static void
  w_unknown_error(class context& context)
  {
    construct_error(context, value::error::code::unknown);
  }

  extern "C" const runtime::dictionary_definition global =
  {
    // Constants.
    { U"true", w_true },
    { U"false", w_false },
    { U"null", w_null },
    { U"e", w_e },
    { U"pi", w_pi },
    { U"inf", w_inf },
    { U"-inf", w_negative_inf },
    { U"nan", w_nan },

    // Core logic.
    { U"nop", w_nop },
    { U"if", w_if },
    { U"if-else", w_if_else },
    { U"while", w_while },
    { U"try", w_try },
    { U"try-else", w_try_else },

    // Stack manipulation.
    { U"depth", w_depth },
    { U"clear", w_clear },
    { U"drop", w_drop },
    { U"dup", w_dup },
    { U"nip", w_nip },
    { U"over", w_over },
    { U"rot", w_rot },
    { U"swap", w_swap },
    { U"tuck", w_tuck },

    // Type testing.
    { U"array?", w_is_array },
    { U"boolean?", w_is_boolean },
    { U"error?", w_is_error },
    { U"null?", w_is_null },
    { U"number?", w_is_number },
    { U"object?", w_is_object },
    { U"quote?", w_is_quote },
    { U"string?", w_is_string },
    { U"symbol?", w_is_symbol },
    { U"word?", w_is_word },

    // Dictionary access.
    { U"globals", w_globals },
    { U"locals", w_locals },

    // I/O.
    { U"write", w_write },
    { U"read", w_read },

    // Random utilities.
    { U"rand", w_rand },
    { U"now", w_now },

    // Global operators.
    { U"=", w_eq },
    { U"!=", w_ne },

    // Conversions.
    { U">boolean", w_to_boolean },
    { U">string", w_to_string },
    { U">source", w_to_source },

    // Error constructors.
    { U"syntax-error", w_syntax_error },
    { U"reference-error", w_reference_error },
    { U"type-error", w_type_error },
    { U"value-error", w_value_error },
    { U"range-error", w_range_error },
    { U"import-error", w_import_error },
    { U"io-error", w_io_error },
    { U"unknown-error", w_unknown_error }
  };
}
