
#include "handlers.h"
#include <catch2/catch.hpp>

TEST_CASE("validate directories and files ", "[files]") {

  REQUIRE(is_directory("/tmp"));
}