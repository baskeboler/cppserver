#include "handlers.h"
#include "html_element.h"
#include <algorithm>
#include <catch2/catch.hpp>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;
TEST_CASE("html text element print", "[htmltext]") {
  text_element elem{"this is my text"};
  REQUIRE(elem.to_string() == "this is my text");
}

TEST_CASE("create a div with class ", "[htmltext]") {
  html_element elem{
      "div", {{"class", "somestyle"}}, {new text_element("some text")}};
  REQUIRE(elem.to_string() == "<div class=\"somestyle\">some text</div>");
}

TEST_CASE("wrap list item", "[htmltext]") {
  html_element elem{
      "div", {{"class", "somestyle"}}, {new text_element("some text")}};
  REQUIRE(elem.to_string() == "<div class=\"somestyle\">some text</div>");
  REQUIRE(elem.to_string() == "<div class=\"somestyle\">some text</div>");
  html_element wrapped = list_item_wrap(elem);
  string res = wrapped.to_string();
  REQUIRE(res == "<li><div class=\"somestyle\">some text</div></li>");
}

TEST_CASE("wrap list item text", "[htmltext]") {
  vector<text_element> ns = {text_element{"1"}, text_element{"2"},
                             text_element{"3"}};
  children_vector items;

  for (auto &i : ns) {
    items.push_back(new html_element(list_item_wrap(i)));
  }

  html_element elem{
      "ul", {}, vector<abstract_dom_element *>{items.begin(), items.end()}};
  //  REQUIRE(elem.to_string() ==
  //          "<li><div class=\"somestyle\">some text</div></li>");
}
