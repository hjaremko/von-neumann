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

    token( type t_type, const std::string& t_value, int t_line )
         : type_( t_type ), lexeme( std::move( t_value ) ), line( std::move( t_line ) ) {}

    token( type t_type, int t_value, int t_line )
         : type_( t_type ), value( t_value ), line( std::move( t_line ) ) {}

    type type_;
    std::string lexeme;
    int value{ 0 };
    int line{ 0 };
};

#endif
