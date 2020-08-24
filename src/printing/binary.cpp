#include "printing/binary.hpp"

#include <bitset>
#include <iomanip>
#include <sstream>

namespace vnm::print_policy
{

void binary::print_word( std::ostream& os_, const word& rhs )
{
    constexpr auto WORD_WIDTH { 6 };
    constexpr auto ARGUMENT_SIZE { 9 };
    constexpr auto WORD_SIZE { 16 };
    std::stringstream ss;

    if ( rhs.is_instruction() )
    {
        if ( rhs.get_code() == vnm::instruction::STOP )
        {
            ss << "STOP";
        }
        else
        {
            ss << std::left << std::setw( WORD_WIDTH )
               << instructions_to_str.at( rhs.get_code() )
               << mode_to_str.at( rhs.get_mode() ) << ' ';
            ss << std::bitset<ARGUMENT_SIZE>( *rhs.get_arg() );
        }
    }
    else if ( *rhs != 0 )
    {
        ss << std::bitset<WORD_SIZE>( *rhs );
    }

    os_ << ss.str();
}

} // namespace vnm::print_policy
