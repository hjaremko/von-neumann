#include "cli_parser.hpp"
#include "machine.hpp"

#include <cxxopts.hpp>
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

auto main( int argc, char** argv ) -> int
{
    try
    {
        auto cli_parser { vnm::cli_parser { argc, argv } };
        auto pmc { cli_parser.make_machine() };
        auto printer { cli_parser.make_printer( pmc ) };

        run_machine( pmc, *printer, cli_parser.get_parse_result() );
    }
    catch ( std::ifstream::failure& /*e*/ )
    {
        std::cout << "Error: Can't open/read file!" << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
