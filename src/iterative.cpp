#include "iterative.hpp"

namespace iterative
{

    // Classic iterative search to find the LSB index.
    int getLSBIndex(uint64_t bb)
    {
        int index = 0;
        while (bb)
        {
            if (bb & 1ULL)
                return index;
            bb >>= 1;
            index++;
        }
        return -1; // Return -1 if no bit is set.
    }

} // namespace iterative
