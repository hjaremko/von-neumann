#ifndef VON_NEUMANN_INSTRUCTION_MAPPINGS_HPP
#define VON_NEUMANN_INSTRUCTION_MAPPINGS_HPP

#include "instruction.hpp"

#include <array>
#include <string_view>
#include <unordered_map>

namespace vnm
{

inline const std::unordered_map<std::string_view, instruction>
    str_instructions_map {
        { "STOP", instruction::STOP },   { "LOAD", instruction::LOAD },
        { "STORE", instruction::STORE }, { "JUMP", instruction::JUMP },
        { "JNEG", instruction::JNEG },   { "JZERO", instruction::JZERO },
        { "ADD", instruction::ADD },     { "SUB", instruction::SUB },
        { "MULT", instruction::MULT },   { "DIV", instruction::DIV },
        { "AND", instruction::AND },     { "OR", instruction::OR },
        { "NOT", instruction::NOT },     { "CMP", instruction::CMP },
        { "SHZ", instruction::SHZ },     { "SHC", instruction::SHC }
    };

constexpr const std::array<std::string_view, 16> instructions_str_map {
    { "STOP",
      "LOAD",
      "STORE",
      "JUMP",
      "JNEG",
      "JZERO",
      "ADD",
      "SUB",
      "MULT",
      "DIV",
      "AND",
      "OR",
      "NOT",
      "CMP",
      "SHZ",
      "SHC" }
};

constexpr auto instruction_to_str( instruction m ) -> std::string_view
{
    constexpr auto OFFSET = 11U;
    return instructions_str_map.at( static_cast<uint16_t>( m ) >> OFFSET );
}

inline auto str_to_instruction( std::string_view str ) -> instruction
{
    return str_instructions_map.at( str );
}

} // namespace vnm

#endif
