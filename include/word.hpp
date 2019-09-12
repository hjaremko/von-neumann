#ifndef WORD_HPP
#define WORD_HPP

#include "instruction_mappings.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

namespace vnm
{

class word
{
public:
    using type = int16_t;

    constexpr word() = default;
    constexpr explicit word( type value ) : word_( value )
    {
    }

    constexpr word( const std::string& code, const std::string& mode, type arg )
        : word_( arg & 0b0'0000'00'111111111 ), is_instruction_( true )
    {
        word_ |= static_cast<type>( instructions_from_str.at( code ) );
        word_ |= static_cast<type>( mode_from_str.at( mode ) );
    }

    constexpr void set( const type raw_value )
    {
        word_ = raw_value;
    }

    constexpr void to_instruction()
    {
        is_instruction_ = true;
    }

    [[nodiscard]] constexpr bool is_arg_negative() const
    {
        return static_cast<bool>( word_ & 0b0'0000'00'100000000 );
    }

    [[nodiscard]] constexpr bool is_instruction() const
    {
        return is_instruction_;
    }

    [[nodiscard]] constexpr type get() const
    {
        return word_;
    }

    [[nodiscard]] constexpr type get_complete_arg() const
    {
        if ( is_arg_negative() )
        {
            return word_ | 0b1'1111'11'000000000;
        }

        return word_ & 0b0'0000'00'111111111;
    }

    [[nodiscard]] constexpr instruction get_code() const
    {
        return static_cast<instruction>( word_ & 0b0'1111'00'000000000 );
    }

    [[nodiscard]] constexpr mode get_mode() const
    {
        return static_cast<mode>( word_ & 0b0'0000'11'000000000 );
    }

    [[nodiscard]] constexpr word get_arg() const
    {
        return word( word_ & 0b0'0000'00'111111111 );
    }

    constexpr word& operator+=( const word& rhs )
    {
        word_ += rhs.get();
        return *this;
    }

    constexpr word& operator-=( const word& rhs )
    {
        word_ -= rhs.get();
        return *this;
    }

    constexpr word& operator*=( const word& rhs )
    {
        word_ *= rhs.get();
        return *this;
    }

    constexpr word& operator/=( const word& rhs )
    {
        word_ = get_complete_arg() / rhs.get_complete_arg();
        return *this;
    }

    constexpr word& operator&=( const word& rhs )
    {
        word_ &= rhs.get();
        return *this;
    }

    constexpr word& operator|=( const word& rhs )
    {
        word_ |= rhs.get();
        return *this;
    }

    constexpr word operator!() const
    {
        return word( !get() );
    }

    constexpr word& operator++()
    {
        ++word_;
        return *this;
    }

    constexpr const word operator++( int )
    {
        auto tmp( *this );
        operator++();
        return tmp;
    }

    constexpr word& operator--()
    {
        --word_;
        return *this;
    }

    constexpr const word operator--( int )
    {
        auto tmp( *this );
        operator--();
        return tmp;
    }

private:
    type word_{ 0 };
    bool is_instruction_{ false }; // not sure if necessary
};

} // namespace vnm

std::ostream& operator<<( std::ostream&, const vnm::word& );
std::istream& operator>>( std::istream&, vnm::word& );

inline bool operator==( const vnm::word& lhs, const vnm::word& rhs )
{
    return lhs.get() == rhs.get();
}

inline bool operator!=( const vnm::word& lhs, const vnm::word& rhs )
{
    return !operator==( lhs, rhs );
}

inline vnm::word operator+( vnm::word lhs, const vnm::word& rhs )
{
    lhs += rhs;
    return lhs;
}

inline vnm::word operator-( vnm::word lhs, const vnm::word& rhs )
{
    lhs -= rhs;
    return lhs;
}

inline vnm::word operator*( vnm::word lhs, const vnm::word& rhs )
{
    lhs *= rhs;
    return lhs;
}

inline vnm::word operator/( vnm::word lhs, const vnm::word& rhs )
{
    lhs /= rhs;
    return lhs;
}

inline vnm::word operator&( vnm::word lhs, const vnm::word& rhs )
{
    lhs &= rhs;
    return lhs;
}

inline vnm::word operator|( vnm::word lhs, const vnm::word& rhs )
{
    lhs |= rhs;
    return lhs;
}

#endif
