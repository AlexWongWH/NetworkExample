// #include "../include/example.hpp"
#include "example.hpp"
#include <array>
#include <asio.hpp>
#include <iostream>

// creating a simple daytime protocol client
// to run the client -> ./client <ip_address>
// <ip_address> -> loopback/local ip address

class Client
{
  public:

  Client(asio::io_context& io) : socket_ {io} , resolver_{io} {

  }

  void connect(std::string ip_address)
  {
    asio::ip::tcp::resolver::results_type endpoints = resolver_.resolve(ip_address, "daytime");
    asio::connect(socket_, endpoints); // connect would implictly check
  }

  void run()
  {
    try{
      while(true)
      {
        std::size_t len = socket_.read_some(asio::buffer(buf), error);

        if (error == asio::error::eof) {
          break; // connection closed normally
        } else if (error) {
          throw asio::system_error(error);
        }
        std::cout.write(buf.data(), len);
      }
    }
    catch(std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }

  }

  private:

  asio::ip::tcp::socket socket_; // need the socket which has the io_context;
  //perform DNS (Domain Name System) resolution for TCP endpoints.
  asio::ip::tcp::resolver resolver_;
  std::array<char, 128> buf;
  asio::error_code error;

};

// passing the ip address
int main(int argc, char *argv[]) {

  std::cout << "hello from client\n";

  asio::io_context io_context;
  Client c1{io_context};
  c1.connect("127.0.0.1"); // local host for now
  c1.run();


  // try {
  //   if (argc != 2) {
  //     std::cerr << "ERROR Usage: client <host>" << std::endl;
  //     return 1;
  //   }
  //   asio::io_context io_context;
  //   // server name which was specified as a parameter to app
  //   asio::ip::tcp::resolver resolver(io_context);
  //   // A resolver takes a host name and service name and turns them into a list
  //   // of endpoints. <host name, service name> perform dns resolution for host
  //   // (argv[1]) and port("daytime") may return ipv4 or ipv6 endpoints
  //   asio::ip::tcp::resolver::results_type endpoints =
  //       resolver.resolve(argv[1], "daytime");
  //   // create and connect socket
  //   asio::ip::tcp::socket socket{io_context};
  //   asio::connect(socket, endpoints); // connect would implictly check
  //   std::cout << "starting loop\n";
  //   for (;;) {
  //     std::array<char, 128> buf;
  //     asio::error_code error;
  //     // asio::buffer automatically determines the size of the array & prevent
  //     // buf overruns
  //     std::size_t len = socket.read_some(asio::buffer(buf), error);

  //     if (error == asio::error::eof) {
  //       break; // connection closed normally
  //     } else if (error) {
  //       throw asio::system_error(error);
  //     }

  //     std::cout.write(buf.data(), len);
  //   }
  // } catch (std::exception &e) {
  //   std::cerr << e.what() << std::endl;
  // }

  return 0;
}