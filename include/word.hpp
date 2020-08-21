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
    using type = uint16_t;

    constexpr word() = default;
    constexpr word( type value ) : word_( value ) // NOLINT
    {
    }

    // TODO: mode as char
    word( const std::string& code, const std::string& mode, type arg )
        : word_( arg & 0b0'0000'00'111111111U ), is_instruction_( true )
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

    [[nodiscard]] constexpr auto is_arg_negative() const -> bool
    {
        return static_cast<bool>( word_ & 0b0'0000'00'100000000U );
    }

    [[nodiscard]] constexpr auto is_instruction() const -> bool
    {
        return is_instruction_;
    }

    [[nodiscard]] constexpr auto get() const -> type
    {
        return word_;
    }

    auto operator*() -> type&
    {
        return word_;
    }

    auto operator*() const -> const type&
    {
        return word_;
    }

    [[nodiscard]] constexpr auto get_code() const -> instruction
    {
        return static_cast<instruction>( word_ & 0b0'1111'00'000000000u );
    }

    [[nodiscard]] constexpr auto get_mode() const -> mode
    {
        return static_cast<mode>( word_ & 0b0'0000'11'000000000u );
    }

    [[nodiscard]] constexpr auto get_arg() const -> word
    {
        return word( word_ & 0b0'0000'00'111111111u );
    }

    constexpr auto operator+=( const word& rhs ) -> word&
    {
        word_ += rhs.get();
        return *this;
    }

    constexpr auto operator-=( const word& rhs ) -> word&
    {
        word_ -= rhs.get();
        return *this;
    }

    constexpr auto operator*=( const word& rhs ) -> word&
    {
        word_ *= rhs.get();
        return *this;
    }

    constexpr auto operator/=( const word& rhs ) -> word&
    {
        word_ /= rhs.get();
        return *this;
    }

    constexpr auto operator&=( const word& rhs ) -> word&
    {
        word_ &= rhs.get();
        return *this;
    }

    constexpr auto operator|=( const word& rhs ) -> word&
    {
        word_ |= rhs.get();
        return *this;
    }

    constexpr auto operator!() const -> word
    {
        return word( !get() );
    }

    constexpr auto operator++() -> word&
    {
        ++word_;
        return *this;
    }

    constexpr auto operator++( int ) -> const word
    {
        auto tmp( *this );
        operator++();
        return tmp;
    }

    constexpr auto operator--() -> word&
    {
        --word_;
        return *this;
    }

    constexpr auto operator--( int ) -> const word
    {
        auto tmp( *this );
        operator--();
        return tmp;
    }

private:
    type word_ { 0 };
    bool is_instruction_ { false }; // not sure if necessary
};

const static auto stop { word { "STOP", "$", 0 } };

auto operator>>( std::istream&, vnm::word& ) -> std::istream&;
auto operator==( const vnm::word& lhs, const vnm::word& rhs ) -> bool;

} // namespace vnm

inline auto operator!=( const vnm::word& lhs, const vnm::word& rhs ) -> bool
{
    return !operator==( lhs, rhs );
}

inline auto operator+( vnm::word lhs, const vnm::word& rhs ) -> vnm::word
{
    lhs += rhs;
    return lhs;
}

inline auto operator-( vnm::word lhs, const vnm::word& rhs ) -> vnm::word
{
    lhs -= rhs;
    return lhs;
}

inline auto operator*( vnm::word lhs, const vnm::word& rhs ) -> vnm::word
{
    lhs *= rhs;
    return lhs;
}

inline auto operator/( vnm::word lhs, const vnm::word& rhs ) -> vnm::word
{
    lhs /= rhs;
    return lhs;
}

inline auto operator&( vnm::word lhs, const vnm::word& rhs ) -> vnm::word
{
    lhs &= rhs;
    return lhs;
}

inline auto operator|( vnm::word lhs, const vnm::word& rhs ) -> vnm::word
{
    lhs |= rhs;
    return lhs;
}

#endif
