#include "machine.hpp"
#include "parsing/cli_parser.hpp"

#include <fmt/color.h>
#include <fstream>

void run_machine( vnm::machine& pmc,
                  const vnm::printer_interface& printer,
                  const cxxopts::ParseResult& parse_result )
{
    const auto register_print = parse_result.count( "register" );
    const auto memory_print = parse_result.count( "memory" );

    if ( register_print )
    {
        printer.print_registers_table();
    }

    while ( pmc.execute() )
    {
        if ( register_print )
        {
            printer.print_registers();
        }

        if ( memory_print )
        {
            printer.print_memory();
        }

        pmc.tick();
    }

    printer.print_memory();
}

void print_error( std::string_view error_msg )
{
    fmt::print( fg( fmt::color::crimson ) | fmt::emphasis::bold, "Error: " );
    fmt::print( "{}\n", error_msg );
}

auto main( int argc, char** argv ) -> int
{
    try
    {
        auto cli_parser { vnm::cli_parser { argc, argv } };
        auto pmc { cli_parser.make_machine() };
        auto printer { cli_parser.make_printer( pmc ) };

        run_machine( pmc, *printer, cli_parser.get_parse_result() );
        return 0;
    }
    catch ( std::ifstream::failure& /*e*/ )
    {
        print_error( "Can't open/read file!" );
    }
    catch ( std::exception& e )
    {
        print_error( e.what() );
    }

    return 1;
}
