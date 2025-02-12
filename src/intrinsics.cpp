#include "intrinsics.hpp"

namespace intrinsics
{

    int getLSBIndex(uint64_t bb)
    {
        // __builtin_ctzll returns the number of trailing 0-bits.
        // It is undefined for bb == 0, so make sure bb is non-zero in your tests.
        return __builtin_ctzll(bb);
    }

} // namespace intrinsics
