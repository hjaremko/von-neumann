#include "printing/with_sign.hpp"

#include <iomanip>

namespace vnm::print_policy
{

void with_sign::print_word( std::ostream& os_, const word& rhs )
{
    std::stringstream ss;
    constexpr auto WORD_WIDTH { 6 };

    if ( rhs.is_instruction() )
    {
        if ( rhs.get_code() == vnm::instruction::STOP )
        {
            ss << "STOP";
        }
        else
        {
            ss << std::left << std::setw( WORD_WIDTH ) << instructions_to_str.at( rhs.get_code() )
               << mode_to_str.at( rhs.get_mode() ) << ' ';
            const auto t { s { *rhs.get_arg() } };
            ss << t.val;
        }
    }
    else if ( *rhs != 0 )
    {
        ss << static_cast<int16_t>( *rhs );
    }

    os_ << ss.str();
}

} // namespace vnm::print_policy
