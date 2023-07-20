#include <iostream>

#include <thread>
#include <chrono>

#include <boost/asio.hpp>

#include "generic_server.h"
#include "chat_handler.h"

int main()
{

    AsioGenericServer<ChatHandler> server;
    server.startServer(uint16_t(8888));

}