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

#include <plorth/value/base.hpp>

namespace plorth::value
{
  base::base() {}

  std::u32string
  to_string(enum type type)
  {
    switch (type)
    {
      case type::array:
        return U"array";

      case type::boolean:
        return U"boolean";

      case type::error:
        return U"error";

      case type::null:
        return U"null";

      case type::number:
        return U"number";

      case type::object:
        return U"object";

      case type::quote:
        return U"quote";

      case type::string:
        return U"string";

      case type::symbol:
        return U"symbol";

      case type::word:
        return U"word";
    }

    return U"unknown";
  }

  std::ostream&
  operator<<(std::ostream& os, enum type type)
  {
    os << peelo::unicode::encoding::utf8::encode(to_string(type));

    return os;
  }

  std::ostream&
  operator<<(std::ostream& os, const base* value)
  {
    if (value)
    {
      os << peelo::unicode::encoding::utf8::encode(value->to_string());
    } else {
      os << "<no value>";
    }

    return os;
  }

  std::ostream&
  operator<<(std::ostream& os, const ref& value)
  {
    if (value)
    {
      os << peelo::unicode::encoding::utf8::encode(value->to_string());
    } else {
      os << "<no value>";
    }

    return os;
  }
}
