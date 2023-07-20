#pragma once

#include <memory>
#include <vector>
#include <thread>

#include <boost/asio.hpp>
// #include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/system/error_code.hpp>

template <typename ConnectionHandler>
class AsioGenericServer
{
    using shared_handler_t = std::shared_ptr<ConnectionHandler>;

public:
    AsioGenericServer(int thread_count = 1)
        : thread_count_(thread_count), acceptor_(io_service_)
    {
    }

    void startServer(uint16_t port)
    {
        std::cout << "server started\n";

        auto handler = std::make_shared<ConnectionHandler>(io_service_);

        // set up the acceptor to listen on the tcp port
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        acceptor_.async_accept(handler->socket(), [=](auto ec)
                               { handleNewConnection(handler, ec); });

        // start pool of threads to process the asio events
        for (int i = 0; i < thread_count_; ++i)
        {
            thread_pool_.emplace_back([=]
                                      { io_service_.run(); });
        }
    }

private:
    void handleNewConnection(shared_handler_t handler, boost::system::error_code const &error)
    {
        std::cout << "handling new connection, errCode: " << error << "\n";

        if (error)
        {
            return;
        }

        handler->start();

        auto new_handler = std::make_shared<ConnectionHandler>(io_service_);

        acceptor_.async_accept(new_handler->socket(),
                               [=](auto ec)
                               {
                                   handleNewConnection(new_handler, ec);
                               });
    }

    int thread_count_;
    std::vector<std::thread> thread_pool_;
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};