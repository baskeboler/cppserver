#include "handlers.h"
using std::string;

boost::beast::string_view mime_type(boost::beast::string_view path) {
  using beast::iequals;
  auto const ext = [&path] {
    auto const pos = path.rfind(".");
    if (pos == beast::string_view::npos)
      return beast::string_view{};
    return path.substr(pos);
  }();
  if (iequals(ext, ".htm"))
    return "text/html";
  if (iequals(ext, ".html"))
    return "text/html";
  if (iequals(ext, ".php"))
    return "text/html";
  if (iequals(ext, ".css"))
    return "text/css";
  if (iequals(ext, ".txt"))
    return "text/plain";
  if (iequals(ext, ".js"))
    return "application/javascript";
  if (iequals(ext, ".json"))
    return "application/json";
  if (iequals(ext, ".xml"))
    return "application/xml";
  if (iequals(ext, ".swf"))
    return "application/x-shockwave-flash";
  if (iequals(ext, ".flv"))
    return "video/x-flv";
  if (iequals(ext, ".png"))
    return "image/png";
  if (iequals(ext, ".jpe"))
    return "image/jpeg";
  if (iequals(ext, ".jpeg"))
    return "image/jpeg";
  if (iequals(ext, ".jpg"))
    return "image/jpeg";
  if (iequals(ext, ".gif"))
    return "image/gif";
  if (iequals(ext, ".bmp"))
    return "image/bmp";
  if (iequals(ext, ".ico"))
    return "image/vnd.microsoft.icon";
  if (iequals(ext, ".tiff"))
    return "image/tiff";
  if (iequals(ext, ".tif"))
    return "image/tiff";
  if (iequals(ext, ".svg"))
    return "image/svg+xml";
  if (iequals(ext, ".svgz"))
    return "image/svg+xml";
  return "text/plain";
}

void fail(beast::error_code ec, const char *what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

std::string path_cat(beast::string_view base, beast::string_view path) {
  if (base.empty())
    return std::string(path);
  std::string result(base);
#ifdef BOOST_MSVC
  char constexpr path_separator = '\\';
  if (result.back() == path_separator)
    result.resize(result.size() - 1);
  result.append(path.data(), path.size());
  for (auto &c : result)
    if (c == '/')
      c = path_separator;
#else
  char constexpr path_separator = '/';
  if (result.back() == path_separator)
    result.resize(result.size() - 1);
  result.append(path.data(), path.size());
#endif
  return result;
}

bool is_directory(std::string path) { return fs::is_directory(path); }

html_element build_link(const string &label, const string &href) {

  return html_element{
      "a", {{"href", href}}, {std::make_shared<text_element>(label)}};
}

html_element list_item_wrap(html_element &el) {
  return html_element{
      "li", {}, {std::shared_ptr<html_element>(new html_element{el})}};
}

html_element list_item_wrap(text_element &el) {
  return html_element{"li", {}, {el.get_shared()}};
}

html_element element_wrap(const std::string &tagname, html_element &el) {
  return html_element{
      tagname, {}, {std::shared_ptr<html_element>(new html_element{el})}};
}

std::string dir_table2(const std::string &path) {
  std::stringstream ss;
  ss << "<table><thead>"
     << "<tr>"
     << "<th>name</th><th>type</th>"
     << "</tr>"
     << "<tbody>";

  for (auto &d : fs::directory_iterator(path)) {
    ss << "<tr>"
       << "<td>" << d.path() << "</td>"
       << "<td>" << mime_type(d.path().generic_string()) << "</td>"
       << "</tr>";
  }
  ss << "</tbody>"
     << "</table>";
  return ss.str();
}
std::string html_document_wrap(const std::string &title,
                               const std::string &html_content) {
  std::stringstream ss;
  ss << "<html>"
     << "<head><title>" << title << "</title></head>"
     << "<body>" << html_content << "</body>"
     << "</html>";
  return ss.str();
}
