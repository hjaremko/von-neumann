#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "word.hpp"

#include <array>
#include <sstream>

namespace vnm
{

template <unsigned int S>
class memory
{
public:
    [[nodiscard]] auto at( const word& register_ ) const -> word
    {
        try
        {
            return memory_.at( *register_ );
        }
        catch ( const std::exception& /*e*/ )
        {
            std::stringstream ss;
            ss << "Address " << *register_ << " exceeds device memory!";

            if ( *register_ == size )
            {
                ss << " Missing STOP perhaps?";
            }

            throw std::runtime_error( ss.str() );
        }
    }

    // TODO: operator[] or merge set with at
    void set( const word& value, const word& register_ )
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

    static constexpr auto size { S };

private:
    std::array<word, S> memory_;
};

} // namespace vnm

#endif
