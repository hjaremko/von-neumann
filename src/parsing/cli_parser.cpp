#include "parsing/cli_parser.hpp"

#include <fstream>

namespace
{

auto parse_command_line( int argc, char** argv ) -> cxxopts::ParseResult
{
    static auto options { cxxopts::Options {
        *argv, "Von Neumann Machine emulator v0.3.0" } };
    options.positional_help( "[optional args]" ).show_positional_help();

    options.add_options()( "h,help",
                           "Show help" )( "f,file",
                                          "Path to the VNM program file",
                                          cxxopts::value<std::string>() )(
        "c,counter",
        "Set initial program counter value",
        cxxopts::value<int>() )( "s,save", "Save output to file" )(
        "r,register", "Print register values before every cycle" )(
        "m,memory", "Print memory before every cycle" )(
        "b,binary", "Print instruction arguments in binary" )(
        "d,signed", "Print instruction arguments as 9-bit signed integers" );

    auto result = options.parse( argc, argv );

    if ( result.count( "help" ) || result.arguments().empty() )
    {
        fmt::print( "{}\n", options.help( { "", "Group" } ) );
        std::exit( 0 );
    }

    return result;
}

} // namespace

vnm::cli_parser::cli_parser( int argc, char** argv )
    : parse_result { parse_command_line( argc, argv ) }
{
}

auto vnm::cli_parser::make_machine() const -> vnm::machine
{
    auto input_file { std::fstream { get_input_filename() } };
    auto pmc { vnm::machine {} };

    pmc.ram = vnm::interpreter { input_file }.interpret();
    pmc.program_counter = get_starting_pc();

    return pmc;
}

auto vnm::cli_parser::get_input_filename() const -> std::filesystem::path
{
    if ( !parse_result.count( "file" ) )
    {
        throw std::logic_error( "No input file!" );
    }

    return parse_result[ "file" ].as<std::string>();
}

auto vnm::cli_parser::get_starting_pc() const -> vnm::word
{
    return parse_result.count( "counter" )
               ? parse_result[ "counter" ].as<vnm::word::type>()
               : 0;
}

auto vnm::cli_parser::make_printer( const vnm::machine& m ) const
    -> std::unique_ptr<printer_interface>
{
    using namespace format_policy;

    if ( parse_result.count( "binary" ) )
    {
        return make_base_printer<binary>( m );
    }

    if ( parse_result.count( "signed" ) )
    {
        return make_base_printer<with_sign>( m );
    }

    return make_base_printer<normal>( m );
}

auto vnm::cli_parser::get_parse_result() const -> cxxopts::ParseResult
{
    return parse_result;
}
