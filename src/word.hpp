#ifndef WORD_HPP
#define WORD_HPP

#include <iostream>
#include <iomanip>
#include <cstdint>

#include "instruction_mappings.hpp"

namespace vnm
{

class word
{
    public:
        word();
        word( const int16_t );
        word( const std::string&, const std::string&, int16_t );

        void set( const int16_t );
        void to_instruction();
        bool is_arg_negative() const;
        bool is_instruction() const;
        int16_t get() const;
        int16_t get_complete_arg() const;
        instruction get_code() const;
        mode get_mode() const;
        word get_arg() const;

    private:
        int16_t m_word{ 0 };
        bool    m_instruction{ false }; //not sure if necessery
};

}

std::ostream& operator<<( std::ostream& t_stream, const vnm::word& t_word );
std::istream& operator>>( std::istream& t_stream, vnm::word& t_word );

#endif
