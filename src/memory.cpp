#include "memory.hpp"

namespace vnm
{
word memory::get( const word& register_ ) const
{
    try
    {
        return memory_.at( register_.get() );
    }
    catch ( const std::exception& /*e*/ )
    {
        throw std::runtime_error( "Address exceeds device memory!" );
    }
}

void memory::set( const word& value, const word& register_ )
{
    try
    {
        memory_.at( register_.get() ) = value;
    }
    catch ( const std::exception& /*e*/ )
    {
        throw std::runtime_error( "Address exceeds device memory!" );
    }
}

} // namespace vnm
