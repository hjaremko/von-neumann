#include "memory.hpp"

namespace vnm
{

memory::memory() : m_mem( 512 )
{
}

word memory::get( const word& t_register ) const
{
    try
    {
        return m_mem.at( t_register.get() );
    }
    catch ( const std::exception& /*e*/ )
    {
        throw std::runtime_error( "Address exceeds device memory!" );
    }
}

void memory::set( const word& t_word, const word& t_register )
{
    try
    {
        m_mem.at( t_register.get() ) = t_word;
    }
    catch ( const std::exception& /*e*/ )
    {
        throw std::runtime_error( "Address exceeds device memory!" );
    }
}

} // namespace vnm
