#include "interpreter.hpp"

#include <algorithm>

namespace vnm
{

interpreter::interpreter( std::istream& istream ) : input_stream_( istream )
{
    input_stream_.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    error_count_ = 0;
}

void interpreter::error( const std::string& msg, const token& t, const std::string& s )
{
    error_count_++;

    // TODO: extract
    auto source_ss { std::stringstream { s } };
    auto l { std::string {} };

    for ( int i = 0; i < t.line; ++i )
    {
        std::getline( source_ss, l );
    }

    auto token_pos = l.find( t.lexeme );

    if ( token_pos == std::string::npos )
    {
        token_pos = l.length() - 1;
    }

    const auto spaces { std::string( token_pos, ' ' ) };
    const auto tildes { std::string( t.lexeme.length() != 0 ? t.lexeme.length() - 1 : 0U, '~' ) };

    std::cout << t.line << ":" << token_pos + 1 << ": " << msg << std::endl;
    std::cout << "\t" << l << std::endl;
    std::cout << "\t" << spaces << "^" << tildes << std::endl;
}

auto interpreter::interpret() -> machine::mem_t // const
{
    const auto input { std::string { std::istreambuf_iterator<char> { input_stream_ },
                                     std::istreambuf_iterator<char> {} } };
    auto ram { machine::mem_t {} };
    auto s { scanner { input } };
    const auto tokens { s.scan_tokens() };

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
                    error( "Expected argument", *( token + 2 ), input );
                }
            }
            else if ( next_token.type_ == token::type::newline )
            {
                error( "Expected addressing mode", next_token, input );
            }
            else
            {
                error( "Unknown addressing mode", next_token, input );
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
                error( "No newline after argument", next_token, input );
            }
        }
    }

    if ( error_count_ )
    {
        const auto msg { "Execution halted: "s + std::to_string( error_count_ ) + " errors"s };
        throw std::runtime_error( msg );
    }

    return ram;
}

} // namespace vnm
