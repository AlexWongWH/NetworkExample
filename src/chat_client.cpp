#include <asio.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <deque>

#include "message.hpp"


class Client
{
    public:
    Client(asio::io_context& io, const asio::ip::tcp::resolver::results_type& endpoints) 
    : socket_ {io} , io_context_{io} {
    
    }

    void connect(const asio::ip::tcp::resolver::results_type& endpoints)
    {
        asio::connect(socket_, endpoints); // connect would implictly check
    }


    private:
    asio::io_context& io_context_;
    asio::ip::tcp::socket_;
    std::deque<CharMessage> write_msg_;
    ChatMessage read_msg_;
};


//main function
// able to connect to the server

// using `message` format

// <ip address, tcp/udp port>


int main(int argc, char **argv)
{


    asio::ip::tcp::resolver::results_type endpoints = resolver_.resolve(argv[1], argv[2]);


    return 0;
}


