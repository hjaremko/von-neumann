#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>

#include "word.hpp"

namespace vnm
{

class memory
{
    public:
        memory();

        const word& get( const word& ) const;
        void set( const word&, const word& );

    private:
        std::vector<word> m_mem;
};

}

#endif
