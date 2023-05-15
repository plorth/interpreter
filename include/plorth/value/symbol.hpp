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

#include <mutex>
#include <optional>

#include <plorth/value/base.hpp>

namespace plorth::value
{
  /**
   * Symbol represents identifier in Plorth source code.
   */
  class symbol : public base
  {
  public:
    /**
     * Constructs new symbol.
     *
     * \param id       String which acts as identifier for the symbol.
     * \param position Optional position in source code where the symbol was
     *                 encountered.
     */
    explicit symbol(
      const std::u32string& id,
      const std::optional<parser::position>& position = std::nullopt
    );

    inline enum type type() const
    {
      return type::symbol;
    }

    /**
     * Returns string which acts as identifier for the symbol.
     */
    inline const std::u32string& id() const
    {
      return m_id;
    }

    /**
     * Returns position of the symbol in source code, or null pointer if no
     * such information is available.
     */
    inline const std::optional<parser::position>& position() const
    {
      return m_position;
    }

    /**
     * Calculates hash code for the symbol, based on the identifier that
     * represents the symbol.
     */
    std::size_t hash() const;

    std::shared_ptr<object> prototype(
      const std::shared_ptr<plorth::runtime>& runtime
    ) const;

    bool equals(const ref& that) const;

    json to_json() const;
    std::u32string to_string() const;
    std::u32string to_source() const;

  private:
    /** Identifier of the symbol. */
    const std::u32string m_id;
    /** Position of the symbol in source code. */
    const std::optional<parser::position> m_position;
    /** Cached hash code of the symbol. */
    std::size_t m_hash;
    /** Used to implement thread safety in hash calculation. */
    std::mutex m_mutex;
  };
}

namespace std
{
  template<>
  struct hash<shared_ptr<plorth::value::symbol>>
  {
    using argument_type = shared_ptr<plorth::value::symbol>;
    using result_type = std::size_t;

    inline result_type operator()(const argument_type& key) const
    {
      return key ? key->hash() : 0;
    }
  };

  template<>
  struct equal_to<shared_ptr<plorth::value::symbol>>
  {
    using first_argument_type = shared_ptr<plorth::value::symbol>;
    using second_argument_type = first_argument_type;
    using result_type = bool;

    inline result_type operator()(
      const first_argument_type& lhs,
      const second_argument_type& rhs
    ) const
    {
      if (lhs && rhs)
      {
        return lhs->id() == rhs->id();
      } else {
        return !lhs && !rhs;
      }
    }
  };

  template<>
  struct not_equal_to<shared_ptr<plorth::value::symbol>>
  {
    using first_argument_type = shared_ptr<plorth::value::symbol>;
    using second_argument_type = first_argument_type;
    using result_type = bool;

    inline result_type operator()(
      const first_argument_type& lhs,
      const second_argument_type& rhs
    ) const
    {
      if (lhs && rhs)
      {
        return lhs->id() != rhs->id();
      }
      else if (lhs)
      {
        return !!rhs;
      } else {
        return !!lhs;
      }
    }
  };

  template<>
  struct greater<shared_ptr<plorth::value::symbol>>
  {
    using first_argument_type = shared_ptr<plorth::value::symbol>;
    using second_argument_type = first_argument_type;
    using result_type = bool;

    inline result_type operator()(
      const first_argument_type& lhs,
      const second_argument_type& rhs
    ) const
    {
      if (lhs && rhs)
      {
        return lhs->id().compare(rhs->id()) > 0;
      }
      else if (lhs)
      {
        return true;
      } else {
        return false;
      }
    }
  };

  template<>
  struct less<shared_ptr<plorth::value::symbol>>
  {
    using first_argument_type = shared_ptr<plorth::value::symbol>;
    using second_argument_type = first_argument_type;
    using result_type = bool;

    inline result_type operator()(
      const first_argument_type& lhs,
      const second_argument_type& rhs
    ) const
    {
      if (lhs && rhs)
      {
        return lhs->id().compare(rhs->id()) < 0;
      }
      else if (lhs)
      {
        return false;
      } else {
        return true;
      }
    }
  };

  template<>
  struct greater_equal<shared_ptr<plorth::value::symbol>>
  {
    using first_argument_type = shared_ptr<plorth::value::symbol>;
    using second_argument_type = first_argument_type;
    using result_type = bool;

    inline result_type operator()(
      const first_argument_type& lhs,
      const second_argument_type& rhs
    ) const
    {
      if (lhs && rhs)
      {
        return lhs->id().compare(rhs->id()) >= 0;
      }
      else if (lhs)
      {
        return true;
      } else {
        return false;
      }
    }
  };

  template<>
  struct less_equal<shared_ptr<plorth::value::symbol>>
  {
    using first_argument_type = shared_ptr<plorth::value::symbol>;
    using second_argument_type = first_argument_type;
    using result_type = bool;

    inline result_type operator()(
      const first_argument_type& lhs,
      const second_argument_type& rhs
    ) const
    {
      if (lhs && rhs)
      {
        return lhs->id().compare(rhs->id()) <= 0;
      }
      else if (lhs)
      {
        return false;
      } else {
        return true;
      }
    }
  };
}
