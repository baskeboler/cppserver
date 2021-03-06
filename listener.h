#ifndef LISTENER_H
#define LISTENER_H

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

//#include <boost/asio/dispatch.hpp>
//#include <boost/asio/strand.hpp>
//#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
#include <boost/config.hpp>
//#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
//#include <boost/log/trivial.hpp>
//#include "handlers.h"
//#include "session.h"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener> {
  std::shared_ptr<net::io_context> ioc_;
  tcp::acceptor acceptor_;
  std::shared_ptr<std::string const> doc_root_;

public:
  listener(std::shared_ptr<net::io_context> ioc, tcp::endpoint endpoint,
           std::shared_ptr<std::string const> const &doc_root);

  // Start accepting incoming connections
  void run();

private:
  void do_accept();

  void on_accept(beast::error_code ec, tcp::socket socket);
};

#endif // LISTENER_H
