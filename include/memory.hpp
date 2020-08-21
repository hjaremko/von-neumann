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
    [[nodiscard]] auto operator[]( const word& addr ) const -> word
    {
        try
        {
            return memory_.at( *addr );
        }
        catch ( const std::exception& /*e*/ )
        {
            throw std::out_of_range( make_exception_message( addr ) );
        }
    }

    [[nodiscard]] auto operator[]( const word& addr ) -> word&
    {
        try
        {
            return memory_.at( *addr );
        }
        catch ( const std::exception& /*e*/ )
        {
            throw std::out_of_range( make_exception_message( addr ) );
        }
    }

    static constexpr auto size { S };

private:
    [[nodiscard]] auto make_exception_message( const word& addr ) const -> std::string
    {
        std::stringstream ss;

        ss << "Address " << *addr << " exceeds device memory!"
           << ( *addr == size ? " Missing STOP perhaps?" : "" );

        return ss.str();
    }

    std::array<word, S> memory_;
};

} // namespace vnm

#endif
