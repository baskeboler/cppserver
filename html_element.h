#ifndef HTML_ELEMENT_H
#define HTML_ELEMENT_H
#include <algorithm>
#include <exception>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

struct abstract_dom_element {

  virtual ~abstract_dom_element() = default;
  virtual std::unique_ptr<abstract_dom_element> clone() const = 0;
  virtual std::string to_string() const = 0;

  friend std::ostream &operator<<(std::ostream &os,
                                  const abstract_dom_element &el);
};

std::ostream &operator<<(std::ostream &os, const abstract_dom_element &el);

template <bool isPlainText, typename derived>
struct dom_element
    : public abstract_dom_element,
      public std::enable_shared_from_this<dom_element<isPlainText, derived>> {

public:
  dom_element() = default;
  dom_element(const dom_element<isPlainText, derived> &other) = default;
  dom_element(dom_element<isPlainText, derived> &&other) = default;
  std::shared_ptr<abstract_dom_element> get_shared() {
    return this->shared_from_this();
  }

  virtual ~dom_element() = default;

public:
  virtual std::unique_ptr<abstract_dom_element> clone() const override {
    return std::make_unique<derived>(static_cast<derived const &>(*this));
  }

  // abstract_dom_element interface
public:
  virtual std::string to_string() const override { return ""; }
};

struct text_element : public dom_element<true, text_element> {
  std::string text_;

public:
  std::string to_string() const override;
  virtual ~text_element() = default;
  text_element(const std::string &t = ""); // template <bool istext>
};

typedef std::vector<std::shared_ptr<abstract_dom_element>> children_vector;

struct html_element : public dom_element<false, html_element> //  friend class

{
  using children_vector = std::vector<std::shared_ptr<abstract_dom_element>>;
  using attr_map = std::map<std::string, std::string>;
  std::string tagname_;
  attr_map attributes_;
  children_vector children_;

  std::string to_string() const override;
  html_element() = default;
  html_element(const html_element &) = default;
  html_element(html_element &&) = default;
  html_element(const std::string &tag, const attr_map &attrs = {},
               const children_vector &children = {});
  virtual ~html_element() = default;
};

#endif // HTML_ELEMENT_H
