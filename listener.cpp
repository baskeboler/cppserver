#include "listener.h"
#include <memory>
#include <utility>

#include "./handlers.h"
#include "./session.h"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/log/trivial.hpp>

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

listener::listener(std::shared_ptr<boost::asio::io_context> ioc,
                   tcp::endpoint endpoint,
                   const std::shared_ptr<const std::string> &doc_root)
    : ioc_(ioc), acceptor_(net::make_strand(*ioc)), doc_root_(doc_root) {
  beast::error_code ec;

  // Open the acceptor
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    fail(ec, "open");
    return;
  }

  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    fail(ec, "set_option");
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if (ec) {
    fail(ec, "bind");
    return;
  }

  // Start listening for connections
  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    fail(ec, "listen");
    return;
  }
}

void listener::run() { do_accept(); }

void listener::do_accept() {
  // The new connection gets its own strand
  acceptor_.async_accept(
      net::make_strand(*ioc_),
      beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}

void listener::on_accept(beast::error_code ec, tcp::socket socket) {
  BOOST_LOG_TRIVIAL(debug) << "on accept!";
  if (ec) {
    fail(ec, "accept");
  } else {
    // Create the session and run it
    std::make_shared<session>(std::move(socket), doc_root_)->run();
  }

  // Accept another connection
  do_accept();
}
