#include "catch.hpp"
#include "memory.hpp"

TEST_CASE( "memory tests", "[memory]" )
{
    using namespace vnm;

    auto mem { memory<16> {} };

    SECTION( "set and at" )
    {
        mem.set( word { 11111 }, word { 1 } );
        REQUIRE( mem.at( word { 0 } ) == word { 0 } );
        REQUIRE( mem.at( word { 1 } ) == word { 11111 } );
        REQUIRE( mem.at( word { 2 } ) == word { 0 } );
    }

    SECTION( "at throws on invalid address" )
    {
        REQUIRE_THROWS( mem.at( word { 16 } ) );
        REQUIRE_THROWS( mem.at( word { static_cast<word::type>( -1 ) } ) );
        REQUIRE_NOTHROW( mem.at( word { 15 } ) );
    }

    SECTION( "set throws on invalid address" )
    {
        REQUIRE_THROWS( mem.set( word {}, word { 16 } ) );
        REQUIRE_THROWS( mem.set( word {}, word { static_cast<word::type>( -1 ) } ) );
        REQUIRE_NOTHROW( mem.set( word {}, word { 15 } ) );
    }

    SECTION( "size" )
    {
        REQUIRE( mem.size == 16 );
    }
}
