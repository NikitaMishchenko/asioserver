#pragma once

#include <deque>
#include <string>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

class ChatHandler
    : public std::enable_shared_from_this<ChatHandler>
{
public:
    ChatHandler(boost::asio::io_service &service)
        : service_(service), socket_(service), write_strand_(service)
    {
        std::cout << "ChatHandler ctr\n";
    }

    boost::asio::ip::tcp::socket &socket()
    {
        return socket_;
    }

    void start()
    {
        std::cout << "ChatHandler start()\n";
        readPacket();
    }

    void send(const std::string& msg)
    {
        service_.post(write_strand_.wrap([&msg, me = shared_from_this()]()
                                         { me->queueMessage(msg); }));
    }

private:
    void readPacket()
    {
        std::cout << "ChatHandler readPacket\n";

        boost::asio::async_read_until(socket_,
                                      in_packet_,
                                      '\0',
                                      [me = shared_from_this()](boost::system::error_code const &ec, std::size_t bytes_xfer)
                                      {
                                          me->readPacketDone(ec, bytes_xfer);
                                      });
    }

    void readPacketDone(boost::system::error_code const &error, std::size_t bytesTransferred)
    {
        if (error)
        {
            return;
        }

        std::istream stream(&in_packet_);
        std::string packet_string;

        stream >> packet_string;

        // todo some work handled here
        // do something with it

        readPacket();
    }

    void queueMessage(std::string message)
    {
        bool write_in_progress = !send_packet_queue.empty();

        send_packet_queue.push_back(std::move(message));

        if (!write_in_progress)
        {
            startPacketSend();
        }
    }

    void startPacketSend()
    {
        send_packet_queue.front() += "\0";
        async_write(socket_,
                    boost::asio::buffer(send_packet_queue.front()),
                    write_strand_.wrap([me = shared_from_this()](boost::system::error_code const &ec, std::size_t)
                                       { me->packetSendDone(ec); }));
    }

    void packetSendDone(boost::system::error_code const &error)
    {
        if (!error)
        {
            send_packet_queue.pop_front();
            if (!send_packet_queue.empty())
            {
                startPacketSend();
            }
        }
    }

    boost::asio::io_service &service_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_service::strand write_strand_;
    boost::asio::streambuf in_packet_;
    std::deque<std::string> send_packet_queue;
};