#include "html_element.h"

std::string html_element::to_string() const {
  std::stringstream ss;
  std::vector<std::string> attr_strs{};
  for (const auto &entry : attributes_) {
    std::string s(" " + entry.first + "=\"" + entry.second + "\"");
    attr_strs.push_back(s);
  }
  ss << "<" << tagname_;
  for (auto &a : attr_strs)
    ss << a;
  if (children_.empty()) {
    ss << "/>";
  } else {
    ss << ">";
    for (const auto &c : children_) {
      ss << c->to_string();
    }
    ss << "</" << tagname_ << ">";
  }
  return ss.str();
}

std::string text_element::to_string() const { return text_; }

text_element::text_element(const std::string &t) : text_{t} {}

html_element::html_element(const std::string &tag, const attr_map &attrs,
                           const children_vector &children)
    : tagname_{tag}, attributes_{attrs}, children_{children} {}

children_vector empty_children_vec() { return {}; }

std::ostream &operator<<(std::ostream &os, const abstract_dom_element &el) {
  return os << el.to_string();
}
