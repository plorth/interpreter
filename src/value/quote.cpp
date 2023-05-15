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

namespace plorth::value
{
  quote::quote(const callback_type& callback)
    : m_container(callback) {}

  quote::quote(const std::vector<ref>& values)
    : m_container(values) {}

  bool
  quote::call(class context& context) const
  {
    if (is_native())
    {
      std::get<callback_type>(m_container)(context);
    } else {
      for (const auto& value : std::get<std::vector<ref>>(m_container))
      {
        if (!context.exec(value))
        {
          return false;
        }
      }
    }

    return !context.error();
  }

  std::shared_ptr<object>
  quote::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    return runtime->quote_prototype();
  }

  bool
  quote::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::quote)
    {
      const auto that_quote = std::static_pointer_cast<quote>(that);

      if (is_native())
      {
        if (!that_quote->is_native())
        {
          return false;
        }

        // There is no way to compare two std::function instances against each
        // other, so we have to do an pointer comparison.
        return this == that_quote.get();
      }
      else if (!that_quote->is_compiled())
      {
        return false;
      } else {
        const auto& a = std::get<std::vector<ref>>(m_container);
        const auto& b = std::get<std::vector<ref>>(that_quote->m_container);
        const auto size = a.size();

        if (size != b.size())
        {
          return false;
        }
        for (std::size_t i = 0; i < size; ++i)
        {
          if (!a[i]->equals(b[i]))
          {
            return false;
          }
        }

        return true;
      }
    }

    return false;
  }

  json
  quote::to_json() const
  {
    if (is_compiled())
    {
      const auto& values = std::get<std::vector<ref>>(m_container);
      std::vector<json> result;

      result.reserve(values.size());
      for (const auto& value : values)
      {
        result.push_back(value->to_json());
      }

      return result;
    }

    return "native quote";
  }

  std::u32string
  quote::to_string() const
  {
    return to_source();
  }

  std::u32string
  quote::to_source() const
  {
    if (!is_native())
    {
      const auto& values = std::get<std::vector<ref>>(m_container);
      std::u32string result;
      bool first = true;

      result += U'(';
      for (const auto& value : values)
      {
        if (first)
        {
          first = false;
        } else {
          result += U' ';
        }
        result += value->to_source();
      }
      result += U')';

      return result;
    }

    return U"(\"native quote\")";
  }
}
