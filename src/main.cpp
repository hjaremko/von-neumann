#include <iostream>
#include <fstream>
#include <unordered_map>

#include "cxxopts.hpp"

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

cxxopts::ParseResult parse_command_line( int argc, char* argv[] )
{
    static cxxopts::Options options( argv[ 0 ], "Von Neumann Machine simulator" );

    options.positional_help("[optional args]").show_positional_help();

    options.add_options()
        ( "h,help", "Show help" )
        ( "f,file", "Path to the VNM program file", cxxopts::value<std::string>() )
        ( "s,save", "Save output to file" )
        ( "r,register", "Print register values before every cycle" )
        ( "m,memory", "Print memory before every cycle" );

    auto result = options.parse( argc, argv );

    if ( result.count( "help" ) || result.arguments().size() == 0 )
    {
        std::cout << options.help( { "", "Group" } ) << std::endl;
        exit( 0 );
    }

    return result;
}

int main( int argc, char *argv[] )
{
    vnm::machine pmc;
    std::fstream* out_stream;

    try
    {
        auto parse_result = parse_command_line( argc, argv );
        std::string file_name;

        if ( parse_result.count( "file" ) )
        {
            file_name = parse_result[ "file" ].as<std::string>();
        }
        else
        {
            throw std::logic_error( "No input file!" );
        }

        vnm::interpreter i( file_name );
        i.interpret( pmc );

        if ( parse_result.count( "save" ) )
        {
            std::stringstream ss;

            ss << "vnm-output" /*<< file_name*/ << ".txt";

            out_stream = new std::fstream( ss.str().c_str(), std::ios::in | std::ios::out | std::ios::trunc );
        }

        if ( parse_result.count( "register" ) )
        {
            pmc.print_registers_table( out_stream ? *out_stream : std::cout );
        }

        while ( pmc.execute() )
        {
            if ( parse_result.count( "register" ) )
            {
                pmc.print_registers( out_stream ? *out_stream : std::cout );
            }

            if ( parse_result.count( "memory" ) )
            {
                pmc.print_memory( out_stream ? *out_stream : std::cout );
            }
                
            pmc.get_from_memory();
        }

        pmc.print_memory( out_stream ? *out_stream : std::cout );
    }
    catch ( std::ifstream::failure& e )
    {
        std::cout << "Error: Can't open/read file!" << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    if ( out_stream )
    {
        out_stream->close();
        delete out_stream;
    }

    return 0;
}
