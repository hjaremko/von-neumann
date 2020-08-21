#include "parsing/scanner.hpp"

#include "instruction_mappings.hpp"

#include <cctype>

namespace vnm
{

scanner::scanner( std::string source, error_reporter& errors )
    : source_( std::move( source ) ), errors_( errors )
{
}

auto scanner::at_end() const -> bool
{
    return current_ >= source_.length();
}

void scanner::scan_token()
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
            errors_.report( "Unexpected character",
                            token { token::type::number,
                                    source_.substr( start_, current_ - start_ ),
                                    static_cast<int>( line_ ) } );
        }

        break;
    }
    }
}

auto scanner::scan_tokens() -> std::vector<token>
{
    while ( !at_end() )
    {
        start_ = current_;
        scan_token();
    }

    return tokens_;
}

auto scanner::advance() -> char
{
    return source_.at( current_++ );
}

void scanner::add_token( token::type token_type )
{
    tokens_.emplace_back( token_type, source_.substr( start_, current_ - start_ ), line_ );
}

void scanner::add_token( token::type token_type, int num )
{
    tokens_.emplace_back( token_type, num, line_ );
}

auto scanner::peek() const -> char
{
    return at_end() ? '\0' : source_.at( current_ );
}

void scanner::number()
{
    while ( std::isdigit( peek() ) )
    {
        advance();
    }

    const auto snum { source_.substr( start_, current_ - start_ ) };
    add_token( token::type::number, std::stoi( snum ) );
}

void scanner::string()
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
        errors_.report( "Invalid instruction",
                        token { token::type::number, str, static_cast<int>( line_ ) } );
    }
}

} // namespace vnm
