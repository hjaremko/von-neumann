#include "interpreter.hpp"

#include <cctype>

namespace vnm
{

interpreter::scanner::scanner( std::string source ) : source_( std::move( source ) )
{
}

bool interpreter::scanner::at_end() const
{
    return current_ >= source_.length();
}

void interpreter::scanner::scan_token()
{
    const auto c { advance() };

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
        line_++;
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
                                token{ token::type::number,
                                       source_.substr( start_, current_ - start_ ),
                                       static_cast<int>( line_ ) },
                                source_ );
        }

        break;
    }
    }
}

std::vector<token> interpreter::scanner::scan_tokens()
{
    while ( !at_end() )
    {
        start_ = current_;
        scan_token();
    }

    return tokens_;
}

char interpreter::scanner::advance()
{
    return source_.at( current_++ );
}

void interpreter::scanner::add_token( token::type token_type )
{
    tokens_.emplace_back( token_type, source_.substr( start_, current_ - start_ ), line_ );
}

void interpreter::scanner::add_token( token::type token_type, int num )
{
    tokens_.emplace_back( token_type, num, line_ );
}

char interpreter::scanner::peek() const
{
    return at_end() ? '\0' : source_.at( current_ );
}

void interpreter::scanner::number()
{
    while ( std::isdigit( peek() ) )
    {
        advance();
    }

    const auto snum { source_.substr( start_, current_ - start_ ) };
    add_token( token::type::number, std::stoi( snum ) );
}

void interpreter::scanner::string()
{
    while ( std::isalpha( peek() ) && !std::isspace( peek() ) && !at_end() )
    {
        advance();
    }

    const auto str { source_.substr( start_, current_ - start_ ) };

    if ( instructions_from_str.count( str ) )
    {
        add_token( token::type::instruction );
    }
    else
    {
        interpreter::error(
            "Invalid instruction", token{ token::type::number, str, static_cast<int>( line_ ) }, source_ );
    }
}

} // namespace vnm
