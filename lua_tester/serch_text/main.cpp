//
// Created by think on 2018/7/24.
//
#include <stddef.h>
#include "boyer-moore-horspool.h"

int main(int argc, char** argv)
{
    // Full constexpr version. Result will be avaliable during compile time
    {
        constexpr StringView haystack = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, This is a test. Lorem ipsum dolor sit amet, consectetur adipiscing elit,"_sv;
        constexpr Horspool::Needle needle("This is a test"_sv);
        constexpr size_t result = Horspool::Search(needle, haystack);
        printf("position:%d\n" ,result);
        return result;
    }

    // Semi-constexpr version. The needle table will
    // be processed during compile time. The search loop will execute during runtime
    {
        const StringView dynamicHaystack(argv[0]);
        constexpr Horspool::Needle needle("This is a test"_sv);
        size_t result = Horspool::Search(needle, dynamicHaystack);
        return result;
    }

    // Non-constexpr version. Will be executed during runtime
    {
        const StringView dynamicHaystack(argv[0]);
        const Horspool::Needle dynamicNeedle(argv[1]);
        size_t result = Horspool::Search(dynamicNeedle, dynamicHaystack);
        return result;
    }

    return 0;
}
