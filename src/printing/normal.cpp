#include "printing/normal.hpp"

#include "word.hpp"

#include <fmt/format.h>

namespace vnm::format_policy
{

auto normal::format( const word& rhs ) -> std::string
{
    if ( rhs.is_instruction() )
    {
        return rhs.get_code() == instruction::STOP
                   ? "STOP"
                   : fmt::format( "{:<5} {} {}",
                                  instruction_to_str( rhs.get_code() ),
                                  mode_to_str( rhs.get_mode() ),
                                  *rhs.get_arg() );
    }

    if ( *rhs != 0 )
    {
        return fmt::format( "{}", *rhs );
    }

    return "";
}

} // namespace vnm::format_policy
