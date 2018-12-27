#include <iostream>
#include <fstream>

#include "cxxopts.hpp"
#include "word.hpp"
#include "machine.hpp"
#include "interpreter.hpp"

cxxopts::ParseResult parse_command_line( int argc, char* argv[] )
{
    static cxxopts::Options options( argv[ 0 ], "Von Neumann Machine emulator" );

    options.positional_help( "[optional args]" ).show_positional_help();

    options.add_options()
        ( "h,help", "Show help" )
        ( "f,file", "Path to the VNM program file", cxxopts::value<std::string>() )
        ( "c,counter", "Set initial program counter value", cxxopts::value<int>() )
        ( "s,save", "Save output to file" )
        ( "r,register", "Print register values before every cycle" )
        ( "m,memory", "Print memory before every cycle" );

    auto result = options.parse( argc, argv );

    if ( result.count( "help" ) || result.arguments().empty() )
    {
        std::cout << options.help( { "", "Group" } ) << std::endl;
        exit( 0 );
    }

    return result;
}

int main( int argc, char *argv[] )
{
    vnm::machine pmc;
    std::fstream out_file;

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

            out_file.open( ss.str().c_str(), std::ios::out | std::ios::trunc );
        }

        if ( parse_result.count( "register" ) )
        {
            pmc.print_registers_table( parse_result.count( "save" ) ? out_file : std::cout );
        }

        if ( parse_result.count( "counter" ) )
        {
            pmc.set_pc( vnm::word( parse_result[ "counter" ].as<int>() ) );
        }

        while ( pmc.execute() )
        {
            if ( parse_result.count( "register" ) )
            {
                pmc.print_registers( parse_result.count( "save" ) ? out_file : std::cout );
            }

            if ( parse_result.count( "memory" ) )
            {
                pmc.print_memory( parse_result.count( "save" ) ? out_file : std::cout );
            }

            pmc.get_from_memory();
        }

        pmc.print_memory( parse_result.count( "save" ) ? out_file : std::cout );
    }
    catch ( std::ifstream::failure& e )
    {
        std::cout << "Error: Can't open/read file!" << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    out_file.close();

    return 0;
}
