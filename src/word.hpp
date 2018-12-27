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
        word() = default;
        explicit word( const int16_t );
        explicit word( const std::string&, const std::string&, int16_t );

        void set( const int16_t );
        void to_instruction();
        bool is_arg_negative() const;
        bool is_instruction() const;
        int16_t get() const;
        int16_t get_complete_arg() const;
        instruction get_code() const;
        mode get_mode() const;
        word get_arg() const;

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
        bool    m_instruction{ false }; //not sure if necessery
};

}

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
