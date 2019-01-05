#include "interpreter.hpp"

namespace vnm
{

bool interpreter::scanner::at_end() const
{
    return m_current >= m_source.length();
}

void interpreter::scanner::scan_token()
{
    auto c = advance();

    switch ( c )
    {
        case '$':
        case '@':
        case '&':
        case '+':
            add_token( token::type::mode );
            break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            add_token( token::type::newline );
            m_line++;
            break;
        case ';':
            while ( peek() != '\n' && !at_end() )
            {
                advance();
            }

            break;

        default:
        {
            if ( std::isdigit( c ) || c == '-' )
            {
                number();
            }
            else if ( std::isalpha( c ) )
            {
                string();
            }
            else
            {
                interpreter::error( "Unexpected character",
                                    token( token::type::number,
                                    m_source.substr( m_start, m_current - m_start ), m_line ),
                                    m_source );

            }

            break;
        }
    }
}

std::vector<token> interpreter::scanner::scan_tokens()
{
    while ( !at_end() )
    {
        m_start = m_current;
        scan_token();
    }

    return std::move( m_tokens );
}

char interpreter::scanner::advance()
{
    return m_source.at( m_current++ );
}

void interpreter::scanner::add_token( token::type t_type )
{
    m_tokens.emplace_back( t_type, m_source.substr( m_start, m_current - m_start ), m_line );
}

void interpreter::scanner::add_token( token::type t_type, int t_num )
{
    m_tokens.emplace_back( t_type, t_num, m_line );
}

char interpreter::scanner::peek() const
{
    return ( at_end() ? '\0' : m_source.at( m_current ) );
}

void interpreter::scanner::number()
{
    while ( std::isdigit( peek() ) )
    {
        advance();
    }

    auto snum = ( m_source.substr( m_start, m_current - m_start ) );
    add_token( token::type::number, std::stoi( snum ) );
}

void interpreter::scanner::string()
{
    while ( std::isalpha( peek() ) && !std::isspace( peek() ) && !at_end() )
    {
        advance();
    }

    auto str = m_source.substr( m_start, m_current - m_start );

    if ( instructions_from_str.count( str ) )
    {
        add_token( token::type::instruction );
    }
    else
    {
        interpreter::error( "Invalid instruction",
                            token( token::type::number, str, m_line ),
                            m_source );
    }
}

}
