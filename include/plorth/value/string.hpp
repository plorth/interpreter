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
#pragma once

#include <plorth/value/base.hpp>

namespace plorth::value
{
  class string : public base
  {
  public:
    using value_type = std::u32string;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = value_type::size_type;
    using iterator = value_type::const_iterator;
    using reverse_iterator = value_type::const_reverse_iterator;
    using const_iterator = iterator;
    using const_reverse_iterator = reverse_iterator;

    static std::u32string format_json(const std::u32string& input);

    explicit string(const_reference value);

    inline enum type type() const
    {
      return type::string;
    }

    inline const_reference value() const
    {
      return m_value;
    }

    std::shared_ptr<object> prototype(
      const std::shared_ptr<plorth::runtime>& runtime
    ) const;

    bool equals(const ref& that) const;

    json to_json() const;
    std::u32string to_string() const;
    std::u32string to_source() const;

    inline iterator begin() const
    {
      return std::begin(m_value);
    }

    inline iterator end() const
    {
      return std::end(m_value);
    }

    inline reverse_iterator rbegin() const
    {
      return std::rbegin(m_value);
    }

    inline reverse_iterator rend() const
    {
      return std::rend(m_value);
    }

  private:
    const value_type m_value;
  };
}
