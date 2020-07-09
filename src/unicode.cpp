/*
 * Copyright (c) 2017-2018, Rauli Laine
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
#include <plorth/unicode.hpp>

#if defined(__EMSCRIPTEN__)
#include <peelo/unicode/ctype/isvalid.hpp>

namespace plorth
{
  static char32_t utf32le_decode_char(wchar_t);
  static wchar_t utf32le_encode_char(char32_t);

  std::u32string utf32le_decode(const std::wstring& input)
  {
    std::u32string output;

    for (const auto& c : input)
    {
      output.append(1, utf32le_decode_char(c));
    }

    return output;
  }

  std::wstring utf32le_encode(const std::u32string& input)
  {
    std::wstring output;

    for (const auto& c : input)
    {
      if (!peelo::unicode::ctype::isvalid(c))
      {
        continue;
      }

      output.append(1, utf32le_encode_char(c));
    }

    return output;
  }

  static char32_t utf32le_decode_char(wchar_t c)
  {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&c);

    return static_cast<char32_t>(
      ((p[3] * 256 + p[2]) * 256 + p[1]) * 256 + p[0]
    );
  }

  wchar_t utf32le_encode_char(char32_t c)
  {
    unsigned char buffer[4];

    buffer[0] = static_cast<unsigned char>(c & 0xff);
    buffer[1] = static_cast<unsigned char>((c & 0xff00) >> 8);
    buffer[2] = static_cast<unsigned char>((c & 0xff0000) >> 16);
    buffer[3] = static_cast<unsigned char>((c & 0xff000000) >> 24);

    return *reinterpret_cast<wchar_t*>(&buffer);
  }
}
#endif
