#ifndef VON_NEUMANN_TOKEN_HPP
#define VON_NEUMANN_TOKEN_HPP

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

    token( type _type, std::string _lexeme, int _line )
        : type_( _type ), lexeme( std::move( _lexeme ) ), line( _line )
    {
    }

    token( type _type, int _value, int _line )
        : type_( _type ), value( _value ), line( _line )
    {
    }

    type type_;
    std::string lexeme{ ' ' };
    int value{ 0 };
    int line{ 0 };
};

#endif
