
#include "handlers.h"
#include <catch2/catch.hpp>
using std::string;

TEST_CASE("validate directories and files ", "[files]") {

  REQUIRE(is_directory("/tmp"));
}

TEST_CASE("get relative path", "[files]") {
  string base = "/base/path";
  string wholepath = "/base/path/some/folder/file.txt";

  REQUIRE(relativepath(base, wholepath) == "/some/folder/file.txt");
}

TEST_CASE("pow 2 10", "[files]") { REQUIRE(POW_2_10 == 1024); }