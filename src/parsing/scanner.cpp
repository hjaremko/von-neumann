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

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded( Ts... ) -> overloaded<Ts...>;

auto scanner::scan_tokens() -> std::vector<std::vector<token>>
{
    while ( !at_end() )
    {
        start_ = current_;

        std::visit(
            overloaded {
                [ this ]( token arg ) { add_token( std::move( arg ) ); },
                [ this ]( error e ) { errors_.report( std::move( e ) ); },
            },
            scan_token() );
    }

    return tokens_;
}

auto scanner::scan_token() -> std::variant<token, error>
{
    auto c { advance() };

    switch ( c )
    {
    case '$':
    case '@':
    case '&':
    case '+':
        //        add_token( token::type::mode );
        return make_token( token::type::mode );
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        return make_token( token::type::newline );
    case ';':
        c = ignore_comment();
        break;
    default:
        break;
    }

    return parse_token( c );
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
    if ( t.type_ == token::type::newline )
    {
        tokens_.emplace_back();
        line_++;
    }

    tokens_.back().push_back( std::move( t ) );
}

// void scanner::add_token( token::type token_type )
//{
//    tokens_.back().push_back( make_token( token_type ) );
//}

auto scanner::make_int_token() -> token
{
    return { token::type::number,
             std::stoi( make_token_string() ),
             static_cast<int>( line_ ) };
}

// void scanner::add_newline_token()
//{
//    //    add_token( token::type::newline );
//    //    tokens_.emplace_back();
//    //    line_++;
//}

auto scanner::ignore_comment() -> char
{
    while ( peek() != '\n' && !at_end() )
    {
        advance();
    }

    return advance();
}

auto scanner::parse_token( char c ) -> std::variant<token, error>
{
    if ( std::isdigit( c ) || c == '-' )
    {
        return parse_number();
    }

    if ( std::isalpha( c ) )
    {
        return parse_string();
    }

    return error { make_token( token::type::number ),
                   "Unexpected character",
                   source_ };
}

auto scanner::peek() const -> char
{
    return at_end() ? '\0' : source_.at( current_ );
}

auto scanner::at_end() const -> bool
{
    return current_ >= source_.length();
}

auto scanner::parse_number() -> token
{
    while ( std::isdigit( peek() ) )
    {
        advance();
    }

    return make_int_token();
}

auto scanner::parse_string() -> std::variant<token, error>
{
    while ( std::isalpha( peek() ) && !std::isspace( peek() ) && !at_end() )
    {
        advance();
    }

    auto string_token = make_token( token::type::instruction );

    if ( str_instructions_map.count( string_token.lexeme ) )
    {
        return string_token;
    }

    return error { string_token, "Invalid instruction", source_ };
}

} // namespace vnm
