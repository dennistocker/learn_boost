#include <boost/asio/io_context.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <ctime>

using namespace boost::asio;
using namespace boost::asio::ip;

static io_context context;
static tcp::endpoint endpoint{tcp::v4(), 2014};
static tcp::acceptor acceptor{context, endpoint};
tcp::socket s{context};
std::string data;

void write_handler(const boost::system::error_code &ec, std::size_t bytes_transfered)
{
    if (!ec) {
        s.shutdown(tcp::socket::shutdown_send);
    }
}

void accept_handler(const boost::system::error_code &ec)
{
    if (!ec) {
        std::time_t now = std::time(nullptr);
        data = std::ctime(&now);
        async_write(s, buffer(data), write_handler);
    }
}

int main(int argc, char **argv)
{
    acceptor.listen();
    acceptor.async_accept(s, accept_handler);
    context.run();

    return 0;
}


