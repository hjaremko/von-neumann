#include "catch.hpp"
#include "memory.hpp"

TEST_CASE( "memory tests", "[memory]" )
{
    using namespace vnm;

    auto mem { memory<16> {} };

    SECTION( "set and at" )
    {
        mem[ 1 ] = 11111;
        REQUIRE( mem[ 0 ] == 0 );
        REQUIRE( mem[ 1 ] == 11111 );
        REQUIRE( mem[ 2 ] == 0 );
    }

    SECTION( "at throws on invalid address" )
    {
        REQUIRE_THROWS( mem[ 16 ] );
        REQUIRE_THROWS( mem[ static_cast<word::type>( -1 ) ] );
        REQUIRE_NOTHROW( mem[ 15 ] );
    }

    SECTION( "set throws on invalid address" )
    {
        REQUIRE_THROWS( mem[ 16 ] = word {} );
        REQUIRE_THROWS( mem[ static_cast<word::type>( -1 ) ] = word {} );
        REQUIRE_NOTHROW( mem[ 15 ] = word {} );
    }

    SECTION( "const at throws on invalid address" )
    {
        const auto cmem { memory<16> {} };
        REQUIRE_THROWS( cmem[ 16 ] );
        REQUIRE_THROWS( cmem[ static_cast<word::type>( -1 ) ] );
        REQUIRE_NOTHROW( cmem[ 15 ] );
    }

    SECTION( "size" )
    {
        REQUIRE( mem.size == 16 );
    }
}
