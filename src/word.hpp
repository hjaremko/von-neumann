#ifndef WORD_HPP
#define WORD_HPP

#include <iostream>
#include <iomanip>
#include <cstdint>

namespace vnm
{

class word
{
    public:
        word() {}

        word( const int16_t t_word ) : m_word( t_word )
        {
        }

        word( const std::string& t_code, const std::string t_mode, int16_t t_arg )
        {
            m_word = t_arg;
            m_word |= static_cast<int16_t>( instructions_from_str.at( t_code ) );
            m_word |= static_cast<int16_t>( mode_from_str.at( t_mode ) );
        }

        void set( const int16_t t_word )
        {
            m_word = t_word;
        }

        int16_t get() const
        {
            return m_word;
        }

        instruction get_code() const
        {
            return static_cast<instruction>( m_word & 0b1'1111'00'000000000 );
        }

        mode get_mode() const
        {
            return static_cast<mode>( m_word & 0b0'0000'11'000000000 );
        }

        word get_arg() const
        {
            word tmp;
            tmp.set( m_word & 0b0'0000'00'111111111 );
            return tmp;
        }


    private:
        int16_t m_word{ 0 };
};

std::ostream& operator<<( std::ostream& t_stream, const word& t_word )
{
    if ( t_word.get() != 0 )
    {
        if ( t_word.get_code() != instruction::ZERO )
        {
            t_stream << std::left << std::setw( 6 ) 
                     << instructions_to_str.at( t_word.get_code() );
        }

        if ( t_word.get_code() != instruction::STOP )
        {
            if ( t_word.get_code() != instruction::ZERO )
            {
                t_stream << mode_to_str.at( t_word.get_mode() ) << ' ';
            }

            t_stream << t_word.get_arg().get();
        }
    }

    return t_stream;
}

}

#endif
