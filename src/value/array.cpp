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
  array::array(const std::initializer_list<value_type>& elements)
    : m_elements(elements) {}

  array::array(const container_type& elements)
    : m_elements(elements) {}

  std::shared_ptr<object>
  array::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    return runtime->array_prototype();
  }

  bool
  array::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::array)
    {
      const auto that_array = std::static_pointer_cast<array>(that);
      const auto size_a = m_elements.size();
      const auto size_b = that_array->m_elements.size();

      if (size_a != size_b)
      {
        return false;
      }
      for (size_type i = 0; i < size_a; ++i)
      {
        if (!m_elements[i]->equals(that_array->m_elements[i]))
        {
          return false;
        }
      }

      return true;
    }

    return false;
  }

  json
  array::to_json() const
  {
    std::vector<json> result;

    result.reserve(m_elements.size());
    for (const auto& element : m_elements)
    {
      result.push_back(element->to_json());
    }

    return result;
  }

  std::u32string
  array::to_string() const
  {
    const auto size = m_elements.size();
    std::u32string result;

    for (container_type::size_type i = 0; i < size; ++i)
    {
      const auto& element = m_elements[i];

      if (i > 0)
      {
        result += U", ";
      }
      result += element->to_string();
    }

    return result;
  }

  std::u32string
  array::to_source() const
  {
    const auto size = m_elements.size();
    std::u32string result;

    result += U'[';
    for (container_type::size_type i = 0; i < size; ++i)
    {
      const auto& element = m_elements[i];

      if (i > 0)
      {
        result += U", ";
      }
      result += element->to_source();
    }
    result += U']';

    return result;
  }
}
