#ifndef INTRINSICS_HPP
#define INTRINSICS_HPP

#include <cstdint>

namespace intrinsics
{

    // Returns the index of the least-significant set bit using a compiler intrinsic.
    // This function relies on __builtin_ctzll which counts trailing zeros.
    int getLSBIndex(uint64_t bb);

} // namespace intrinsics

#endif // INTRINSICS_HPP
