//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, asynchronous
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/log/trivial.hpp>

#include "./server.h"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
  using std::make_shared;
  using std::map;
  using std::string;
  if (argc != 5) {
    BOOST_LOG_TRIVIAL(info)
        << "Usage: http-server-async <address> <port> <doc_root> <threads>\n"
        << "Example:\n"
        << "    http-server-async 0.0.0.0 8080 . 1\n";
    return EXIT_FAILURE;
  }
  //  auto const address = net::ip::make_address(argv[1]);
  //  auto const port = static_cast<uint16_t>(std::atoi(argv[2]));
  //  auto const doc_root = std::make_shared<std::string>(argv[3]);
  //  auto const threads = std::max<int>(1, std::atoi(argv[4]));

  //  // The io_context is required for all I/O
  //  net::io_context ioc{threads};

  //  // Create and launch a listening port
  //  std::make_shared<listener>(ioc, tcp::endpoint{address, port}, doc_root)
  //      ->run();

  //  // Run the I/O service on the requested number of threads
  //  std::vector<std::thread> v;
  //  v.reserve(threads - 1);
  //  for (auto i = threads - 1; i > 0; --i)
  //    v.emplace_back([&ioc] { ioc.run(); });
  //  ioc.run();

  server serv;
  start_server(serv, argv[1], argv[2], argv[3], argv[4]);

  return EXIT_SUCCESS;
}
