#include "interpreter.hpp"

namespace vnm
{
unsigned interpreter::m_error_count{ 0 };

interpreter::interpreter( const std::string& t_filename )
{
    m_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    m_file.open( t_filename.c_str() );
}

interpreter::~interpreter()
{
    m_file.close();
}

void interpreter::error( const std::string& t_msg, const token& t_t, const std::string& t_s )
{
    m_error_count++;

    std::stringstream source_ss{ t_s };
    std::string l;

    for ( int i = 0; i < t_t.line; ++i )
    {
        std::getline( source_ss, l );
    }

    auto token_pos = l.find( t_t.lexeme );

    if ( token_pos == std::string::npos )
    {
        token_pos = l.length() - 1;
    }

    auto spaces = std::string( token_pos, ' ' );
    auto tildes = std::string( t_t.lexeme.length() - 1, '~' );

    std::cout << t_t.line << ":" << token_pos + 1 << ": " << t_msg << std::endl;
    std::cout << "\t" << l << std::endl;
    std::cout << "\t" << spaces << "^" << tildes << std::endl;
}

void interpreter::interpret( machine& t_machine )
{
    std::string input( ( std::istreambuf_iterator<char>( m_file ) ),
                         std::istreambuf_iterator<char>() );
    scanner s( input );
    auto tokens = s.scan_tokens();

    for ( auto token = begin( tokens ); token != end( tokens ); ++token )
    {
        if ( token->type_ == token::type::instruction && token->lexeme != "STOP" )
        {
            if ( ( token + 1 )->type_ == token::type::mode )
            {
                if ( ( token + 2 )->type_ == token::type::number )
                {
                    if ( ( token + 3 )->type_ == token::type::newline )
                    {
                        word wtmp( token->lexeme, ( token + 1 )->lexeme, ( token + 2 )->value );
                        wtmp.to_instruction();
                        t_machine.put_to_memory( wtmp, word( ( token + 1 )->line - 1 ) );
                        token += 2;
                    }
                }
                else
                {
                    error( "Expected argument", *( token + 2 ), input );
                }
            }
            else if ( ( token + 1 )->type_ == token::type::newline )
            {
                error( "Expected addressing mode", *( token + 1 ), input );
            }
            else
            {
                error( "Unknown addressing mode", *( token + 1 ), input );
            }
        }
        else if ( token->type_ == token::type::instruction && token->lexeme == "STOP" )
        {
            word wtmp( static_cast<int16_t>( instructions_from_str[ "STOP" ] ) );
            wtmp.to_instruction();
            t_machine.put_to_memory( wtmp, word( token->line - 1 ) );
        }
        else if ( token->type_ == token::type::number )
        {
            t_machine.put_to_memory( word( token->value ), word( token->line - 1 ) );

            if ( ( token + 1 )->type_ != token::type::newline )
            {
                error( "No newline after argument", *( token + 1 ), input );
            }
        }
    }

    if ( m_error_count )
    {
        auto msg = "Execution halted: "s + std::to_string( m_error_count ) + " errors"s;
        throw std::runtime_error( msg );
    }
}

}
