#include "parsing/scanner.hpp"

#include "instruction_map.hpp"

#include <cctype>

namespace vnm
{

scanner::scanner( std::string source, error_reporter& errors )
    : source_( std::move( source ) ), errors_( errors )
{
    tokens_.emplace_back();
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
        add_newline_token();
        break;
    case ';':
        ignore_comment();
        break;
    default:
        parse_token( c );
        break;
    }
}

auto scanner::scan_tokens() -> std::vector<std::vector<token>>
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

auto scanner::make_token_string() const -> std::string
{
    return source_.substr( start_, current_ - start_ );
}

auto scanner::make_token( token::type token_type ) const -> token
{
    return { token_type, make_token_string(), static_cast<int>( line_ ) };
}

void scanner::add_token( token&& t )
{
    tokens_.back().push_back( std::move( t ) );
}

void scanner::add_token( token::type token_type )
{
    tokens_.back().push_back( make_token( token_type ) );
}

void scanner::add_int_token()
{
    tokens_.back().emplace_back(
        token::type::number, std::stoi( make_token_string() ), line_ );
}

void scanner::add_newline_token()
{
    add_token( token::type::newline );
    tokens_.emplace_back();
    line_++;
}

void scanner::ignore_comment()
{
    while ( peek() != '\n' && !at_end() )
    {
        advance();
    }
}

void scanner::parse_token( char c )
{
    if ( std::isdigit( c ) || c == '-' )
    {
        parse_number();
    }
    else if ( std::isalpha( c ) )
    {
        parse_string();
    }
    else
    {
        errors_.report( "Unexpected character",
                        make_token( token::type::number ) );
    }
}

auto scanner::peek() const -> char
{
    return at_end() ? '\0' : source_.at( current_ );
}

void scanner::parse_number()
{
    while ( std::isdigit( peek() ) )
    {
        advance();
    }

    add_int_token();
}

void scanner::parse_string()
{
    while ( std::isalpha( peek() ) && !std::isspace( peek() ) && !at_end() )
    {
        advance();
    }

    auto string_token = make_token( token::type::instruction );

    if ( instructions_from_str.count( string_token.lexeme ) )
    {
        add_token( std::move( string_token ) );
    }
    else
    {
        errors_.report( "Invalid instruction", string_token );
    }
}

} // namespace vnm
