#include "parsing/cli_parser.hpp"

#include "printing/printer.hpp"

#include <fstream>

namespace
{

auto parse_command_line( int argc, char** argv ) -> cxxopts::ParseResult
{
    static auto options { cxxopts::Options { *argv, "Von Neumann Machine emulator" } };
    options.positional_help( "[optional args]" ).show_positional_help();

    options.add_options()( "h,help", "Show help" )(
        "f,file", "Path to the VNM program file", cxxopts::value<std::string>() )(
        "c,counter", "Set initial program counter value", cxxopts::value<int>() )(
        "s,save", "Save output to file" )( "r,register",
                                           "Print register values before every cycle" )(
        "m,memory", "Print memory before every cycle" )( "b,binary",
                                                         "Print instruction arguments in binary" )(
        "d,signed", "Print instruction arguments as 9-bit signed integers" );

    auto result = options.parse( argc, argv );

    if ( result.count( "help" ) || result.arguments().empty() )
    {
        std::cout << options.help( { "", "Group" } ) << std::endl;
        std::exit( 0 );
    }

    return result;
}

auto open_output_file( std::fstream& out_file, std::string_view filename ) -> std::ostream*
{
    auto ss { std::stringstream {} };

    ss << "output-" << filename << ".txt";
    out_file.open( ss.str().c_str(), std::ios::out | std::ios::trunc );

    return &out_file;
}

} // namespace

vnm::cli_parser::cli_parser( int argc, char** argv )
    : parse_result { parse_command_line( argc, argv ) }
{
}

auto vnm::cli_parser::make_machine() const -> vnm::machine
{
    auto input_path { get_input_filename() };
    auto input_file { std::fstream { input_path } };
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
    return parse_result.count( "counter" ) ? parse_result[ "counter" ].as<vnm::word::type>() : 0;
}

auto vnm::cli_parser::make_printer( const vnm::machine& m ) const
    -> std::unique_ptr<printer_interface>
{
    using namespace print_policy;
    auto out_file { std::fstream {} };
    auto* output_stream { get_output_stream( out_file ) };

    if ( parse_result.count( "binary" ) )
    {
        return std::make_unique<printer<binary>>( *output_stream, m );
    }

    if ( parse_result.count( "signed" ) )
    {
        return std::make_unique<printer<with_sign>>( *output_stream, m );
    }

    return std::make_unique<printer<normal>>( *output_stream, m );
}

auto vnm::cli_parser::get_output_stream( std::fstream& out_file ) const -> std::ostream*
{
    return parse_result.count( "save" )
               ? open_output_file( out_file, get_input_filename().stem().string() )
               : &std::cout;
}

auto vnm::cli_parser::get_parse_result() const -> const cxxopts::ParseResult&
{
    return parse_result;
}
