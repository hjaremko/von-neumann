#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>

#include "word.hpp"

namespace vnm
{

class memory
{
    public:
        memory() : m_mem( 512 ) {}

        word get( const word t_register ) const
        {
            return m_mem.at( t_register.get() );
        }

        void set( const word t_word, const word t_register )
        {
            m_mem.at( t_register.get() ) = t_word;
        }

    private:
        std::vector<word> m_mem;
};

}

#endif
