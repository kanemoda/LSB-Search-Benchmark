#include "debruijn.hpp"

namespace
{
    // The magic constant used for the De Bruijn sequence.
    const uint64_t DE_BRUJIN_MAGIC = 0x03f79d71b4cb0a89ULL;
    // Internal lookup table for indices.
    int deBruijnTable[64];
}

namespace debruijn
{

    // Build the lookup table based on the magic constant.
    void initDeBruijnTable()
    {
        for (int i = 0; i < 64; i++)
        {
            // Multiply (1 << i) by the magic number and shift right by 58 bits.
            deBruijnTable[((1ULL << i) * DE_BRUJIN_MAGIC) >> 58] = i;
        }
    }

    // Use the De Bruijn lookup to find the index of the LSB.
    int getLSBIndex(uint64_t bb)
    {
        // Isolate the lowest set bit and use the lookup table.
        return deBruijnTable[((bb & -bb) * DE_BRUJIN_MAGIC) >> 58];
    }

} // namespace debruijn
