#ifndef SERVER_H
#define SERVER_H
#include <map>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
//#include <boost/asio/dispatch.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <boost/asio/strand.hpp>
//#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
//#include <boost/beast/version.hpp>
#include <boost/config.hpp>

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

struct server {
  std::shared_ptr<net::io_context> ioc;
  net::ip::address address;
  uint16_t port;
  int threads;
  std::shared_ptr<std::string> doc_root;
};

extern void start_server(server &serv, char *address, char *port, char *root,
                         char *threads);

#endif // SERVER_H
