#include <iostream>


#include <Flies.h>


#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>
#include <asio/awaitable.hpp>
#include <asio/ssl/context.hpp>
#include <asio/ssl/stream.hpp>
#include <cstdio>
#include <filesystem>
namespace fs = std::filesystem;

using asio::ip::tcp;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
namespace this_coro = asio::this_coro;

typedef asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;

#if defined(ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

#define CERT_PATH "res/cert.pem"
#define KEY_PATH "res/key.pem"


awaitable<void> echo(tcp::socket socket, asio::ssl::context& ssl_context)
{
  try

  {
      char data[1024];

      // before connect verify device id and cert
      std::size_t n = co_await socket.async_read_some(asio::buffer(data), use_awaitable);
      data[n] = '\0';
      std::stringstream msg( data);
      std::string name;
      uint16_t version;

      msg >> name >> version;

      std::cout << "[recv from (name:" << name << "    var:" << version << ")] " <<   std::endl;
      if (name != "heee" || version != 90) {
          socket.close();
          co_return;
      }
      

      std::string se = "iooo 66";
      co_await async_write(socket, asio::buffer(se.c_str()  , se.length()), use_awaitable);


     ssl_socket secure_socket(std::move(socket), ssl_context);

     co_await secure_socket.async_handshake(asio::ssl::stream_base::server, use_awaitable);
     const auto& sock = secure_socket.next_layer();
     std::string s = sock.remote_endpoint().address().to_string();
     uint16_t port = sock.remote_endpoint().port();

    for (;;)
    {
      
     // co_await socket.async_handshake();


      std::size_t n = co_await secure_socket.async_read_some(asio::buffer(data), use_awaitable);
      data[n] = '\0';
      std::string msg =  data;
      
      // std::string s = socket.remote_endpoint().address().to_string();
      std::cout << "[recv from (" << s << ':' << port << ")] " << msg << std::endl;

      co_await async_write(secure_socket, asio::buffer(data, n), use_awaitable);
    }
  }
  catch (std::exception& e)
  {
      // connection close
    std::printf("echo Exception: %s\n", e.what());
  }
}

std::string password_callback(
    std::size_t max_length,
    asio::ssl::context::password_purpose purpose)
{
    return "2252";
}

awaitable<void> listener()
{
  auto executor = co_await this_coro::executor;

  asio::ssl::context ssl_context(asio::ssl::context::tls);
    try{
      // ssl options  2252 
      ssl_context.set_options(
          asio::ssl::context::default_workarounds
          | asio::ssl::context::no_sslv2
          | asio::ssl::context::single_dh_use);
      ssl_context.set_password_callback(password_callback);
      ssl_context.use_certificate_chain_file(CERT_PATH);
      ssl_context.use_private_key_file(KEY_PATH, asio::ssl::context::pem);
      // ssl_context.use_tmp_dh_file("res/dh4096.pem");
    }
    catch (std::exception& e)
    {
        std::cout << "[Ssl error]: " << e.what() << std::endl;
    }

  std::cout << "[info] " << "ready to accept" << std::endl;

  tcp::acceptor acceptor(executor, {tcp::v4(), 55555});
  for (;;)
  {
    tcp::socket socket = co_await acceptor.async_accept(use_awaitable);

   // std::string s =  .remote_endpoint().address().to_string();
    // std::cout << "[connected] " << s << std::endl;
     
    co_spawn(executor, echo(std::move(socket), ssl_context), detached);
  }
}

int main()
{

  try
  {

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
