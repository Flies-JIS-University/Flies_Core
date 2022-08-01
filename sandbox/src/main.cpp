﻿#include <iostream>


#include <lib.h>


#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>
#include <asio/awaitable.hpp>
#include <cstdio>

using asio::ip::tcp;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
namespace this_coro = asio::this_coro;

#if defined(ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

awaitable<void> echo(tcp::socket socket)
{
  try
  {
    for (;;)
    {
      char data[1024];
      std::size_t n = co_await socket.async_read_some(asio::buffer(data), use_awaitable);
      data[n] = '\0';
      std::string msg =  data;
      std::string s = socket.remote_endpoint().address().to_string();
      std::cout << "[recv from (" << s << ':' << socket.remote_endpoint().port() << ")] " 
          << msg << std::endl;

      co_await async_write(socket, asio::buffer(data, n), use_awaitable);
    }
  }
  catch (std::exception& e)
  {
      // connection close
    std::printf("echo Exception: %s\n", e.what());
  }
}

awaitable<void> listener()
{
  auto executor = co_await this_coro::executor;
  tcp::acceptor acceptor(executor, {tcp::v4(), 55555});
  for (;;)
  {
    tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
    std::string s = socket.remote_endpoint().address().to_string();
    std::cout << "[connected] " << s << std::endl;
     
    co_spawn(executor, echo(std::move(socket)), detached);
  }
}

int main()
{
  try
  {
      std::cout << "❤️❤️" << std::endl;

      std::printf("🌟😃😇🌏🎮\n");
    asio::io_context io_context(1);

    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto){ io_context.stop(); });

    co_spawn(io_context, listener(), detached);

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::printf("Exception: %s\n", e.what());
  }
}
