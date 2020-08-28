#ifndef VON_NEUMANN_CLI_PARSER_HPP
#define VON_NEUMANN_CLI_PARSER_HPP

#include "interpreter.hpp"
#include "machine.hpp"
#include "printing/file_printer.hpp"

#include <cxxopts/cxxopts.hpp>
#include <filesystem>
#include <fstream>

namespace vnm
{

class cli_parser
{
public:
    cli_parser( int argc, char** argv );

    [[nodiscard]] auto make_machine() const -> machine;
    [[nodiscard]] auto make_printer( const machine& ) const
        -> std::unique_ptr<printer_interface>;
    [[nodiscard]] auto get_parse_result() const -> cxxopts::ParseResult;

private:
    auto get_input_filename() const -> std::filesystem::path;
    auto get_starting_pc() const -> vnm::word;

    template <class Policy>
    auto make_base_printer( const vnm::machine& m ) const
        -> std::unique_ptr<printer_interface>
    {
        const auto make_output_filename = []( const auto& input_path ) {
            return fmt::format( "output-{}.txt", input_path.stem().string() );
        };

        if ( parse_result.count( "save" ) )
        {
            return std::make_unique<file_printer<Policy>>(
                make_output_filename( get_input_filename() ), m );
        }

        return std::make_unique<printer<Policy>>( std::cout, m );
    }

    cxxopts::ParseResult parse_result;
};

} // namespace vnm
#endif // VON_NEUMANN_CLI_PARSER_HPP
