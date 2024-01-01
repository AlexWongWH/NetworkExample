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


class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
  std::shared_ptr<tcp_connection> pointer;

  static std::shared_ptr<tcp_connection> create(asio::io_context& io_context)
  {
    return std::shared_ptr<tcp_connection>(new tcp_connection(io_context));
  }

  asio::ip::tcp::socket& socket()
  {
    return socket_;
  }


  void start()
  {
    message_ = make_daytime_string();
  //     asio::write(socket, asio::buffer(message), ignored_error);

    asio::async_write(socket_, asio::buffer(message_), std::bind(&tcp_connection::handle_write , shared_from_this(),asio::placeholders::error,
            asio::placeholders::bytes_transferred ));

  }

  // further actions for this client connection are now the responsibility of handle_write().
  tcp_connection(asio::io_context& io_context)
    : socket_(io_context)
  {
  }

void handle_write(const asio::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

private:
  asio::ip::tcp::socket socket_;
  std::string message_;

};



class tcp_server
{
public:
tcp_server(asio::io_context& io_context)
  : io_context_(io_context),
    acceptor_(io_context, asio::ip::tcp::tcp::endpoint(asio::ip::tcp::tcp::v4(), 13))
{
  start_accept();
}

private:

  //creates socket and init async object to wait for new connection 
  void start_accept()
  {
     std::shared_ptr<tcp_connection> new_connection =
          tcp_connection::create(io_context_);

          acceptor_.async_accept(new_connection->socket(),
        std::bind(&tcp_server::handle_accept, this, new_connection,
          asio::placeholders::error));

      // asio::ip::tcp::socket socket{io_context_};
      //       acceptor_.accept(socket);

    //     std::string message = make_daytime_string();
    //     asio::error_code ignored_error;
    //     asio::write(socket, asio::buffer(message), ignored_error);

  }

  //function handle_accept() is called when the asynchronous accept operation initiated by start_accept() finishes. 
  //It services the client request, and then calls start_accept() to initiate the next accept operation.
  void handle_accept(std::shared_ptr<tcp_connection> new_connection, const asio::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  asio::io_context& io_context_;
  asio::ip::tcp::acceptor acceptor_; // accepting incoming network assiociation
};




int main() {
  std::cout << "hello from server!\n";

  // try {
  //   asio::io_context io_context;
  //   // created to listen for new connections
  //   // initialized to listen on TCP port 13 , ipv4 protocol
  //   asio::ip::tcp::acceptor acceptor(
  //       io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));
  //   // iterative server, one connection at a time
  //   for (;;) {
  //     asio::ip::tcp::socket socket{io_context};
  //     acceptor.accept(socket);
  //     std::string message = make_daytime_string();
  //     asio::error_code ignored_error;
  //     asio::write(socket, asio::buffer(message), ignored_error);
  //   }
  // } catch (const std::exception &e) {
  //   std::cerr << e.what() << "\n";
  // }

  try
  {
    asio::io_context io_context;
    tcp_server server{io_context}; // server object
    io_context.run();
  }catch(const std::exception & e)
  {
    std::cerr << e.what() <<std::endl;
  }

  return 0;
}