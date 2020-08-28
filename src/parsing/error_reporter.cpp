#include "parsing/error_reporter.hpp"

#include <fmt/core.h>

vnm::error_reporter::error_reporter( std::string source )
    : source_( std::move( source ) )
{
}

void vnm::error_reporter::report( const std::string& error_message,
                                  const token& invalid_token )
{
    errors_.emplace_back( invalid_token, error_message, source_ );
}

void vnm::error_reporter::report( error&& e )
{
    errors_.push_back( std::move( e ) );
}

void vnm::error_reporter::print_errors() const
{
    for ( const auto& error : errors_ )
    {
        fmt::print( "{}\n", error.make_error_message() );
    }
}

auto vnm::error_reporter::has_errors() const -> bool
{
    return !errors_.empty();
}

auto vnm::error_reporter::count() const -> unsigned int
{
    return errors_.size();
}

void vnm::error_reporter::clear()
{
    errors_.clear();
}
