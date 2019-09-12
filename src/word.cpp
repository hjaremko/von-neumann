#include "word.hpp"

#include <sstream>

std::ostream& operator<<( std::ostream& os, const vnm::word& rhs )
{
    std::stringstream ss;

    if ( rhs.is_instruction() )
    {
        if ( rhs.get_code() == vnm::instruction::STOP )
        {
            ss << "STOP";
        }
        else
        {
            ss << std::left << std::setw( 6 )
               << vnm::instructions_to_str.at( rhs.get_code() )
               << vnm::mode_to_str.at( rhs.get_mode() ) << ' '
               << rhs.get_complete_arg();
        }
    }
    else if ( rhs.get() != 0 )
    {
        ss << rhs.get_complete_arg();
    }

    return os << ss.str();
}

std::istream& operator>>( std::istream& os, vnm::word& rhs )
{
    auto line { std::string{} };
    auto code { std::string{} };
    auto mode { std::string{} };
    auto args { vnm::word::type{} };

    std::getline( os, line );
    auto ss { std::stringstream( line ) };
    std::getline( ss, line, ';' );
    ss.str( line );

    ss >> code >> mode >> args;

    rhs = vnm::word{ code, mode, args };
    return os;
}
