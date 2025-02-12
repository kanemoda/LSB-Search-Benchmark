#ifndef ITERATIVE_HPP
#define ITERATIVE_HPP

#include <cstdint>

namespace iterative
{

    // Returns the index of the least-significant set bit in bb using an iterative search.
    // Returns -1 if no bit is set.
    int getLSBIndex(uint64_t bb);

} // namespace iterative

#endif // ITERATIVE_HPP
