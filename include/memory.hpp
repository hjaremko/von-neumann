#ifndef VON_NEUMANN_MEMORY_HPP
#define VON_NEUMANN_MEMORY_HPP

#include "word.hpp"

#include <array>
#include <fmt/format.h>

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
    [[nodiscard]] auto make_exception_message( const word& addr ) const
        -> std::string
    {
        return fmt::format( "Address {} exceeds device memory!{}",
                            *addr,
                            ( *addr == size ? " Missing STOP perhaps?" : "" ) );
    }

    std::array<word, S> memory_;
};

} // namespace vnm

#endif
