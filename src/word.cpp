#include "word.hpp"

#include <sstream>

namespace vnm
{
word::word( const std::string& t_code, const std::string& t_mode, int16_t t_arg )
    : m_word( t_arg & 0b0'0000'00'111111111 )
{
    m_word |= static_cast<int16_t>( instructions_from_str.at( t_code ) );
    m_word |= static_cast<int16_t>( mode_from_str.at( t_mode ) );
}

void word::set( const int16_t t_word )
{
    m_word = t_word;
}

int16_t word::get() const
{
    return m_word;
}

instruction word::get_code() const
{
    return static_cast<instruction>( m_word & 0b0'1111'00'000000000 );
}

mode word::get_mode() const
{
    return static_cast<mode>( m_word & 0b0'0000'11'000000000 );
}

word word::get_arg() const
{
    return word( m_word & 0b0'0000'00'111111111 );
}

bool word::is_arg_negative() const
{
    return static_cast<bool>( m_word & 0b0'0000'00'100000000 );
}

bool word::is_instruction() const
{
    return m_instruction;
}

void word::to_instruction()
{
    m_instruction = true;
}

int16_t word::get_complete_arg() const
{
    if ( is_arg_negative() )
    {
        return m_word | 0b1'1111'11'000000000;
    }
    else
    {
        return m_word & 0b0'0000'00'111111111;
    }
}
} // namespace vnm

std::ostream& operator<<( std::ostream& t_stream, const vnm::word& t_word )
{
    std::stringstream ss;

    if ( t_word.is_instruction() )
    {
        if ( t_word.get_code() == vnm::instruction::STOP )
        {
            ss << "STOP";
        }
        else
        {
            ss << std::left << std::setw( 6 )
               << vnm::instructions_to_str.at( t_word.get_code() )
               << vnm::mode_to_str.at( t_word.get_mode() ) << ' '
               << t_word.get_complete_arg();
        }
    }
    else if ( t_word.get() != 0 )
    {
        ss << t_word.get_complete_arg();
    }

    return t_stream << ss.str();
}

std::istream& operator>>( std::istream& t_stream, vnm::word& t_word )
{
    std::string line;
    std::string code;
    std::string mode;
    int16_t args;

    std::getline( t_stream, line );
    std::stringstream ss( line );
    std::getline( ss, line, ';' );
    ss.str( line );

    ss >> code >> mode >> args;

    t_word = vnm::word( code, mode, args );

    return t_stream;
}
