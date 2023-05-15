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
#include <plorth/runtime.hpp>
#include <plorth/value.hpp>

namespace plorth
{
  namespace api
  {
    extern "C" const runtime::dictionary_definition array;
    extern "C" const runtime::dictionary_definition boolean;
    extern "C" const runtime::dictionary_definition error;
    extern "C" const runtime::dictionary_definition global;
    extern "C" const runtime::dictionary_definition null;
    extern "C" const runtime::dictionary_definition number;
    extern "C" const runtime::dictionary_definition object;
    extern "C" const runtime::dictionary_definition quote;
    extern "C" const runtime::dictionary_definition string;
    extern "C" const runtime::dictionary_definition symbol;
    extern "C" const runtime::dictionary_definition word;
  }

  static std::shared_ptr<value::object> make_prototype(
    runtime*,
    const char32_t*,
    const runtime::dictionary_definition&
  );

  runtime::runtime(
    const std::shared_ptr<io::input>& input,
    const std::shared_ptr<io::output>& output
  )
    : m_input(input)
    , m_output(output)
    , m_null(std::make_shared<value::null>())
    , m_true(std::make_shared<value::boolean>(true))
    , m_false(std::make_shared<value::boolean>(false))
  {
    for (const auto& entry : api::global)
    {
      m_dictionary[entry.first] = value::new_native_quote(entry.second);
    }

    m_object_prototype = make_prototype(
      this,
      U"object",
      api::object
    );
    m_array_prototype = make_prototype(
      this,
      U"array",
      api::array
    );
    m_boolean_prototype = make_prototype(
      this,
      U"boolean",
      api::boolean
    );
    m_error_prototype = make_prototype(
      this,
      U"error",
      api::error
    );
    m_number_prototype = make_prototype(
      this,
      U"number",
      api::number
    );
    m_quote_prototype = make_prototype(
      this,
      U"quote",
      api::quote
    );
    m_string_prototype = make_prototype(
      this,
      U"string",
      api::string
    );
    m_symbol_prototype = make_prototype(
      this,
      U"symbol",
      api::symbol
    );
    m_word_prototype = make_prototype(
      this,
      U"word",
      api::word
    );
  }

  static std::shared_ptr<value::object>
  make_prototype(
    class runtime* runtime,
    const char32_t* name,
    const runtime::dictionary_definition& definition
  )
  {
    value::object::container_type properties;
    std::shared_ptr<value::object> prototype;

    for (const auto& entry : definition)
    {
      properties[entry.first] = value::new_native_quote(entry.second);
    }
    properties[U"__proto__"] = runtime->null_instance();
    prototype = value::new_object(properties);

    // Define prototype into global dictionary as constant if name has been
    // given.
    if (name)
    {
      value::ref prototype_of_prototype = runtime->object_prototype();

      if (!prototype_of_prototype)
      {
        prototype_of_prototype = runtime->null_instance();
      }
      runtime->dictionary()[name] = value::new_compiled_quote({
        value::new_object({
          { U"__proto__", prototype_of_prototype },
          { U"prototype", prototype }
        })
      });
    }

    return prototype;
  }
}
