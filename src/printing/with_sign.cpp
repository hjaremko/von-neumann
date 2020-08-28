#include "printing/with_sign.hpp"

#include <fmt/format.h>

namespace vnm::format_policy
{

auto with_sign::format( const word& rhs ) -> std::string
{
    if ( rhs.is_instruction() )
    {
        const auto t { s { *rhs.get_arg() } };
        return rhs.get_code() == vnm::instruction::STOP
                   ? "STOP"
                   : fmt::format( "{:<5} {} {}",
                                  instruction_to_str( rhs.get_code() ),
                                  mode_to_str( rhs.get_mode() ),
                                  t.val );
    }

    if ( *rhs != 0 )
    {
        return fmt::format( "{}", static_cast<int16_t>( *rhs ) );
    }

    return "";
}

} // namespace vnm::format_policy
