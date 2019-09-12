#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "word.hpp"

#include <vector>

namespace vnm
{

class memory
{
public:
    memory();

    [[nodiscard]] word get( const word& ) const;
    void set( const word&, const word& );

private:
    std::vector<word> m_mem;
};

} // namespace vnm

#endif
