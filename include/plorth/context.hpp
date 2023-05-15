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

#include <deque>

#include <plorth/runtime.hpp>
#include <plorth/value.hpp>

namespace plorth
{
  /**
   * Represents program execution state.
   */
  class context
  {
  public:
    using value_type = value::ref;
    using container_type = std::deque<value_type>;
    using size_type = container_type::size_type;
    using iterator = container_type::reverse_iterator;
    using reverse_iterator = container_type::iterator;
    using const_iterator = container_type::const_reverse_iterator;
    using const_reverse_iterator = container_type::const_iterator;

    explicit context(const std::shared_ptr<class runtime>& runtime);

    context(const context&) = delete;
    context(context&&) = delete;
    void operator=(const context&) = delete;
    void operator=(context&&) = delete;

    /**
     * Returns the runtime associated with this context.
     */
    inline const std::shared_ptr<class runtime>& runtime() const
    {
      return m_runtime;
    }

    /**
     * Returns the number of items contained in the data stack.
     */
    inline size_type size() const
    {
      return m_data.size();
    }

    /**
     * Returns the data stack.
     */
    inline container_type& data()
    {
      return m_data;
    }

    /**
     * Returns the data stack.
     */
    inline const container_type& data() const
    {
      return m_data;
    }

    /**
     * Returns the dictionary used by this context to store words.
     */
    inline runtime::dictionary_type& dictionary()
    {
      return m_dictionary;
    }

    /**
     * Returns the dictionary used by this context to store words.
     */
    inline const runtime::dictionary_type& dictionary() const
    {
      return m_dictionary;
    }

    /**
     * Returns the currently uncaught error in this context or null reference
     * if this context has no error.
     */
    inline std::shared_ptr<value::error>& error()
    {
      return m_error;
    }

    /**
     * Returns the currently uncaught error in this context or null reference
     * if this context has no error.
     */
    inline const std::shared_ptr<value::error>& error() const
    {
      return m_error;
    }

    /**
     * Sets the current uncaught error to one given as argument.
     *
     * \param error Error instance to set as the currently uncaught error.
     */
    inline void error(const std::shared_ptr<value::error>& error)
    {
      m_error = error;
    }

    /**
     * Constructs new error instance with given error code and error message
     * and replaces this execution state's currently uncaught error with it.
     *
     * \param code     Error code
     * \param message  Textual description of the error
     * \param position Optional position in the source code where the error
     *                 occurred
     */
    void error(
      enum value::error::code code,
      const std::u32string& message,
      const std::optional<parser::position>& position = std::nullopt
    );

    /**
     * Returns reference to a structure which has information about current
     * position in source code.
     */
    inline struct parser::position& position()
    {
      return m_position;
    }

    /**
     * Returns reference to a structure which has information about current
     * position in source code.
     */
    inline const struct parser::position& position() const
    {
      return m_position;
    }

    /**
     * Pushes an value on top of the data stack.
     *
     * \param value Value to be pushed as the new top-most value of the data
     *              stack.
     */
    inline void push(const value::ref& value)
    {
      if (value)
      {
        m_data.push_back(value);
      } else {
        m_data.push_back(m_runtime->null_instance());
      }
    }

    /**
     * Pops value from the data stack and discards it. If the stack is empty,
     * range error will be set.
     *
     * \return Boolean flag that tells whether the operation was successfull or
     *         not.
     */
    bool pop();

    /**
     * Pops value of certain type from the data stack and discards it. If the
     * stack is empty, range error will be set. If the top value of the stack
     * is different type than expected, type error will be set.
     *
     * \param type Value type to be expected to be at the top of the stack.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop(enum value::type type);

    /**
     * Pops value from the data stack and places it into given reference slot.
     * If the stack is empty, range error will be set.
     *
     * \param slot Reference where the value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop(value_type& slot);

    /**
     * Pops value of certain type from the data stack and places it into given
     * reference slot. If the stack is empty, range error will be set. If the
     * top value of the stack is different type than expected, type error will
     * be set.
     *
     * \param slot Reference where the value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop(value_type& slot, enum value::type type);

    /**
     * Pops array value from the data stack and places it into given slot. If
     * the stack is empty, range error will be set. If something else than
     * array is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the array value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_array(std::shared_ptr<value::array>& slot);

    /**
     * Pops boolean value from the data stack and places it into given slot. If
     * the stack is empty, range error will be set. If something else than
     * boolean is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the boolean value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_boolean(bool& slot);

    /**
     * Pops error value from the data stack and places it into given slot. If
     * the stack is empty, range error will be set. If something else than
     * error is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the error value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_error(std::shared_ptr<value::error>& slot);

    /**
     * Pops number value from the data stack and places it into given slot. If
     * the stack is empty, range error will be set. If something else than
     * number is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the number value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_number(std::shared_ptr<value::number>& slot);

    /**
     * Pops object value from the data stack and places it into given slot. If
     * the stack is empty, range error will be set. If something else than
     * object is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the object value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_object(std::shared_ptr<value::object>& slot);

    /**
     * Pops quote from the data stack and places it into given slot. If the
     * stack is empty, range error will be set. If something else than quote
     * is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the quote will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_quote(std::shared_ptr<value::quote>& slot);

    /**
     * Pops string value from the data stack and places it into given slot. If
     * the stack is empty, range error will be set. If something else than
     * string is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the string value will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_string(value::string::reference slot);

    /**
     * Pops symbol from the data stack and places it into given slot. If the
     * stack is empty, range error will be set. If something else than symbol
     * is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the symbol will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_symbol(std::shared_ptr<value::symbol>& slot);

    /**
     * Pops word from the data stack and places it into given slot. If the
     * stack is empty, range error will be set. If something else than word
     * is as top-most value of the stack, type error will be set.
     *
     * \param slot Where the word will be placed into.
     * \return     Boolean flag that tells whether the operation was
     *             successfull or not.
     */
    bool pop_word(std::shared_ptr<value::word>& slot);


