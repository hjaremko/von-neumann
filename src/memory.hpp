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
            try
            {
                return m_mem.at( t_register.get() );
            }
            catch (...)
            {
                throw std::runtime_error( "Address exceeds device memory!" );
            }
        }

        void set( const word t_word, const word t_register )
        {
            try
            {
                m_mem.at( t_register.get() ) = t_word;
            }
            catch (...)
            {
                throw std::runtime_error( "Address exceeds device memory!" );
            }
        }

    private:
        std::vector<word> m_mem;
};

}

#endif
