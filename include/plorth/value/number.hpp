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

#include <cstdint>
#include <variant>

#include <plorth/value/base.hpp>

namespace plorth::value
{
  class number : public base
  {
  public:
#if defined(PLORTH_ENABLE_32BIT_INT)
    using int_type = std::int32_t;
#else
    using int_type = std::int64_t;
#endif
    using real_type = double;
    using container_type = std::variant<int_type, real_type>;

    static const int_type int_min;
    static const int_type int_max;
    static const real_type real_min;
    static const real_type real_max;

    static bool is_valid(const std::u32string& input);
    static int_type parse_int(const std::u32string& input);
    static real_type parse_real(const std::u32string& input);
    static std::u32string to_string(int_type value);
    static std::u32string to_string(real_type value);

    explicit number(int_type value);
    explicit number(real_type value);
    explicit number(const std::u32string& input);

    inline enum type type() const
    {
      return type::number;
    }

    inline bool is_int() const
    {
      return std::holds_alternative<int_type>(m_container);
    }

    inline bool is_real() const
    {
      return !is_int();
    }

    int_type int_value() const;

    real_type real_value() const;

    std::shared_ptr<object> prototype(
      const std::shared_ptr<plorth::runtime>& runtime
    ) const;

    bool equals(const ref& that) const;

    json to_json() const;
    std::u32string to_string() const;
    std::u32string to_source() const;

  private:
    const container_type m_container;
  };
}
