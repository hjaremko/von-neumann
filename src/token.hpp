#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

struct token
{
    enum class type
    {
        instruction,
        mode,
        number,
        newline
    };

    token( type t_type, std::string t_lexeme, int t_line )
        : type_( t_type ), lexeme( std::move( t_lexeme ) ), line( t_line )
    {
    }

    token( type t_type, int t_value, int t_line )
        : type_( t_type ), value( t_value ), line( t_line )
    {
    }

    type type_;
    std::string lexeme{ ' ' };
    int value{ 0 };
    int line{ 0 };
};

#endif
