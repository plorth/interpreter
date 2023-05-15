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
#include <peelo/unicode/ctype/iscntrl.hpp>
#include <peelo/unicode/encoding/utf8.hpp>

#include <plorth/context.hpp>

namespace plorth::value
{
  string::value_type
  string::format_json(const value_type& input)
  {
    value_type result;

    result.reserve(input.length() + 2);
    result.append(1, U'"');

    for (const auto& c : input)
    {
      switch (c)
      {
        case 010:
          result.append(1, U'\\');
          result.append(1, U'b');
          break;

        case 011:
          result.append(1, U'\\');
          result.append(1, U't');
          break;

        case 012:
          result.append(1, U'\\');
          result.append(1, U'n');
          break;

        case 014:
          result.append(1, U'\\');
          result.append(1, U'f');
          break;

        case 015:
          result.append(1, U'\\');
          result.append(1, U'r');
          break;

        case '"':
        case '\\':
        case '/':
          result.append(1, U'\\');
          result.append(1, c);
          break;

        default:
          if (peelo::unicode::ctype::iscntrl(c))
          {
            char buffer[7];

            std::snprintf(buffer, 7, "\\u%04x", c);
            for (const char* p = buffer; *p; ++p)
            {
              result.append(1, static_cast<char32_t>(*p));
            }
          } else {
            result.append(1, c);
          }
      }
    }

    result += U'"';

    return result;
  }

  string::string(const_reference value)
    : m_value(value) {}

  std::shared_ptr<object>
  string::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    return runtime->string_prototype();
  }

  bool
  string::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::string)
    {
      const auto that_string = std::static_pointer_cast<string>(that);

      return !m_value.compare(that_string->m_value);
    }

    return false;
  }

  json
  string::to_json() const
  {
    return peelo::unicode::encoding::utf8::encode(m_value);
  }

  std::u32string
  string::to_string() const
  {
    return m_value;
  }

  std::u32string
  string::to_source() const
  {
    return format_json(m_value);
  }
}
