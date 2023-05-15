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
   * Word: length
   * Prototype: array
   *
   * Takes:
   * - array
   *
   * Gives:
   * - array
   * - number
   *
   * Returns the number of elements in the array, while keeping the array on
   * the stack.
   */
  static void
  w_length(class context& context)
  {
    std::shared_ptr<value::array> array;

    if (context.pop_array(array))
    {
      context << value::new_int(array->size());
    }
  }

  /**
   * Word: for-each
   * Prototype: array
   *
   * Takes:
   * - quote
   * - array
   *
   * Runs quote once for every element in the array.
   */
  static void
  w_for_each(class context& context)
  {
    std::shared_ptr<value::array> array;
    std::shared_ptr<value::quote> quote;

    if (context.pop_array(array) && context.pop_quote(quote))
    {
      for (const auto& element : array->elements())
      {
        context << element;
        if (!quote->call(context))
        {
          return;
        }
      }
    }
  }

  /**
   * Word: map
   * Prototype: array
   *
   * Takes:
   * - quote
   * - array
   *
   * Gives:
   * - array
   *
   * Applies quote once for each element in the array and constructs a new
   * array from values returned by the quote.
   */
  static void
  w_map(class context& context)
  {
    std::shared_ptr<value::array> array;
    std::shared_ptr<value::quote> quote;

    if (context.pop_array(array) && context.pop_quote(quote))
    {
      const auto& elements = array->elements();
      value::array::container_type result;

      result.reserve(elements.size());
      for (const auto& element : elements)
      {
        value::ref value;

        context << element;
        if (!quote->call(context) || !context.pop(value))
        {
          return;
        }
        result.push_back(value);
      }
      context << result;
    }
  }

  /**
   * Word: filter
   * Prototype: array
   *
   * Takes:
   * - quote
   * - array
   *
   * Gives:
   * - array
   *
   * Removes elements of the array that do not satisfy the provided testing
   * quote.
   */
  static void
  w_filter(class context& context)
  {
    std::shared_ptr<value::array> array;
    std::shared_ptr<value::quote> quote;

    if (context.pop_array(array) && context.pop_quote(quote))
    {
      value::array::container_type result;

      for (const auto& element : array->elements())
      {
        bool quote_result;

        context << element;
        if (!quote->call(context) || !context.pop_boolean(quote_result))
        {
          return;
        }
        else if (quote_result)
        {
          result.push_back(element);
        }
      }
      context << result;
    }
  }

  /**
   * Word: reduce
   * Prototype: array
   *
   * Takes:
   * - quote
   * - array
   *
   * Gives:
   * - any
   *
   * Applies given quote against an accumulator and each element in the array
   * to reduce it into a single value.
   */
  static void
  w_reduce(class context& context)
  {
    std::shared_ptr<value::array> array;
    std::shared_ptr<value::quote> quote;

    if (context.pop_array(array) && context.pop_quote(quote))
    {
      const auto& elements = array->elements();
      const auto size = elements.size();
      value::ref result;

      if (size == 0)
      {
        context.error(
          value::error::code::range,
          U"Cannot reduce empty array."
        );
        return;
      }
      result = elements[0];
      for (value::array::size_type i = 1; i < size; ++i)
      {
        context << result << elements[i];
        if (!quote->call(context) || !context.pop(result))
        {
          return;
        }
      }
      context << result;
    }
  }

  static void
  do_flatten(
    const std::shared_ptr<value::array>& input,
    std::vector<value::ref>& output
  )
  {
    const auto& elements = input->elements();

    output.reserve(output.size() + elements.size());
    for (const auto& element : elements)
    {
      if (element->type() == value::type::array)
      {
        do_flatten(std::static_pointer_cast<value::array>(element), output);
      } else {
        output.push_back(element);
      }
    }
  }

  /**
   * Word: flatten
   * Prototype: array
   *
   * Takes:
   * - array
   *
   * Gives:
   * - array
   *
   * Creates new array with all sub-array elements concatted into it
   * recursively.
   */
  static void
  w_to_flatten(class context& context)
  {
    std::shared_ptr<value::array> array;

    if (context.pop_array(array))
    {
      std::vector<value::ref> result;

      result.reserve(array->size());
      do_flatten(array, result);
      context << value::new_array(result);
    }
  }

  /**
   * Word: >reverse
   * Prototype: array
   *
   * Takes:
   * - array
   *
   * Gives:
   * - array
   *
   * Reverses the array. The first array element becomes the last and the last
   * array element becomes first.
   */
  static void
  w_to_reverse(class context& context)
  {
    std::shared_ptr<value::array> array;

    if (context.pop_array(array))
    {
      const auto& elements = array->elements();

      context << value::array::container_type(
        std::rbegin(elements),
        std::rend(elements)
      );
    }
  }

  /**
   * Word: >quote
   * Prototype: array
   *
   * Takes:
   * - array
   *
   * Gives:
   * - quote
   *
   * Converts array into executable quote.
   */
  static void
  w_to_quote(class context& context)
  {
    std::shared_ptr<value::array> array;

    if (context.pop_array(array))
    {
      context << value::new_compiled_quote(array->elements());
    }
  }

  /**
   * Word: +
   * Prototype: array
   *
   * Takes:
   * - array
   * - array
   *
   * Gives:
   * - array
   *
   * Concatenates the contents of two arrays and returns the result.
   */
  static void
  w_concat(class context& context)
  {
    std::shared_ptr<value::array> a;
    std::shared_ptr<value::array> b;

    if (context.pop_array(a) && context.pop_array(b))
    {
      value::array::container_type result;

      result.reserve(a->size() + b->size());
      result.insert(std::end(result), std::begin(*a), std::end(*a));
      result.insert(std::end(result), std::begin(*b), std::end(*b));
      context << result;
    }
  }

  /**
   * Word: *
   * Prototype: array
   *
   * Takes:
   * - number
   * - array
   *
   * Gives:
   * - array
   *
   * Repeats the array given number of times.
   */
  static void
  w_repeat(class context& context)
  {
    std::shared_ptr<value::array> array;
    std::shared_ptr<value::number> number;

    if (context.pop_array(array) && context.pop_number(number))
    {
      const auto count = number->int_value();

      if (count > 0)
      {
        value::array::container_type result;

        result.reserve(array->size() * count);
        for (value::number::int_type i = 0; i < count; ++i)
        {
          result.insert(
            std::end(result),
            std::begin(*array),
            std::end(*array)
          );
        }
        context << result;
      }
      else if (count == 0)
      {
        context << value::array::container_type();
      } else {
        context.error(value::error::code::range, U"Invalid repeat count.");
      }
    }
  }

  /**
   * Word: &
   * Prototype: array
   *
   * Takes:
   * - array
   * - array
   *
   * Gives:
   * - array
   *
   * Set intersection: Returns a new array containing unique elements common to
   * the two arrays.
   */
  static void
  w_intersect(class context& context)
  {
    std::shared_ptr<value::array> a;
    std::shared_ptr<value::array> b;

    if (context.pop_array(a) && context.pop_array(b))
    {
      value::array::container_type result;

      for (const auto& value1 : *b)
      {
        bool found = false;

        for (const auto& value2 : *a)
        {
          if (value1->equals(value2))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          continue;
        }

        for (const auto& value2 : result)
        {
          if (value1->equals(value2))
          {
            found = false;
            break;
          }
        }

        if (found)
        {
          result.push_back(value1);
        }
      }

      context << result;
    }
  }

  static void
  w_union(class context& context)
  {
    std::shared_ptr<value::array> a;
    std::shared_ptr<value::array> b;

    if (context.pop_array(a) && context.pop_array(b))
    {
      value::array::container_type result;

      for (const auto& value1 : *b)
      {
        bool found = false;

        for (const auto& value2 : result)
        {
          if (value1->equals(value2))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          result.push_back(value1);
        }
      }

      for (const auto& value1 : *a)
      {
        bool found = false;

        for (const auto& value2 : result)
        {
          if (value1->equals(value2))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          result.push_back(value1);
        }
      }

      context << result;
    }
  }

  extern "C" const runtime::dictionary_definition array =
  {
    // Getters.
    { U"length", w_length },

    // Iteration.
    { U"for-each", w_for_each },
    { U"map", w_map },
    { U"filter", w_filter },
    { U"reduce", w_reduce },

    // Converters.
    { U">flatten", w_to_flatten },
    { U">reverse", w_to_reverse },
    { U">quote", w_to_quote },

    // Operators.
    { U"+", w_concat },
    { U"*", w_repeat },
    { U"&", w_intersect },
    { U"|", w_union }
  };
}
