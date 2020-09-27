#include <fstream>
#include <iostream>
#include <istream>
#include <served/plugins.hpp>
#include <served/served.hpp>

using namespace std;

void hello_get_handler(served::response &res, const served::request &) {
  res << "Hello world";
}

void handleimage(served::response &res, const served::request &req) {
  std::ofstream of{"out.jpg"};

  // of << istream{req.body()};
}

void getImage(served::response &res, const served::request &req) {
  res << "<html><body>"
      << "<form method='post' action='/image' enctype='multipart/form=data'>"
      << "<input type='submit'>"
      << "<input name='thefile' type='file'>"
      << "</form></body></html>";
}

int main() {
  cout << "Hello World!" << endl;
  served::multiplexer mux;

  mux.use_before(served::plugin::access_log);
  mux.handle("/hello").get(hello_get_handler);
  mux.handle("/image").post(handleimage).get(getImage);
  std::cout << "Try this example with:" << std::endl;
  std::cout << " curl http://localhost:8123/hello" << std::endl;

  served::net::server server("127.0.0.1", "8123", mux);
  server.run(10); // Run with a pool of 10 threads.
  return 0;
}
