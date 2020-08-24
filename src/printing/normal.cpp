#include "printing/normal.hpp"

#include "word.hpp"

#include <iomanip>
#include <sstream>

namespace vnm::print_policy
{

void normal::print_word( std::ostream& os_, const word& rhs )
{
    constexpr auto WORD_WIDTH { 6 };
    std::stringstream ss;

    if ( rhs.is_instruction() )
    {
        rhs.get_code() == instruction::STOP
            ? ss << "STOP"
            : ss << std::left << std::setw( WORD_WIDTH )
                 << instructions_to_str.at( rhs.get_code() )
                 << mode_to_str.at( rhs.get_mode() ) << ' ' << *rhs.get_arg();
    }
    else if ( *rhs != 0 )
    {
        ss << *rhs;
    }

    os_ << ss.str();
}

} // namespace vnm::print_policy
