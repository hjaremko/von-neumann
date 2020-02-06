#include "memory.hpp"

#include <sstream>

namespace vnm
{

word memory::at( const word& register_ ) const
{
    try
    {
        return memory_.at( *register_ );
    }
    catch ( const std::exception& /*e*/ )
    {
        std::stringstream ss;
        ss << "Address " << *register_ << " exceeds device memory!";

        if ( *register_ == 512 )
            ss << " Missing STOP perhaps?";

        throw std::runtime_error( ss.str() );
    }
}

void memory::set( const word& value, const word& register_ )
{
    try
    {
        memory_.at( *register_ ) = value;
    }
    catch ( const std::exception& /*e*/ )
    {
        std::stringstream ss;
        ss << "Address " << *register_ << " exceeds device memory!";
        throw std::runtime_error( ss.str() );
    }
}

} // namespace vnm
