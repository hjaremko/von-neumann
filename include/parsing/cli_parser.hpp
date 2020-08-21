#ifndef VON_NEUMANN_CLI_PARSER_HPP
#define VON_NEUMANN_CLI_PARSER_HPP

#include "interpreter.hpp"
#include "machine.hpp"
#include "printing/printer_interface.hpp"

#include <cxxopts.hpp>
#include <filesystem>

namespace vnm
{

class cli_parser
{
public:
    cli_parser( int argc, char** argv );

    [[nodiscard]] auto make_machine() const -> machine;
    [[nodiscard]] auto make_printer( const machine& ) const -> std::unique_ptr<printer_interface>;
    [[nodiscard]] auto get_parse_result() const -> const cxxopts::ParseResult&;

private:
    auto get_input_filename() const -> std::filesystem::path;
    auto get_starting_pc() const -> vnm::word;
    auto get_output_stream( std::fstream& out_file ) const -> std::ostream*;

    cxxopts::ParseResult parse_result;
};

} // namespace vnm
#endif // VON_NEUMANN_CLI_PARSER_HPP
