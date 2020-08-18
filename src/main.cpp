#include "interpreter.hpp"
#include "machine.hpp"
#include "printer_interface.hpp"

#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

cxxopts::ParseResult parse_command_line( int argc, char* argv[] )
{
    static auto options { cxxopts::Options { argv[ 0 ], "Von Neumann Machine emulator" } };
    options.positional_help( "[optional args]" ).show_positional_help();

    options.add_options()( "h,help", "Show help" )(
        "f,file", "Path to the VNM program file", cxxopts::value<std::string>() )(
        "c,counter", "Set initial program counter value", cxxopts::value<int>() )(
        "s,save", "Save output to file" )( "r,register",
                                           "Print register values before every cycle" )(
        "m,memory", "Print memory before every cycle" )( "b,binary",
                                                         "Print instruction arguments in binary" )(
        "d,signed", "Print instruction arguments as 9-bit signed integers" );

    const auto result = options.parse( argc, argv );

    if ( result.count( "help" ) || result.arguments().empty() )
    {
        std::cout << options.help( { "", "Group" } ) << std::endl;
        std::exit( 0 );
    }

    return result;
}

int main( int argc, char* argv[] )
{
    auto pmc { vnm::machine {} };
    auto out_file { std::fstream {} };

    try
    {
        const auto parse_result { parse_command_line( argc, argv ) };
        auto out_stream { &std::cout };
        auto input_path { std::filesystem::path {} };

        if ( parse_result.count( "file" ) )
        {
            input_path /= parse_result[ "file" ].as<std::string>();
        }
        else
        {
            throw std::logic_error( "No input file!" );
        }

        pmc.set_memory( vnm::interpreter { input_path.string() }.interpret() );

        if ( parse_result.count( "save" ) )
        {
            auto ss { std::stringstream {} };

            ss << "output-" << input_path.filename().string() << ".txt";
            out_file.open( ss.str().c_str(), std::ios::out | std::ios::trunc );
            out_stream = &out_file;
        }

        const auto register_print = parse_result.count( "register" );
        const auto memory_print = parse_result.count( "memory" );
        const auto printer { vnm::make_printer( parse_result, *out_stream, pmc ) };

        if ( register_print )
        {
            printer->print_registers_table();
        }

        if ( parse_result.count( "counter" ) )
        {
            pmc.set_pc( vnm::word( parse_result[ "counter" ].as<vnm::word::type>() ) );
        }

        while ( pmc.execute() )
        {
            if ( register_print )
            {
                printer->print_registers();
            }

            if ( memory_print )
            {
                printer->print_memory();
            }

            pmc.tick();
        }

        printer->print_memory();
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
