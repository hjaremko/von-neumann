#include "catch.hpp"
#include "memory.hpp"

TEST_CASE( "memory tests", "[memory]" )
{
    using namespace vnm;

    auto mem { memory<16> {} };

    SECTION( "set and at" )
    {
        mem[ word { 1 } ] = word { 11111 };
        REQUIRE( mem[ word { 0 } ] == word { 0 } );
        REQUIRE( mem[ word { 1 } ] == word { 11111 } );
        REQUIRE( mem[ word { 2 } ] == word { 0 } );
    }

    SECTION( "at throws on invalid address" )
    {
        REQUIRE_THROWS( mem[ word { 16 } ] );
        REQUIRE_THROWS( mem[ word { static_cast<word::type>( -1 ) } ] );
        REQUIRE_NOTHROW( mem[ word { 15 } ] );
    }

    SECTION( "set throws on invalid address" )
    {
        REQUIRE_THROWS( mem[ word { 16 } ] = word {} );
        REQUIRE_THROWS( mem[ word { static_cast<word::type>( -1 ) } ] = word {} );
        REQUIRE_NOTHROW( mem[ word { 15 } ] = word {} );
    }

    SECTION( "size" )
    {
        REQUIRE( mem.size == 16 );
    }
}
