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

#include <unordered_map>

#include <plorth/io.hpp>
#include <plorth/value/boolean.hpp>
#include <plorth/value/null.hpp>
#include <plorth/value/quote.hpp>

namespace plorth
{
  class runtime
  {
  public:
    using dictionary_type = std::unordered_map<std::u32string, value::ref>;
    using dictionary_definition = std::initializer_list<
      std::pair<
        std::u32string,
        value::quote::callback_type
      >
    >;

    explicit runtime(
      const std::shared_ptr<io::input>& input = nullptr,
      const std::shared_ptr<io::output>& output = nullptr
    );

    runtime(const runtime&) = delete;
    runtime(runtime&&) = delete;
    void operator=(const runtime&) = delete;
    void operator=(runtime&&) = delete;

    /**
     * Returns the input associated with this runtime, or null pointer if this
     * runtime has no input.
     */
    inline const std::shared_ptr<io::input>& input() const
    {
      return m_input;
    }

    /**
     * Returns the output associated with this runtime, or null pointer if this
     * runtime has no output.
     */
    inline const std::shared_ptr<io::output>& output() const
    {
      return m_output;
    }

    /**
     * Returns the global dictionary that contains core word set available to
     * all contexts.
     *
     * This non-constant version of the method can be used to define new words
     * into the global dictionary, or remove existing ones.
     */
    inline dictionary_type& dictionary()
    {
      return m_dictionary;
    }

    /**
     * Returns the global dictionary that contains core word set available to
     * all contexts.
     */
    inline const dictionary_type& dictionary() const
    {
      return m_dictionary;
    }

    inline const std::shared_ptr<value::null>& null_instance() const
    {
      return m_null;
    }

    inline const std::shared_ptr<value::boolean>& true_instance() const
    {
      return m_true;
    }

    inline const std::shared_ptr<value::boolean>& false_instance() const
    {
      return m_false;
    }

    inline const std::shared_ptr<value::object>& array_prototype() const
    {
      return m_array_prototype;
    }

    inline const std::shared_ptr<value::object>& boolean_prototype() const
    {
      return m_boolean_prototype;
    }

    inline const std::shared_ptr<value::object>& error_prototype() const
    {
      return m_error_prototype;
    }

    inline const std::shared_ptr<value::object>& number_prototype() const
    {
      return m_number_prototype;
    }

    inline const std::shared_ptr<value::object>& object_prototype() const
    {
      return m_object_prototype;
    }

    inline const std::shared_ptr<value::object>& quote_prototype() const
    {
      return m_quote_prototype;
    }

    inline const std::shared_ptr<value::object>& string_prototype() const
    {
      return m_string_prototype;
    }

    inline const std::shared_ptr<value::object>& symbol_prototype() const
    {
      return m_symbol_prototype;
    }

    inline const std::shared_ptr<value::object>& word_prototype() const
    {
      return m_word_prototype;
    }

  private:
    /** Optional input used by the runtime. */
    const std::shared_ptr<io::input> m_input;
    /** Optional output used by the runtime. */
    const std::shared_ptr<io::output> m_output;
    /** Global dictionary available to all contexts. */
    dictionary_type m_dictionary;
    /** Shared null instance. */
    std::shared_ptr<value::null> m_null;
    /** Shared true instance. */
    std::shared_ptr<value::boolean> m_true;
    /** Shared false instance. */
    std::shared_ptr<value::boolean> m_false;
    /** Prototype object for array values. */
    std::shared_ptr<value::object> m_array_prototype;
    /** Prototype object for boolean values. */
    std::shared_ptr<value::object> m_boolean_prototype;
    /** Prototype object for error values. */
    std::shared_ptr<value::object> m_error_prototype;
    /** Prototype object for number values. */
    std::shared_ptr<value::object> m_number_prototype;
    /** Prototype object for object values. */
    std::shared_ptr<value::object> m_object_prototype;
    /** Prototype object for quote values. */
    std::shared_ptr<value::object> m_quote_prototype;
    /** Prototype object for string values. */
    std::shared_ptr<value::object> m_string_prototype;
    /** Prototype object for symbol values. */
    std::shared_ptr<value::object> m_symbol_prototype;
    /** Prototype object for word values. */
    std::shared_ptr<value::object> m_word_prototype;
  };
}
