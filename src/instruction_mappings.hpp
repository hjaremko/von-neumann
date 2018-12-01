#ifndef INSTRUCTION_MAPPINGS_HPP
#define INSTRUCTION_MAPPINGS_HPP

#include <unordered_map>

namespace vnm
{

enum class instruction
{
    ZERO  = 0b0'0000'00'000000000,
    STOP  = 0b0'0000'00'000000000,
    LOAD  = 0b0'0001'00'000000000,
    STORE = 0b0'0010'00'000000000,
    JUMP  = 0b0'0011'00'000000000,
    JNEG  = 0b0'0100'00'000000000,
    JZERO = 0b0'0101'00'000000000,
    ADD   = 0b0'0110'00'000000000,
    SUB   = 0b0'0111'00'000000000,
    MULT  = 0b0'1000'00'000000000,
    DIV   = 0b0'1001'00'000000000,
    AND   = 0b0'1010'00'000000000,
    OR    = 0b0'1011'00'000000000,
    NOT   = 0b0'1100'00'000000000,
    CMP   = 0b0'1101'00'000000000,
    SHZ   = 0b0'1110'00'000000000,
    SHC   = 0b0'1111'00'000000000,
};

enum class mode
{
    instant  = 0b0'0000'00'000000000, //$
    direct   = 0b0'0000'01'000000000, //@
    indirect = 0b0'0000'10'000000000, //&
    index    = 0b0'0000'11'000000000  //+
};

extern std::unordered_map<std::string, instruction> instructions_from_str;
extern std::unordered_map<instruction, std::string> instructions_to_str ;
extern std::unordered_map<std::string, mode> mode_from_str;
extern std::unordered_map<mode, std::string> mode_to_str;

}

#endif
