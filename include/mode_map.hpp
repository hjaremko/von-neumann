#ifndef VON_NEUMANN_MODE_MAP_HPP
#define VON_NEUMANN_MODE_MAP_HPP

#include "mode.hpp"

#include <string>
#include <unordered_map>

namespace vnm
{

inline const std::unordered_map<std::string, mode> mode_from_str {
    { "$", mode::instant },
    { "@", mode::direct },
    { "&", mode::indirect },
    { "+", mode::index },
};

inline const std::unordered_map<mode, std::string> mode_to_str {
    { mode::instant, "$" },
    { mode::direct, "@" },
    { mode::indirect, "&" },
    { mode::index, "+" },
};

} // namespace vnm

#endif // VON_NEUMANN_MODE_MAP_HPP
