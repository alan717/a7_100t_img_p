//
// Created by think on 2018/7/24.
//

/*
 * Copyright 2017 Jesper Meyer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <climits>

#if defined(__clang__)
#include <experimental/string_view>
using StringView = std::experimental::string_view;

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#elif defined(__GNUC__) || defined(__GNUG__)
#include <string_view>
using StringView = std::string_view;

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#elif defined(_MSC_VER)
#include <string_view>
using StringView = std::string_view;

#define likely(x)   (x)
#define unlikely(x) (x)
#endif

// sv operator not implemented in clang/msvc at the time writing
constexpr StringView operator"" _sv(const char* str, size_t len) noexcept
{
    return { str, len };
}

namespace Horspool
{
    class Needle
    {
    private:
        size_t table[UCHAR_MAX + 1];
        const StringView str;
    public:
        explicit constexpr Needle(const StringView needle) noexcept : table{ 0 }, str(needle)
        {
            const auto size = needle.size();
            for (int i = 0; i < UCHAR_MAX + 1; ++i)
                table[i] = size;

            if (likely(size >= 1))
            {
                const size_t needle_length_minus_1 = size - 1;
                for (auto i = 0; i < needle_length_minus_1; ++i)
                    table[needle[i]] = needle_length_minus_1 - i;
            }
        }

        constexpr StringView toString() const noexcept
        {
            return str;
        }

        constexpr size_t lookup(size_t i) const noexcept
        {
            return table[i];
        }
    };

    constexpr inline size_t Search(const Needle needle, StringView haystack)
    {
        const StringView needleStr = needle.toString();
        const size_t haystackLen = haystack.size();
        const size_t needleLen = needleStr.size();
        if (unlikely(needleLen > haystackLen))
            return haystackLen;

        if (unlikely(needleLen == 1))
        {
            const auto pos = (const char*)memchr(haystack.data(), needleStr[0], haystackLen);
            return pos != nullptr ? size_t(pos - haystack.data()) : haystackLen;
        }

        const size_t needleLastPos = needleLen - 1;
        const size_t needleLastChr = needle.lookup(needleLastPos);
        size_t haystackPos = 0;
        while (haystackPos <= haystackLen - needleLen)
        {
            const char hayChar = haystack[haystackPos + needleLastPos];
            if (needleLastChr == hayChar
                && memcmp(needleStr.data(), haystack.data() + haystackPos, needleLastChr) == 0)
            {
                return haystackPos;
            }

            haystackPos += needle.lookup(hayChar);
        }
        return haystackLen;
    }
};