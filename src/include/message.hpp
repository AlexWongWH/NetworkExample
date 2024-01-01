#include <iostream>
#include <chrono>
#include <asio.hpp>

//what does the message header require?

class ChatMessage
{
    public:
    static constexpr std::size_t header_length {4};
    static constexpr std::size_t max_msg_length {1020};

    ChatMessage() : msglength_{0} {}

    const char* GetData() const {return data_;}
    char* GetData() {return data_;}

    std::size_t GetMsgLength() {return header_length + msglength_; }

    void WriteData(const char* msg, size_t msgsize)
    {
        std::memcpy( (&data_+header_length), msg, msgsize);
    }

    //operation for header checksum??
    //simple function for now
    void EncodeHeader()
    {
        std::memset(&data_,1,4);
    }


    private:
    char data_[header_length + msg_length];
    std::size_t msglength_{};
}


