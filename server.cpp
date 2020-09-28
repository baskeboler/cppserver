#include "server.h"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/log/trivial.hpp>
#include <catch2/catch.hpp>

#include "./handlers.h"
#include "./html_element.h"
#include "./listener.h"
#include "./session.h"
namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

void start_server(server &serv, char *address, char *port, char *root,
                  char *threads) {
  serv.address = net::ip::make_address(address);
  serv.port = static_cast<uint16_t>(std::atoi(port));
  serv.doc_root = std::make_shared<string>(root);
  serv.threads = std::max<int>(1, std::atoi(threads));
  serv.ioc = std::make_shared<net::io_context>(serv.threads);

  std::make_shared<listener>(serv.ioc, tcp::endpoint{serv.address, serv.port},
                             serv.doc_root)
      ->run();

  // Run the I/O service on the requested number of threads
  std::vector<std::thread> v;
  v.reserve(serv.threads - 1);
  for (auto i = serv.threads - 1; i > 0; --i)
    v.emplace_back([&serv] { serv.ioc->run(); });
  serv.ioc->run();
}
