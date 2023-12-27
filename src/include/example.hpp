// #include "../lib/asio.hpp"
#include <asio.hpp>
#include <iostream>

void firstasiofunction() {
  std::cout << "Entered  firstfunction\n";

  asio::io_context io;
  asio::steady_timer t(io, asio::chrono::seconds(5));
  t.wait();

  std::cout << "first function\n";
}
