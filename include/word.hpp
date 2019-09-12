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
    word() = default;
    explicit word( int16_t t_word ) : m_word( t_word )
    {
    }
    explicit word( const std::string&, const std::string&, int16_t );

    void set( int16_t );
    void to_instruction();
    [[nodiscard]] bool is_arg_negative() const;
    [[nodiscard]] bool is_instruction() const;
    [[nodiscard]] int16_t get() const;
    [[nodiscard]] int16_t get_complete_arg() const;
    [[nodiscard]] instruction get_code() const;
    [[nodiscard]] mode get_mode() const;
    [[nodiscard]] word get_arg() const;

    word& operator+=( const word& rhs )
    {
        m_word += rhs.get();
        return *this;
    }

    word& operator-=( const word& rhs )
    {
        m_word -= rhs.get();
        return *this;
    }

    word& operator*=( const word& rhs )
    {
        m_word *= rhs.get();
        return *this;
    }

    word& operator/=( const word& rhs )
    {
        m_word = get_complete_arg() / rhs.get_complete_arg();
        return *this;
    }

    word& operator&=( const word& rhs )
    {
        m_word &= rhs.get();
        return *this;
    }

    word& operator|=( const word& rhs )
    {
        m_word |= rhs.get();
        return *this;
    }

    word operator!()
    {
        return word( !get() );
    }

    word& operator++()
    {
        ++m_word;
        return *this;
    }

    word operator++( int )
    {
        word tmp( *this );
        operator++();
        return tmp;
    }

    word& operator--()
    {
        --m_word;
        return *this;
    }

    word operator--( int )
    {
        word tmp( *this );
        operator--();
        return tmp;
    }

private:
    int16_t m_word{ 0 };
    bool m_instruction{ false }; // not sure if necessary
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
