#include "catch.hpp"
#include "parsing/interpreter.hpp"

#include <sstream>

TEST_CASE( "interpreter", "[interpreter]" )
{
    using namespace vnm;

    SECTION( "parses input correctly" )
    {
        auto input { std::string { "LOAD @ 1\n"
                                   "DIV @ 0\n"
                                   "STORE $ 3\n"
                                   "LOAD @ 1; comment\n"
                                   "JNEG $ 24\n"
                                   "123\n"
                                   "STOP;" } };
        auto ss { std::stringstream { input } };
        auto actual = interpreter { ss }.interpret();

        REQUIRE( actual[ 0 ] == word { "LOAD", "@", 1 } );
        REQUIRE( actual[ 1 ] == word { "DIV", "@", 0 } );
        REQUIRE( actual[ 2 ] == word { "STORE", "$", 3 } );
        REQUIRE( actual[ 3 ] == word { "LOAD", "@", 1 } );
        REQUIRE( actual[ 4 ] == word { "JNEG", "$", 24 } );
        REQUIRE( actual[ 5 ] == 123 );
        REQUIRE( actual[ 6 ] == stop );
    }

    SECTION( "negative arguments are parsed correctly" )
    {
        auto input { std::string { "STORE $ -5\n"
                                   "-123\n"
                                   "STOP" } };

        auto ss { std::stringstream { input } };
        auto actual = interpreter { ss }.interpret();

        REQUIRE( actual[ 0 ] ==
                 word { "STORE", "$", static_cast<word::type>( -5 ) } );
        REQUIRE( actual[ 1 ] == word { static_cast<word::type>( -123 ) } );
        REQUIRE( actual[ 2 ] == stop );
    }

    SECTION( "newlines are ignored" )
    {
        auto input { std::string { "\n"
                                   "\n"
                                   "\n"
                                   "LOAD @ 1; comment\n"
                                   "\n"
                                   "STOP\n"
                                   "\n"
                                   "\n" } };
        auto ss { std::stringstream { input } };
        auto actual = interpreter { ss }.interpret();
        REQUIRE( actual[ 5 ] == stop );
    }
}

TEST_CASE( "input errors detection", "[interpreter]" )
{
    using namespace vnm;

    SECTION( "invalid instruction" )
    {
        auto input { std::string { "LOAD @ 1\n"
                                   "DIV @ 0\n"
                                   "STORE $ 3\n"
                                   "ROAD @ 1\n"
                                   "JNEG $ 24\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }

    SECTION( "unexpected character in pure value cells" )
    {
        auto input { std::string { "1231244\n"
                                   "123123d122\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }

    SECTION( "invalid addressing mode" )
    {
        auto input { std::string { "LOAD @ 1\n"
                                   "DIV # 0\n"
                                   "STORE $ 3\n"
                                   "LOAD @ 1\n"
                                   "JNEG * 24\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }

    //    SECTION( "instruction argument overflowing" )
    //    {
    //        auto input { std::string { "LOAD @ 1\n"
    //                                   "DIV @ 0\n"
    //                                   "STORE $ 3000\n"
    //                                   "LOAD $ 1000\n"
    //                                   "JNEG $ 24\n"
    //                                   "STOP" } };
    //        auto ss { std::stringstream { input } };
    //
    //        REQUIRE_THROWS( interpreter { ss }.interpret() );
    //    }

    SECTION( "only one instruction per line" )
    {
        auto input { std::string { "LOAD @ 1\n"
                                   "DIV @ 0\n"
                                   "STORE $ 3 LOAD $ 3\n"
                                   "LOAD @ 1\n"
                                   "JNEG $ 24\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }

    SECTION( "only one number per line" )
    {
        auto input { std::string { "123 123 123\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }

    SECTION( "expected argument in instruction (ex LOAD $)" )
    {
        auto input { std::string { "LOAD @ 1\n"
                                   "DIV @ \n"
                                   "STORE $ 3\n"
                                   "LOAD @ 1\n"
                                   "JNEG $\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }

    SECTION( "expected addressing mode (LOAD 2)" )
    {
        auto input { std::string { "LOAD 1\n"
                                   "DIV  0\n"
                                   "STORE $ 3\n"
                                   "LOAD @ 1\n"
                                   "JNEG\n"
                                   "STOP" } };
        auto ss { std::stringstream { input } };

        REQUIRE_THROWS( interpreter { ss }.interpret() );
    }
}
