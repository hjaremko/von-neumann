#ifndef VON_NEUMANN_WORD_HPP
#define VON_NEUMANN_WORD_HPP

#include "instruction_map.hpp"
#include "mode_map.hpp"

#include <cstdint>

namespace vnm
{

class word
{
public:
    using type = uint16_t;

    enum class mask : type
    {
        code = 0b0'1111'00'000000000U,
        mode = 0b0'0000'11'000000000U,
        arg = 0b0'0000'00'111111111U,
        arg_sign = 0b0'0000'00'100000000U,
    };

    constexpr word() = default;
    constexpr word( type value ) : word_( value ) // NOLINT
    {
    }

    word( std::string_view code, std::string_view mode, type arg )
        : word_( arg & static_cast<type>( mask::arg ) ), is_instruction_( true )
    {
        word_ |= static_cast<type>( str_to_instruction( code ) );
        word_ |= static_cast<type>( str_to_mode( mode ) );
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
        return static_cast<bool>( word_ & static_cast<type>( mask::arg_sign ) );
    }

    [[nodiscard]] constexpr auto is_instruction() const -> bool
    {
        return is_instruction_;
    }

    [[nodiscard]] constexpr auto get() const -> type
    {
        return word_;
    }

    [[nodiscard]] constexpr auto get() -> type&
    {
        return word_;
    }

    constexpr auto operator*() -> type&
    {
        return get();
    }

    constexpr auto operator*() const -> type
    {
        return get();
    }

    [[nodiscard]] constexpr auto get_code() const -> instruction
    {
        return static_cast<instruction>( word_ &
                                         static_cast<type>( mask::code ) );
    }

    [[nodiscard]] constexpr auto get_mode() const -> mode
    {
        return static_cast<mode>( word_ & static_cast<type>( mask::mode ) );
    }

    [[nodiscard]] constexpr auto get_arg() const -> word
    {
        return word( word_ & static_cast<type>( mask::arg ) );
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

    constexpr auto operator~() const -> word
    {
        return word( get() ^ 0xFFFF );
    }

private:
    type word_ { 0 };
    bool is_instruction_ { false }; // not sure if necessary
};

inline const auto stop { word { "STOP", "$", 0 } };

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