    bool eval(const value::ref& value, value::ref& slot);
    bool exec(const value::ref& value);

    inline iterator begin()
    {
      return std::rbegin(m_data);
    }

    inline const_iterator begin() const
    {
      return std::rbegin(m_data);
    }

    inline const_iterator cbegin() const
    {
      return std::crbegin(m_data);
    }

    inline iterator end()
    {
      return std::rend(m_data);
    }

    inline const_iterator end() const
    {
      return std::rend(m_data);
    }

    inline const_iterator cend() const
    {
      return std::crend(m_data);
    }

    inline reverse_iterator rbegin()
    {
      return std::begin(m_data);
    }

    inline const_reverse_iterator rbegin() const
    {
      return std::begin(m_data);
    }

    inline const_reverse_iterator crbegin() const
    {
      return std::cbegin(m_data);
    }

    inline reverse_iterator rend()
    {
      return std::end(m_data);
    }

    inline const_reverse_iterator rend() const
    {
      return std::end(m_data);
    }

    inline const_reverse_iterator crend() const
    {
      return std::cend(m_data);
    }

    /**
     * Pushes given value on the data stack.
     */
    inline context&
    operator<<(const value_type& value)
    {
      if (value)
      {
        m_data.push_back(value);
      } else {
        m_data.push_back(m_runtime->null_instance());
      }

      return *this;
    }

    /**
     * Pushes null value on to the data stack.
     */
    inline context&
    operator<<(value::null::value_type)
    {
      push(m_runtime->null_instance());

      return *this;
    }

    /**
     * Pushes an boolean value on to the data stack.
     */
    inline context&
    operator<<(value::boolean::value_type value)
    {
      push(value ? m_runtime->true_instance() : m_runtime->false_instance());

      return *this;
    }

    /**
     * Pushes an integer numeric value on to the data stack.
     */
    inline context&
    operator<<(value::number::int_type value)
    {
      push(value::new_int(value));

      return *this;
    }

    /**
     * Pushes an real numeric value on to the data stack.
     */
    inline context&
    operator<<(value::number::real_type value)
    {
      push(value::new_real(value));

      return *this;
    }

    /**
     * Pushes an string value on to the data stack.
     */
    inline context&
    operator<<(value::string::const_reference& value)
    {
      push(value::new_string(value));

      return *this;
    }

    /**
     * Pushes an array value on to the data stack.
     */
    inline context&
    operator<<(const std::initializer_list<value::ref>& elements)
    {
      push(value::new_array(elements));

      return *this;
    }

    /**
     * Pushes an array value on to the data stack.
     */
    inline context&
    operator<<(const value::array::container_type& elements)
    {
      push(value::new_array(elements));

      return *this;
    }

    /**
     * Pushes an object value on to the data stack.
     */
    inline context&
    operator<<(
      const std::initializer_list<value::object::value_type>& properties
    )
    {
      push(value::new_object(properties));

      return *this;
    }

    /**
     * Pushes an object value on to the data stack.
     */
    inline context&
    operator<<(const value::object::container_type& properties)
    {
      push(value::new_object(properties));

      return *this;
    }

  private:
    /** Runtime associated with this context. */
    const std::shared_ptr<class runtime> m_runtime;
    /** Values contained in this context's data stack. */
    container_type m_data;
    /** Container for words associated with this context. */
    runtime::dictionary_type m_dictionary;
    /** Currently uncaught error in this context. */
    std::shared_ptr<value::error> m_error;
    /** Current position in source code. */
    struct parser::position m_position;
  };
}
