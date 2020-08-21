#include "parsing/interpreter.hpp"

#include "parsing/error_reporter.hpp"
#include "parsing/scanner.hpp"

#include <algorithm>
#include <fstream>

namespace
{

auto make_halt_message( unsigned int error_count ) -> std::string
{
    auto ss { std::stringstream {} };
    ss << "Execution halted: " << error_count << " errors";
    return ss.str();
}

auto read_input( std::istream& istream ) -> std::string
{
    istream.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    return { std::istreambuf_iterator<char> { istream }, std::istreambuf_iterator<char> {} };
}

} // namespace

namespace vnm
{

interpreter::interpreter( std::istream& istream )
{
    auto input { read_input( istream ) };
    errors_ = error_reporter( input );
    tokens_ = scanner( input, errors_ ).scan_tokens();
    check_for_errors();
}

auto interpreter::interpret() -> machine::mem_t
{
    auto ram { machine::mem_t {} };

    for ( auto token = std::begin( tokens_ ); token != std::end( tokens_ ); ++token )
    {
        if ( token->type_ == token::type::instruction && token->lexeme != "STOP" )
        {
            const auto& next_token { *( token + 1 ) };
            if ( next_token.type_ == token::type::mode )
            {
                if ( ( token + 2 )->type_ == token::type::number )
                {
                    if ( ( token + 3 )->type_ == token::type::newline )
                    {
                        ram[ next_token.line - 1 ] =
                            word( token->lexeme, next_token.lexeme, ( token + 2 )->value );
                        token += 2;
                    }
                }
                else
                {
                    errors_.report( "Expected argument", *( token + 2 ) );
                }
            }
            else if ( next_token.type_ == token::type::newline )
            {
                errors_.report( "Expected addressing mode", next_token );
            }
            else
            {
                errors_.report( "Unknown addressing mode", next_token );
            }
        }
        else if ( token->type_ == token::type::instruction && token->lexeme == "STOP" )
        {
            ram[ token->line - 1 ] = stop;
        }
        else if ( token->type_ == token::type::number )
        {
            ram[ token->line - 1 ] = token->value;
            const auto& next_token { *( token + 1 ) };

            if ( next_token.type_ != token::type::newline )
            {
                errors_.report( "No newline after argument", next_token );
            }
        }
    }

    check_for_errors();
    return ram;
}

void interpreter::check_for_errors()
{
    if ( errors_.has_errors() )
    {
        errors_.print_errors();
        errors_.clear();
        throw std::runtime_error( make_halt_message( errors_.count() ) );
    }
}

} // namespace vnm
