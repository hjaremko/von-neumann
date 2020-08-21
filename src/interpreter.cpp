#include "interpreter.hpp"

#include "error_reporter.hpp"

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

} // namespace

namespace vnm
{

interpreter::interpreter( std::istream& istream ) : input_stream_( istream )
{
    input_stream_.exceptions( std::ifstream::failbit | std::ifstream::badbit );
}

auto interpreter::interpret() -> machine::mem_t // const
{
    const auto input { read_input() };
    auto errors { error_reporter { input } };
    auto ram { machine::mem_t {} };
    auto scan { scanner { input, errors } };
    const auto tokens { scan.scan_tokens() };

    for ( auto token = std::begin( tokens ); token != std::end( tokens ); ++token )
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
                    errors.report( "Expected argument", *( token + 2 ) );
                }
            }
            else if ( next_token.type_ == token::type::newline )
            {
                errors.report( "Expected addressing mode", next_token );
            }
            else
            {
                errors.report( "Unknown addressing mode", next_token );
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
                errors.report( "No newline after argument", next_token );
            }
        }
    }

    if ( errors.has_errors() )
    {
        errors.print_errors();
        throw std::runtime_error( make_halt_message( errors.count() ) );
    }

    return ram;
}

auto interpreter::read_input() const -> std::string
{
    return { std::istreambuf_iterator<char> { input_stream_ }, std::istreambuf_iterator<char> {} };
}

} // namespace vnm
