#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

/*
 * DO NOT PUT YOUR TESTS IN THIS FILE UNLESS YOU LIKE LONG COMPILE TIMES!
 * ======================================================================
 *
 * When this file is compiled it will compile ALL of Catch2, which is quite
 * slow. Given that the compiler will recompile any files that change between
 * compilations, we want this file to NEVER CHANGE so that it only gets
 * compiled once (and all subsequent compilations will be speedy!)
 *
 * Keep all of your test cases in other `.cpp` files. You only need to
 * `#include "catch.hpp"` and then you can write your TEST_CASEs
 * and REQUIREs as usual.
 */

/*
 * See:
 * https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#writing-tests
 * for documentation on how to write test cases.
 */
