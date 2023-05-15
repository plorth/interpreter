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
#include <peelo/unicode/encoding/utf8.hpp>

#include <plorth/context.hpp>

namespace plorth::value
{
  std::u32string
  error::to_string(enum code code)
  {
    switch (code)
    {
      case error::code::syntax:
        return U"Syntax error";

      case error::code::reference:
        return U"Reference error";

      case error::code::type:
        return U"Type error";

      case error::code::value:
        return U"Value error";

      case error::code::range:
        return U"Range error";

      case error::code::import:
        return U"Import error";

      case error::code::io:
        return U"I/O error";

      case error::code::unknown:
        return U"Unknown error";
    }

    return U"Unknown error";
  }

  error::error(
    enum code code,
    const std::u32string& message,
    const std::optional<parser::position>& position
  )
    : m_code(code)
    , m_message(message)
    , m_position(position) {}

  std::shared_ptr<object>
  error::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    return runtime->error_prototype();
  }

  bool
  error::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::error)
    {
      const auto that_error = std::static_pointer_cast<error>(that);

      return m_code == that_error->m_code &&
        !m_message.compare(that_error->m_message);
    }

    return false;
  }

  json
  error::to_json() const
  {
    json result;

    result["code"] = static_cast<int>(m_code);
    result["message"] = peelo::unicode::encoding::utf8::encode(m_message);
    if (m_position)
    {
      result["position"] = plorth::to_json(*m_position);
    }

    return result;
  }

  std::u32string
  error::to_string() const
  {
    std::u32string result;

    result += to_string(m_code);
    if (!m_message.empty())
    {
      result += U": " + m_message;
    }

    return result;
  }

  std::u32string
  error::to_source() const
  {
    return U'<' + to_string() + U'>';
  }
}
