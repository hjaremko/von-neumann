#include "parsing/line_interpreter.hpp"

namespace vnm
{

namespace
{

auto is_number( const token& token ) -> bool
{
    return token.type_ == token::type::number;
}

auto is_stop( const token& token ) -> bool
{
    if ( token.type_ == token::type::instruction )
    {
        return token.lexeme == "STOP";
    }

    return false;
}

} // namespace

line_interpreter::line_interpreter( const std::vector<token>& line,
                                    error_reporter& errors )
    : line_( line ), errors_( errors )
{
}

auto line_interpreter::parse_line() -> std::optional<word>
{
    if ( line_.empty() )
    {
        return std::nullopt;
    }

    if ( is_number( *line_.begin() ) )
    {
        return parse_number();
    }

    return parse_instruction();
}

auto line_interpreter::parse_instruction() -> std::optional<word>
{
    if ( line_.begin()->type_ != token::type::instruction )
    {
        return std::nullopt;
    }

    if ( is_stop( *line_.begin() ) )
    {
        return stop;
    }

    return parse_mode();
}

auto line_interpreter::parse_mode() -> std::optional<word>
{
    const auto& token = line_.at( 1 );

    if ( token.type_ != token::type::mode )
    {
        errors_.report( "Expected addressing mode", token );
        return std::nullopt;
    }

    return parse_argument();
}

auto line_interpreter::parse_argument() -> std::optional<word>
{
    const auto& token = line_.at( 2 );

    if ( token.type_ != token::type::number )
    {
        errors_.report( "Expected argument", token );
        return std::nullopt;
    }

    check_for_newline( line_.at( 3 ) );
    return word( line_.at( 0 ).lexeme, line_.at( 1 ).lexeme, token.value );
}

auto line_interpreter::parse_number() -> std::optional<word>
{
    if ( line_.size() != 1 )
    {
        check_for_newline( line_.at( 1 ) );
    }

    return line_.at( 0 ).value;
}

void line_interpreter::check_for_newline( const token& token )
{
    if ( token.type_ != token::type::newline )
    {
        errors_.report( "No newline after argument", token );
    }
}

} // namespace vnm
