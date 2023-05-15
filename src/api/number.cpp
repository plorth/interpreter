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
   * Word: range
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - array
   *
   * Takes two numbers and constructs an array that has range of numbers
   * between the two given numbers.
   */
  static void
  w_range(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      value::array::container_type result;

      if (a->is_real() || b->is_real())
      {
        const auto limit = a->real_value();
        auto current = b->real_value();

        while (current < limit)
        {
          result.push_back(value::new_real(current++));
        }
      } else {
        const auto limit = a->int_value();
        auto current = b->int_value();

        while (current < limit)
        {
          result.push_back(value::new_int(current++));
        }
      }
      context << result;
    }
  }

  /**
   * Word: times
   * Prototype: number
   *
   * Takes:
   * - quote
   * - number
   *
   * Executes a quote given number of times.
   */
  static void
  w_times(class context& context)
  {
    std::shared_ptr<value::number> number;
    std::shared_ptr<value::quote> quote;

    if (context.pop_number(number) && context.pop_quote(quote))
    {
      auto count = number->int_value();

      if (count < 0)
      {
        count = -count;
      }
      while (count > 0)
      {
        --count;
        if (!quote->call(context))
        {
          return;
        }
      }
    }
  }

  template<class IntOperator, class RealOperator>
  static void
  binary_op(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;
    value::number::real_type result;

    if (!context.pop_number(a) || !context.pop_number(b))
    {
      return;
    }

    result = RealOperator()(a->real_value(), b->real_value());

    if (
      a->is_int() &&
      b->is_int() &&
      std::fabs(result) <= value::number::int_max
    )
    {
      // Repeat the operation with full integer precision.
      context << IntOperator()(a->int_value(), b->int_value());
      return;
    }

    // Otherwise keep it real as it seems to be integer overflow or either of
    // the arguments are real numbers.
    context << result;
  }

  /**
   * Word: +
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Performs addition on the two given numbers.
   */
  static void
  w_add(class context& context)
  {
    binary_op<
      std::plus<value::number::int_type>,
      std::plus<value::number::real_type>
    >(context);
  }

  /**
   * Word: -
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Subtracts the second number from the first and returns the result.
   */
  static void
  w_sub(class context& context)
  {
    binary_op<
      std::minus<value::number::int_type>,
      std::minus<value::number::real_type>
    >(context);
  }

  /**
   * Word: *
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Performs multiplication on the two given numbers.
   */
  static void
  w_mul(class context& context)
  {
    binary_op<
      std::multiplies<value::number::int_type>,
      std::multiplies<value::number::real_type>
    >(context);
  }

  /**
   * Word: /
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Divides the first number by the second and returns the result.
   */
  static void
  w_div(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      // TODO: This makes weird divisions, find out why.
      context << (a->real_value() / b->real_value());
    }
  }

  /**
   * Word: %
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Computes the modulo of the first number with respect to the second number
   * i.e. the remainder after floor division.
   */
  static void
  w_mod(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      auto dividend = a->real_value();
      auto divider = b->real_value();
      auto result = std::fmod(dividend, divider);

      if (std::signbit(dividend) != std::signbit(divider))
      {
        result += divider;
      }
      context << result;
    }
  }

  template<class Operator>
  static void
  bit_op(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      context << Operator()(a->int_value(), b->int_value());
    }
  }

  /**
   * Word: &
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Performs bitwise and on the two given numbers.
   */
  static void
  w_bit_and(class context& context)
  {
    bit_op<std::bit_and<value::number::int_type>>(context);
  }

  /**
   * Word: |
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Performs bitwise or on the two given numbers.
   */
  static void
  w_bit_or(class context& context)
  {
    bit_op<std::bit_or<value::number::int_type>>(context);
  }

  /**
   * Word: ^
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Performs bitwise xor on the two given numbers.
   */
  static void
  w_bit_xor(class context& context)
  {
    bit_op<std::bit_xor<value::number::int_type>>(context);
  }

  /**
   * Word: >>
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Returns the first value with bits shifted right by the second value.
   */
  static void
  w_shift_right(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      context << (a->int_value() >> b->int_value());
    }
  }

  /**
   * Word: <<
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - number
   *
   * Returns the first value with bits shifted left by the second value.
   */
  static void
  w_shift_left(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      context << (a->int_value() << b->int_value());
    }
  }

  /**
   * Word: ~
   * Prototype: number
   *
   * Takes:
   * - number
   *
   * Gives:
   * - number
   *
   * Flips the bits of the value.
   */
  static void
  w_bit_not(class context& context)
  {
    std::shared_ptr<value::number> a;

    if (context.pop_number(a))
    {
      context << ~a->int_value();
    }
  }

  template<class IntOperator, class RealOperator>
  static void
  compare_op(class context& context)
  {
    std::shared_ptr<value::number> a;
    std::shared_ptr<value::number> b;

    if (context.pop_number(a) && context.pop_number(b))
    {
      if (a->is_real() || b->is_real())
      {
        context << RealOperator()(a->real_value(), b->real_value());
      } else {
        context << IntOperator()(a->int_value(), b->int_value());
      }
    }
  }

  /**
   * Word: <
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - boolean
   *
   * Returns true if the first number is less than the second one.
   */
  static void
  w_lt(class context& context)
  {
    compare_op<
      std::less<value::number::int_type>,
      std::less<value::number::real_type>
    >(context);
  }

  /**
   * Word: >
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - boolean
   *
   * Returns true if the first number is greater than the second one.
   */
  static void
  w_gt(class context& context)
  {
    compare_op<
      std::greater<value::number::int_type>,
      std::greater<value::number::real_type>
    >(context);
  }

  /**
   * Word: <=
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - boolean
   *
   * Returns true if the first number is less than or equal to the second one.
   */
  static void
  w_le(class context& context)
  {
    compare_op<
      std::less_equal<value::number::int_type>,
      std::less_equal<value::number::real_type>
    >(context);
  }

  /**
   * Word: >=
   * Prototype: number
   *
   * Takes:
   * - number
   * - number
   *
   * Gives:
   * - boolean
   *
   * Returns true if the first number is greater than or equal to the second
   * one.
   */
  static void
  w_ge(class context& context)
  {
    compare_op<
      std::greater_equal<value::number::int_type>,
      std::greater_equal<value::number::real_type>
    >(context);
  }

  extern "C" const runtime::dictionary_definition number =
  {
    { U"range", w_range },
    { U"times", w_times },

    // Arithmetic.
    { U"+", w_add },
    { U"-", w_sub },
    { U"*", w_mul },
    { U"/", w_div },
    { U"%", w_mod },

    // Bit operations.
    { U"&", w_bit_and },
    { U"|", w_bit_or },
    { U"^", w_bit_xor },
    { U">>", w_shift_right },
    { U"<<", w_shift_left },
    { U"~", w_bit_not },

    // Comparison.
    { U"<", w_lt },
    { U">", w_gt },
    { U"<=", w_le },
    { U">=", w_ge }
  };
}
