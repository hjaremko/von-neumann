#ifndef INSTRUCTION_MAPPINGS_HPP
#define INSTRUCTION_MAPPINGS_HPP

#include <string>
#include <unordered_map>

namespace vnm
{

enum class instruction : uint16_t
{
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

enum class mode : uint16_t
{
    instant  = 0b0'0000'00'000000000, //$
    direct   = 0b0'0000'01'000000000, //@
    indirect = 0b0'0000'10'000000000, //&
    index    = 0b0'0000'11'000000000  //+
};

inline const std::unordered_map<std::string, instruction> instructions_from_str {
    { "STOP",  instruction::STOP  },
    { "LOAD",  instruction::LOAD  },
    { "STORE", instruction::STORE },
    { "JUMP",  instruction::JUMP  },
    { "JNEG",  instruction::JNEG  },
    { "JZERO", instruction::JZERO },
    { "ADD",   instruction::ADD   },
    { "SUB",   instruction::SUB   },
    { "MULT",  instruction::MULT  },
    { "DIV",   instruction::DIV   },
    { "AND",   instruction::AND   },
    { "OR",    instruction::OR    },
    { "NOT",   instruction::NOT   },
    { "CMP",   instruction::CMP   },
    { "SHZ",   instruction::SHZ   },
    { "SHC",   instruction::SHC   }
};

inline const std::unordered_map<instruction, std::string> instructions_to_str {
    { instruction::STOP,  "STOP"  },
    { instruction::LOAD,  "LOAD"  },
    { instruction::STORE, "STORE" },
    { instruction::JUMP,  "JUMP"  },
    { instruction::JNEG,  "JNEG"  },
    { instruction::JZERO, "JZERO" },
    { instruction::ADD,   "ADD"   },
    { instruction::SUB,   "SUB"   },
    { instruction::MULT,  "MULT"  },
    { instruction::DIV,   "DIV"   },
    { instruction::AND,   "AND"   },
    { instruction::OR,    "OR",   },
    { instruction::NOT,   "NOT"   },
    { instruction::CMP,   "CMP"   },
    { instruction::SHZ,   "SHZ"   },
    { instruction::SHC,   "SHC"   }
};

inline const std::unordered_map<std::string, mode> mode_from_str {
    { "$", mode::instant  },
    { "@", mode::direct   },
    { "&", mode::indirect },
    { "+", mode::index    },
};

inline const std::unordered_map<mode, std::string> mode_to_str {
    { mode::instant,  "$" },
    { mode::direct,   "@" },
    { mode::indirect, "&" },
    { mode::index,    "+" },
};

}

#endif
