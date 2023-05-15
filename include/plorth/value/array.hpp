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
  /**
   * An array is an container value that contains an ordered sequence of other
   * values.
   */
  class array : public base
  {
  public:
    using value_type = ref;
    using reference = value_type&;
    using const_reference = const value_type&;
    using container_type = std::vector<value_type>;
    using size_type = container_type::size_type;
    using iterator = container_type::const_iterator;
    using reverse_iterator = container_type::const_reverse_iterator;
    using const_iterator = iterator;
    using const_reverse_iterator = reverse_iterator;

    /**
     * Constructs new array from given elements.
     */
    explicit array(const std::initializer_list<value_type>& elements);

    /**
     * Constructs new array from given elements.
     */
    explicit array(const container_type& elements);

    inline enum type type() const
    {
      return type::array;
    }

    /**
     * Returns the number of elements in the array.
     */
    inline size_type size() const
    {
      return m_elements.size();
    }

    /**
     * Provides access to the underlying container.
     */
    inline const container_type elements() const
    {
      return m_elements;
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
      return std::begin(m_elements);
    }

    inline iterator end() const
    {
      return std::end(m_elements);
    }

    inline reverse_iterator rbegin() const
    {
      return std::rbegin(m_elements);
    }

    inline reverse_iterator rend() const
    {
      return std::rend(m_elements);
    }

  private:
    const container_type m_elements;
  };
}
