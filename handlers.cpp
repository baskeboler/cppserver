#include "handlers.h"
using std::string;
using std::stringstream;

string filesize(unsigned int n) {
  stringstream ss;
  if (n < POW_2_10) {
    ss << n << " bytes";
    return ss.str();
  }
  n /= POW_2_10;

  if (n < POW_2_10) {
    ss << n << " Kb";
    return ss.str();
  }
  n /= POW_2_10;
  if (n < POW_2_10) {
    ss << n << " Mb";
    return ss.str();
  }
  n /= POW_2_10;
  if (n < POW_2_10) {
    ss << n << " Gb";
    return ss.str();
  }
  n /= POW_2_10;
  if (n < POW_2_10) {
    ss << n << " Tb";
    return ss.str();
  }
  n /= POW_2_10;

  ss << n << " Pb";
  return ss.str();
}

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
  if (iequals(ext, ".pdf"))
    return "application/pdf";
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

string path_cat(beast::string_view base, beast::string_view path) {
  if (base.empty())
    return string(path);
  string result(base);
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

bool is_directory(string path) { return fs::is_directory(path); }

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

html_element element_wrap(const string &tagname, html_element &el) {
  return html_element{
      tagname, {}, {std::shared_ptr<html_element>(new html_element{el})}};
}

string relativepath(const string &base, const string &path) {
  if (path.starts_with(base)) {
    return path.substr(base.size());
  }
  return path;
}

string dir_table2(const string &server_root, const string &path) {
  std::stringstream ss;
  ss << "<table><thead>"
     << "<tr>"
     << "<th>name</th><th>type</th><th>size</th>"
     << "</tr>"
     << "<tbody>";

  for (auto &d : fs::directory_iterator(path)) {
    ss << "<tr>"
       << "<td>"
       << "<a href=\"" << relativepath(server_root, d.path()) << "\">"
       << relativepath(path, d.path()) << "</a>"
       << "</td>"
       << "<td>"
       << (d.is_directory() ? "directory"
                            : mime_type(d.path().generic_string()))
       << "</td>"
       << "<td>" << (d.is_regular_file() ? filesize(d.file_size()) : "-")
       << "</td>"
       << "</tr>";
  }
  ss << "</tbody>"
     << "</table>";
  return ss.str();
}
string html_document_wrap(const string &title, const string &html_content) {
  std::stringstream ss;
  ss << "<html>"
     << "<head><title>" << title << "</title></head>"
     << "<body>" << html_content << "</body>"
     << "</html>";
  return ss.str();
}
