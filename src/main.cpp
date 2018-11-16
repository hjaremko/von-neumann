#include <iostream>
#include <unordered_map>

namespace vnm
{

enum class instruction
{
    ZERO  = 0b0'0000'00'000000000,
    STOP  = 0b1'0000'00'000000000,
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

std::unordered_map<std::string, instruction> instructions_from_str {
    { "ZERO",  instruction::ZERO  },
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

std::unordered_map<instruction, std::string> instructions_to_str {
    { instruction::ZERO,  "0"  },
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

std::unordered_map<std::string, mode> mode_from_str {
    { "$", mode::instant  },
    { "@", mode::direct   },
    { "&", mode::indirect },
    { "+", mode::index    },
};

std::unordered_map<mode, std::string> mode_to_str {
    { mode::instant,  "$" },
    { mode::direct,   "@" },
    { mode::indirect, "&" },
    { mode::index,    "+" },
};

}

#include "word.hpp"
#include "machine.hpp"
#include "interpreter.hpp"


int main( int argc, char const *argv[] )
{
    vnm::machine pmc;

    // vnm::interpreter i( "example/sum_double.vnm" );
    vnm::interpreter i( "example/array_sum.vnm" );
    i.interpret( pmc );

    pmc.print_memory();

    do
    {
        pmc.get_from_memory();
    } while ( pmc.execute() );

    return 0;
}
