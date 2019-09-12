#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "word.hpp"

#include <vector>

namespace vnm
{

class memory
{
public:
    [[nodiscard]] word get( const word& ) const;
    void set( const word&, const word& );

private:
    std::vector<word> memory_{ 512 };
};

} // namespace vnm

#endif
