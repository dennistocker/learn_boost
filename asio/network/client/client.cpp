#include <boost/asio/io_context.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

static io_context context;
static tcp::resolver resolver{context};
static tcp::socket tcp_socket{context};
static std::array<char, 4096> bytes;

void read_handler(const boost::system::error_code &ec,
        std::size_t bytes_transfered)
{
    if (!ec) {
        std::cout.write(bytes.data(), bytes_transfered);
        tcp_socket.async_read_some(buffer(bytes), read_handler);
    }
}

void connect_handler(const boost::system::error_code &ec)
{
    if (!ec) {
        std::string r = "GET / HTTP/1.1\r\nHost: theboostcpplibraries.com\r\n\r\n";
        write(tcp_socket, buffer(r));
        tcp_socket.async_read_some(buffer(bytes), read_handler);
    }
}

void resolve_handler(const boost::system::error_code &ec,
        tcp::resolver::iterator it)
{
    if (!ec) {
        tcp_socket.async_connect(*it, connect_handler);
    }
}

int main()
{
    tcp::resolver::query q{"theboostcpplibraries.com", "80"};
    resolver.async_resolve(q, resolve_handler);
    context.run();

    return 0;
}
