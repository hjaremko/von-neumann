#ifndef VON_NEUMANN_MODE_MAP_HPP
#define VON_NEUMANN_MODE_MAP_HPP

#include "mode.hpp"

#include <array>
#include <string_view>
#include <unordered_map>

namespace vnm
{

inline const std::unordered_map<std::string_view, mode> str_mode_map {
    { "$", mode::instant },
    { "@", mode::direct },
    { "&", mode::indirect },
    { "+", mode::index },
};

constexpr const std::array<std::string_view, 4> mode_str_map {
    { "$", "@", "&", "+" }
};

constexpr auto mode_to_str( mode m ) -> std::string_view
{
    return mode_str_map.at( static_cast<uint16_t>( m ) );
}

inline auto str_to_mode( std::string_view str ) -> mode
{
    return str_mode_map.at( str );
}

} // namespace vnm

#endif // VON_NEUMANN_MODE_MAP_HPP
