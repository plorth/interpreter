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

#include <functional>
#include <variant>

#include <plorth/value/base.hpp>

namespace plorth
{
  class context;
}

namespace plorth::value
{
  /**
   * Quote is Plorth's version of an function. It's an value that contains
   * piece of code that can be executed on demand.
   */
  class quote : public base
  {
  public:
    using callback_type = std::function<void(class context&)>;
    using container_type = std::variant<callback_type, std::vector<ref>>;

    static std::shared_ptr<quote> compile(
      class context& context,
      const std::u32string& source,
      const std::u32string& filename = U"<eval>",
      int line = 1,
      int column = 0
    );

    explicit quote(const callback_type& callback);
    explicit quote(const std::vector<ref>& values);

    inline enum type type() const
    {
      return type::quote;
    }

    inline bool is_native() const
    {
      return std::holds_alternative<callback_type>(m_container);
    }

    inline bool is_compiled() const
    {
      return !is_native();
    }

    /**
     * Invokes the quote.
     *
     * \param context Scripting context to execute the quote in.
     * \return        Boolean flag which tells whether execution of the quote
     *                was performed successfully without errors.
     */
    bool call(class context& context) const;

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
