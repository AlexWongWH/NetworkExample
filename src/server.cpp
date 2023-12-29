#include "example.hpp"
#include <asio.hpp>
#include <chrono>
#include <ctime>
#include <iostream>

// creating a daytime protocol server

// required to sudo ./server

std::string make_daytime_string() {
  std::time_t t_today =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  // char* ctime( const std::time_t* time );

  // Www Mmm dd hh:mm:ss yyyy\n
  return std::ctime(&t_today);
}

int main() {
  std::cout << "hello from server!\n";

  try {
    asio::io_context io_context;

    // created to listen for new connections
    // initialized to listen on TCP port 13 , ipv4 protocol
    asio::ip::tcp::acceptor acceptor(
        io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));

    // iterative server, one connection at a time
    for (;;) {
      asio::ip::tcp::socket socket{io_context};
      acceptor.accept(socket);

      std::string message = make_daytime_string();
      asio::error_code ignored_error;
      asio::write(socket, asio::buffer(message), ignored_error);
    }

  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
  }

  return 0;
}