#ifndef DEBRUIJN_HPP
#define DEBRUIJN_HPP

#include <cstdint>

namespace debruijn
{

    // Initialize the De Bruijn lookup table.
    void initDeBruijnTable();

    // Returns the index of the least-significant set bit in bb.
    int getLSBIndex(uint64_t bb);

} // namespace debruijn

#endif // DEBRUIJN_HPP
