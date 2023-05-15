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

#include <tsl/ordered_map.h>

#include <plorth/value/base.hpp>

namespace plorth::value
{
  class object : public base
  {
  public:
    using key_type = std::u32string;
    using mapped_type = ref;
    using container_type = tsl::ordered_map<key_type, mapped_type>;
    using value_type = container_type::value_type;
    using iterator = container_type::const_iterator;
    using reverse_iterator = container_type::const_reverse_iterator;
    using const_iterator = iterator;
    using const_reverse_iterator = reverse_iterator;

    explicit object(const std::initializer_list<value_type>& properties);

    explicit object(const container_type& properties);

    inline enum type type() const
    {
      return type::object;
    }

    inline const container_type& properties() const
    {
      return m_properties;
    }

    bool property(
      const std::shared_ptr<class runtime>& runtime,
      const std::u32string& id,
      ref& slot
    ) const;

    std::shared_ptr<object> prototype(
      const std::shared_ptr<plorth::runtime>& runtime
    ) const;

    bool equals(const ref& that) const;

    json to_json() const;
    std::u32string to_string() const;
    std::u32string to_source() const;

    inline iterator begin() const
    {
      return std::begin(m_properties);
    }

    inline iterator end() const
    {
      return std::end(m_properties);
    }

    inline reverse_iterator rbegin() const
    {
      return std::rbegin(m_properties);
    }

    inline reverse_iterator rend() const
    {
      return std::rend(m_properties);
    }

  private:
    const container_type m_properties;
  };
}
