#include "catch.hpp"
#include "interpreter.hpp"

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
                                   "STOP;" } };
        auto ss { std::stringstream { input } };
        auto actual = interpreter { ss }.interpret();

        REQUIRE( actual.at( word { 0 } ) == word { "LOAD", "@", 1 } );
        REQUIRE( actual.at( word { 1 } ) == word { "DIV", "@", 0 } );
        REQUIRE( actual.at( word { 2 } ) == word { "STORE", "$", 3 } );
        REQUIRE( actual.at( word { 3 } ) == word { "LOAD", "@", 1 } );
        REQUIRE( actual.at( word { 4 } ) == word { "JNEG", "$", 24 } );
        REQUIRE( actual.at( word { 5 } ) == word { "STOP", "$", 0 } );
        REQUIRE( actual.at( word { 6 } ) == word { 0 } );
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
