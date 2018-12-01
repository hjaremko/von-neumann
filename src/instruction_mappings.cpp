#include "instruction_mappings.hpp"

namespace vnm
{

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
