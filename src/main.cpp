#include "cxxopts.hpp"
#include "interpreter.hpp"
#include "machine.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

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

int main( int argc, char* argv[] )
{
    vnm::machine pmc;
    std::fstream out_file;

    try
    {
        const auto parse_result = parse_command_line( argc, argv );
        auto out_stream = &std::cout;
        std::filesystem::path input_path;

        if ( parse_result.count( "file" ) )
        {
            input_path /= parse_result[ "file" ].as<std::string>();
        }
        else
        {
            throw std::logic_error( "No input file!" );
        }

        vnm::interpreter i( input_path.string() );
        i.interpret( pmc );

        if ( parse_result.count( "save" ) )
        {
            std::stringstream ss;

            ss << "output-" << input_path.filename().string() << ".txt";

            out_file.open( ss.str().c_str(), std::ios::out | std::ios::trunc );
            out_stream = &out_file;
        }

        const auto register_print = parse_result.count( "register" );
        const auto memory_print = parse_result.count( "memory" );

        if ( register_print )
        {
            pmc.print_registers_table( *out_stream );
        }

        if ( parse_result.count( "counter" ) )
        {
            pmc.set_pc( vnm::word( parse_result[ "counter" ].as<int>() ) );
        }

        while ( pmc.execute() )
        {
            if ( register_print )
            {
                pmc.print_registers( *out_stream );
            }

            if ( memory_print )
            {
                pmc.print_memory( *out_stream );
            }

            pmc.tick();
        }

        pmc.print_memory( *out_stream );
    }
    catch ( std::ifstream::failure& /*e*/ )
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
