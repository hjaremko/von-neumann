#include "printing/binary.hpp"

#include <bitset>
#include <fmt/format.h>

namespace vnm::format_policy
{

auto binary::format( const word& rhs ) -> std::string
{
    constexpr auto ARGUMENT_SIZE { 9 };
    constexpr auto WORD_SIZE { 16 };

    if ( rhs.is_instruction() )
    {
        return rhs.get_code() == vnm::instruction::STOP
                   ? "STOP"
                   : fmt::format( "{:<5} {} {}",
                                  instruction_to_str( rhs.get_code() ),
                                  mode_to_str( rhs.get_mode() ),
                                  std::bitset<ARGUMENT_SIZE>( *rhs.get_arg() )
                                      .to_string() );
    }

    if ( *rhs )
    {
        return fmt::format( "{}", std::bitset<WORD_SIZE>( *rhs ).to_string() );
    }

    return "";
}

} // namespace vnm::format_policy
