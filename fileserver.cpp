#include <algorithm>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <served/served.hpp>
#include <sstream>
#include <string>
using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;

void my_access_log(served::response &res, const served::request &req);

fs::path relativeTo(const fs::path &from, const fs::path &to);

string index_html(const fs::path &doc_root, const fs::path &r) {
  if (fs::is_directory(r)) {
    auto it = fs::directory_iterator(r);
    stringstream ss;
    ss << "<html><head>"
       //       << "<base href="
       //       << "/" / fs::relative(r, doc_root).append("/") << "
       //       target=\"_self\"/>"
       << "</head>"
       << "<body>";
    ss << "<table>";
    //    std::sort(fs::begin(it), fs::end(it));
    for (auto &entry : it) {
      auto p = fs::relative(entry, r);

      ss << "<tr>";
      ss << "<td>";
      ss << "<a href=" << relativeTo(entry.path(), doc_root) << ">"
         << entry.path().relative_path() << "</a>";
      ss << "</td>";
      ss << "<td>" << entry.file_size() << "</td>";

      ss << "</tr>";
    }
    //    for (auto &entry : r) {
    //      auto p = fs::relative(entry, r);
    //      ss << "<li>";
    //      ss << "<a href=" << p << ">" << p << "</a>";
    //      ss << "</li>";
    //    }

    ss << "</ul>"
       << "</body></html>";
    return ss.str();
  } else {
    BOOST_LOG_TRIVIAL(warning)
        << "entry is not a directory nor a regular file: " << r;
    return "";
  }
}

string get_file(const fs::path &p) {
  ifstream s{p};
  return string{istreambuf_iterator<char>(s), istreambuf_iterator<char>()};
}
int main(int ac, char **av) {
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "root", po::value<string>(), "root mount point path")(
      "port", po::value<int>()->default_value(8080), "port to listen to");

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  fs::path root = fs::current_path();
  if (vm.count("root")) {
    BOOST_LOG_TRIVIAL(info)
        << "mount point  was set to " << vm["root"].as<string>() << ".\n";
    root = fs::path{vm["root"].as<string>()};

    if (fs::exists(root)) {
      BOOST_LOG_TRIVIAL(info) << root << " exists";
    } else {
      BOOST_LOG_TRIVIAL(info) << root << " does not exist";
    }
  } else {
    BOOST_LOG_TRIVIAL(info)
        << "mount point was not set. using " << root << endl;
  }

  served::multiplexer mux;
  auto httphandler = [&](served::response &res, const served::request &req) {
    auto newroot = root;
    auto path = req.url().path().substr(1);
    auto thepath = newroot / path;
    BOOST_LOG_TRIVIAL(info) << "requested path: " << thepath;
    if (fs::is_directory(thepath)) {
      res.set_header("content-type", "text/html");
      res.set_status(200);
      res << index_html(root, thepath);
    } else if (fs::is_regular_file(thepath)) {
      res.set_body(get_file(thepath));
      //      res.set_header(thepath.)
    }
  };

  mux.use_before(my_access_log);
  mux.handle("/").get(httphandler);
  //  mux.handle("/image").post(handleimage).get(getImage);
  BOOST_LOG_TRIVIAL(info) << "Try this example with:" << std::endl;
  BOOST_LOG_TRIVIAL(info) << " curl http://localhost:" << vm["port"].as<int>()
                          << std::endl;

  served::net::server server("127.0.0.1", std::to_string(vm["port"].as<int>()),
                             mux);
  server.run(10); // Run with a pool of 10 threads.

  return 0;
}

void my_access_log(served::response &res, const served::request &req) {
  std::stringstream ss;

  //      boost::posix_time::time_facet *facet = new
  //      boost::posix_time::time_facet("%d/%b/%Y:%H:%M:%S");
  //      ss.imbue(std::locale(ss.getloc(), facet));

  std::string source = req.source();
  if (source.empty()) {
    source = "-";
  }

  //      ss << source << " - - [" <<
  //      boost::posix_time::second_clock::local_time() << " -0000]";
  ss << " \"" << method_to_string(req.method()) << " " << req.url().path()
     << " " << req.HTTP_version() << "\"";
  ss << " " << res.status() << " " << res.body_size();

  BOOST_LOG_TRIVIAL(debug) << ss.str();
}

fs::path relativeTo(const fs::path &from, const fs::path &to) {
  // Start at the root path and while they are the same then do nothing then
  // when they first diverge take the entire from path, swap it with '..'
  // segments, and then append the remainder of the to path.
  auto fromIter = from.begin();
  auto toIter = to.begin();

  // Loop through both while they are the same to find nearest common directory
  while (fromIter != from.end() && toIter != to.end() && *toIter == *fromIter) {
    ++toIter;
    ++fromIter;
  }

  // Replace from path segments with '..' (from => nearest common directory)
  auto finalPath = fs::path{};
  while (fromIter != from.end()) {
    finalPath /= "..";
    ++fromIter;
  }

  // Append the remainder of the to path (nearest common directory => to)
  while (toIter != to.end()) {
    finalPath /= *toIter;
    ++toIter;
  }

  return finalPath;
}