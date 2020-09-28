#ifndef LISTENER_H
#define LISTENER_H

#include <algorithm>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
//#include <boost/log/trivial.hpp>
#include "handlers.h"
#include "session.h"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

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
