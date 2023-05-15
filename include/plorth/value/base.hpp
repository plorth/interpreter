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

#include <memory>

#include <plorth/json.hpp>

namespace plorth
{
  class runtime;
}

namespace plorth::value
{
  class object;

  /**
   * Enumeration of different supported value types.
   */
  enum class type
  {
    /** Array values. */
    array = 0,
    /** Boolean values. */
    boolean = 1,
    /** Errors. */
    error = 2,
    /** Value for null. */
    null = 3,
    /** Number (floating point) values. */
    number = 4,
    /** Other type of objects. */
    object = 5,
    /** Quotes. */
    quote = 6,
    /** String (Unicode) values. */
    string = 7,
    /** Symbols. */
    symbol = 8,
    /** Words. */
    word = 9
  };

  /**
   * Abstract base class for values in Plorth programming language.
   */
  class base
  {
  public:
    explicit base();

    base(const base&) = delete;
    base(base&&) = delete;
    void operator=(const base&) = delete;
    void operator=(base&&) = delete;

    /**
     * Returns type of the value.
     */
    virtual enum type type() const = 0;

    /**
     * Determines prototype object of the value, based on it's type. If the
     * value is an object, property called "__proto__" will be used instead,
     * with the runtime's object prototype acting as a fallback.
     *
     * \param runtime Script runtime to use for prototype retrieval.
     * \return        Prototype object of the value.
     */
    virtual std::shared_ptr<object> prototype(
      const std::shared_ptr<plorth::runtime>& runtime
    ) const = 0;

    /**
     * Tests whether an value is considered to be equal with another value.
     *
     * \param that Reference to other value to compare this one against.
     * \return     A boolean value which tells whether this value is considered
     *             to be equal with the one given as argument
     */
    virtual bool equals(const std::shared_ptr<base>& that) const = 0;

    /**
     * Constructs JSON representation of the value.
     */
    virtual json to_json() const = 0;

    /**
     * Constructs string representation of the value.
     */
    virtual std::u32string to_string() const = 0;

    /**
     * Constructs a string that resembles as accurately as possible what this
     * value would look like in source code.
     */
    virtual std::u32string to_source() const = 0;
  };

  using ref = std::shared_ptr<base>;

  /**
   * Returns textual description of type of an value.
   */
  std::u32string to_string(enum type type);

  std::ostream& operator<<(std::ostream&, type);
  std::ostream& operator<<(std::ostream&, const base*);
  std::ostream& operator<<(std::ostream&, const ref&);

  inline bool
  operator==(const ref& a, const ref& b)
  {
    return a ? a->equals(b) : !b;
  }

  inline bool
  operator!=(const ref& a, const ref& b)
  {
    return a ? !a->equals(b) : !!b;
  }
}
