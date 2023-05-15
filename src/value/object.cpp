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
  object::object(const std::initializer_list<value_type>& properties)
    : m_properties(properties) {}

  object::object(const container_type& properties)
    : m_properties(properties) {}

  bool
  object::property(
    const std::shared_ptr<class runtime>& runtime,
    const key_type& key,
    mapped_type& slot
  ) const
  {
    const auto i = m_properties.find(key);

    if (i == std::end(m_properties))
    {
      const auto proto = prototype(runtime);

      if (proto && this != proto.get())
      {
        return proto->property(runtime, key, slot);
      }

      return false;
    }
    slot = i->second;

    return true;
  }

  std::shared_ptr<object>
  object::prototype(const std::shared_ptr<class runtime>& runtime) const
  {
    const auto i = m_properties.find(U"__proto__");

    if (i != std::end(m_properties))
    {
      if (i->second->type() == type::object)
      {
        return std::static_pointer_cast<object>(i->second);
      }

      return nullptr;
    }

    return runtime->object_prototype();
  }

  bool
  object::equals(const ref& that) const
  {
    if (!that)
    {
      return false;
    }
    else if (this == that.get())
    {
      return true;
    }
    else if (that->type() == type::object)
    {
      const auto that_object = std::static_pointer_cast<object>(that);

      if (m_properties.size() != that_object->m_properties.size())
      {
        return false;
      }
      for (const auto& property : m_properties)
      {
        const auto i = that_object->m_properties.find(property.first);

        if (i == std::end(that_object->m_properties))
        {
          return false;
        }
        else if (!property.second->equals(i->second))
        {
          return false;
        }
      }

      return true;
    }

    return false;
  }

  json
  object::to_json() const
  {
    json result;

    for (const auto& property : m_properties)
    {
      result[peelo::unicode::encoding::utf8::encode(property.first)]
        = property.second->to_json();
    }

    return result;
  }

  std::u32string
  object::to_string() const
  {
    std::u32string result;
    bool first = true;

    for (const auto& property : m_properties)
    {
      if (first)
      {
        first = false;
      } else {
        result += U", ";
      }
      result += property.first;
      result += U'=';
      result += property.second->to_string();
    }

    return result;
  }

  std::u32string
  object::to_source() const
  {
    std::u32string result;
    bool first = true;

    result += U'{';
    for (const auto& property : m_properties)
    {
      if (first)
      {
        first = false;
      } else {
        result += U", ";
      }
      result += string::format_json(property.first);
      result += U": ";
      result += property.second->to_source();
    }
    result += U'}';

    return result;
  }
}
