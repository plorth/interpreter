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
  symbol::symbol(
    const std::u32string& id,
    const std::optional<parser::position>& position
  )
    : m_id(id)
    , m_position(position)
    , m_hash(0) {}

  std::size_t
  symbol::hash() const
  {
    std::size_t h = m_hash;

    if (h == 0)
    {
      auto sym = const_cast<symbol*>(this);
      std::lock_guard<std::mutex> lock(sym->m_mutex);

      h = sym->m_hash = std::hash<std::u32string>()(sym->m_id);
    }

    return h;
  }

  std::shared_ptr<object>
  symbol::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    return runtime->symbol_prototype();
  }

  bool
  symbol::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::symbol)
    {
      const auto that_symbol = std::static_pointer_cast<symbol>(that);

      return !m_id.compare(that_symbol->m_id);
    }

    return false;
  }

  json
  symbol::to_json() const
  {
    json result;

    result["id"] = peelo::unicode::encoding::utf8::encode(m_id);
    if (m_position)
    {
      result["position"] = plorth::to_json(*m_position);
    }

    return result;
  }

  std::u32string
  symbol::to_string() const
  {
    return to_source();
  }

  std::u32string
  symbol::to_source() const
  {
    return m_id;
  }
}
