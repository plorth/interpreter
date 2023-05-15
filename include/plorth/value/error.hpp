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

#include <optional>

#include <plorth/value/base.hpp>

namespace plorth::value
{
  class error : public base
  {
  public:
    enum class code
    {
      /** Syntax error. */
      syntax = 1,
      /** Reference error. */
      reference = 2,
      /** Type error. */
      type = 3,
      /** Value error. */
      value = 4,
      /** Range error. */
      range = 5,
      /** Import error. */
      import = 6,
      /** I/O error. */
      io = 7,
      /** Unknown error. */
      unknown = 100
    };

    /**
     * Returns textual description of an error code.
     */
    static std::u32string to_string(enum code code);

    /**
     * Constructs new error instance.
     *
     * \param code     Error code
     * \param message  Textual description of the error
     * \param position Optional position in source code where the error
     *                 occurred.
     */
    explicit error(
      enum code code,
      const std::u32string& message,
      const std::optional<parser::position>& position = std::nullopt
    );

    inline enum type type() const
    {
      return type::error;
    }

    /**
     * Returns the error code associated with this error instance.
     */
    inline enum code code() const
    {
      return m_code;
    }

    /**
     * Returns the error message associated with this error instance.
     */
    inline const std::u32string& message() const
    {
      return m_message;
    }

    /**
     * Returns position in source code where the error occurs, if such
     * information is available.
     */
    inline const std::optional<parser::position>& position() const
    {
      return m_position;
    }

    std::shared_ptr<object> prototype(
      const std::shared_ptr<plorth::runtime>& runtime
    ) const;

    bool equals(const ref& that) const;

    json to_json() const;
    std::u32string to_string() const;
    std::u32string to_source() const;

  private:
    const enum code m_code;
    const std::u32string m_message;
    const std::optional<parser::position> m_position;
  };
}
