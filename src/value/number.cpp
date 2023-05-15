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
#include <cfloat>
#include <climits>
#include <cmath>

#include <peelo/unicode/encoding/utf8.hpp>

#include <plorth/context.hpp>

namespace plorth::value
{
  using uint_type = std::uint64_t;

  static const char digitmap[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  static const std::u32string string_nan = U"nan";
  static const std::u32string string_inf = U"inf";
  static const std::u32string string_inf_neg = U"-inf";

#if defined(PLORTH_ENABLE_32BIT_INT)
  const number::int_type number::int_min = INT32_MIN;
  const number::int_type number::int_max = INT32_MAX;
#else
  const number::int_type number::int_min = INT64_MIN;
  const number::int_type number::int_max = INT64_MAX;
#endif
  const number::real_type number::real_min = DBL_MIN;
  const number::real_type number::real_max = DBL_MAX;

  static bool string_contains_real(const std::u32string&);

  bool
  number::is_valid(const std::u32string& input)
  {
    const auto length = input.length();
    std::u32string::size_type start;
    bool seen_dot = false;
    bool seen_exponent = false;

    if (!length)
    {
      return false;
    }

    if (input[0] == '+' || input[0] == '-')
    {
      start = 1;
      if (length < 2)
      {
        return false;
      }
    } else {
      start = 0;
    }

    for (auto i = start; i < length; ++i)
    {
      const auto c = input[i];

      if (c == '.')
      {
        if (seen_dot || seen_exponent || i == start || i + 1 > length)
        {
          return false;
        }
        seen_dot = true;
      }
      else if (c == 'e' || c == 'E')
      {
        if (seen_exponent || i == start || i + 2 > length)
        {
          return false;
        }
        if (input[i + 1] == '+' || input[i + 1] == '-')
        {
          if (i + 3 > length)
          {
            return false;
          }
          ++i;
        }
        seen_exponent = true;
      }
      else if (!std::isdigit(c))
      {
        return false;
      }
    }

    return true;
  }

  number::int_type
  number::parse_int(const std::u32string& input)
  {
    static const int_type div = int_max / 10;
    static const int_type rem = int_max % 10;
    int_type number = 0;
    const auto length = input.length();
    std::u32string::size_type offset;
    bool sign;

    // Extract the sign.
    if (length > 0 && (input[0] == '+' || input[0] == '-'))
    {
      offset = 1;
      sign = input[0] == '+';
    } else {
      offset = 0;
      sign = true;
    }

    for (; offset < length; ++offset)
    {
      const auto c = input[offset];
      int digit;

      if (!std::isdigit(c))
      {
        continue;
      }
      digit = c - '0';
      if (number > div || (number == div && digit > rem))
      {
        return 0; // Integer underflow / overflow.
      }
      number = (number * 10) + digit;
    }

    return sign ? number : -number;
  }

  number::real_type
  number::parse_real(const std::u32string& input)
  {
    const auto length = input.length();
    number::real_type number;
    std::u32string::size_type offset;
    bool seen_digits = false;
    bool seen_dot = false;
    bool sign;
    int_type exponent = 0;

    if (!length)
    {
      return false;
    }

    if (!input.compare(string_nan))
    {
      return NAN;
    }
    else if (!input.compare(string_inf))
    {
      return INFINITY;
    }
    else if (!input.compare(string_inf_neg))
    {
      return -INFINITY;
    }

    if (input[0] == '+' || input[0] == '-')
    {
      sign = input[0] == '+';
      offset = 1;
    } else {
      sign = true;
      offset = 0;
    }

    number = 0.0;

    for (; offset < length; ++offset)
    {
      const auto c = input[offset];

      if (std::isdigit(c))
      {
        seen_digits = true;
        if (number > real_max * 0.1)
        {
          ++exponent;
        } else {
          number = (number * 10.0) + (c - '0');
        }
        if (seen_dot)
        {
          --exponent;
        }
      }
      else if (!seen_dot && c == '.')
      {
        seen_dot = true;
      } else {
        break;
      }
    }

    if (!seen_digits)
    {
      return 0.0;
    }

    // Parse exponent.
    if (offset < length && (input[offset] == 'e' || input[offset] == 'E'))
    {
      exponent += parse_int(input.substr(offset + 1));
    }

    if (number == 0.0)
    {
      return 0.0;
    }

    number *= std::pow(10.0, static_cast<real_type>(exponent));
    if (!sign)
    {
      number = -number;
    }

    return number;
  }

  std::u32string
  number::to_string(int_type value)
  {
    const bool negative = value < 0;
    uint_type mag = static_cast<uint_type>(negative ? -value : value);
    std::u32string result;

    if (mag != 0)
    {
      result.reserve(negative ? 21 : 20);
      do
      {
        result.insert(result.begin(), digitmap[mag % 10]);
        mag /= 10;
      }
      while (mag);
    } else {
      result.insert(result.begin(), '0');
    }
    if (negative)
    {
      result.insert(result.begin(), '-');
    }

    return result;
  }

  std::u32string
  number::to_string(real_type value)
  {
    char buffer[20];

    if (std::isnan(value))
    {
      return string_nan;
    }
    else if (std::isinf(value))
    {
      return value < 0.0 ? string_inf_neg : string_inf;
    }
    std::snprintf(buffer, sizeof(buffer), "%g", value);

    return peelo::unicode::encoding::utf8::decode(buffer);
  }

  number::number(int_type value)
    : m_container(value) {}

  number::number(real_type value)
    : m_container(value) {}

  number::number(const std::u32string& input)
    : m_container(
      string_contains_real(input)
        ? parse_real(input)
        : parse_int(input)
    ) {}

  number::int_type
  number::int_value() const
  {
    if (!is_int())
    {
      auto value = std::get<real_type>(m_container);

      if (value > 0.0)
      {
        value = std::floor(value);
      }
      if (value < 0.0)
      {
        value = std::ceil(value);
      }

      return static_cast<int_type>(value);
    }

    return std::get<int_type>(m_container);
  }

  number::real_type
  number::real_value() const
  {
    if (is_int())
    {
      return static_cast<real_type>(std::get<int_type>(m_container));
    } else {
      return std::get<real_type>(m_container);
    }
  }

  std::shared_ptr<object>
  number::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    return runtime->number_prototype();
  }

  bool
  number::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::number)
    {
      const auto that_number = std::static_pointer_cast<number>(that);

      if (is_real() || that_number->is_real())
      {
        return real_value() == that_number->real_value();
      } else {
        return int_value() == that_number->int_value();
      }
    }

    return false;
  }

  json
  number::to_json() const
  {
    if (is_int())
    {
      return std::get<int_type>(m_container);
    } else {
      return std::get<real_type>(m_container);
    }
  }

  std::u32string
  number::to_string() const
  {
    return to_source();
  }

  std::u32string
  number::to_source() const
  {
    if (is_int())
    {
      return to_string(std::get<int_type>(m_container));
    } else {
      return to_string(std::get<real_type>(m_container));
    }
  }

  static bool
  string_contains_real(const std::u32string& input)
  {
    const auto dot_index = input.find(U'.');
    const auto exponent_index_lower_case = input.find(U'e');
    const auto exponent_index_upper_case = input.find(U'E');

    if (
      dot_index == std::u32string::npos &&
      exponent_index_lower_case == std::u32string::npos &&
      exponent_index_upper_case == std::u32string::npos
    )
    {
      return false;
    }

    return true;
  }
}
