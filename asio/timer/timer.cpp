#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>

using namespace boost::asio;


int main(int argc, char **argv) {
    io_context io;
    steady_timer timer{io, std::chrono::seconds{3}};
    timer.wait();
    std::cout << "Hello world"<< std::endl;

    steady_timer timer2{io, std::chrono::seconds{3}};
    timer2.async_wait([](const boost::system::error_code &ec){
        std::cout << "3 sec main thread\n" << std::endl;
    });
    io.run();

    io_context io1;
    io_context io2;
    steady_timer timer11{io1, std::chrono::seconds{3}};
    timer11.async_wait([](const boost::system::error_code &ec) {
            std::cout<<"3 sec thread1 \n"<<std::endl;
    });
    steady_timer timer12{io2, std::chrono::seconds{3}};
    timer12.async_wait([](const boost::system::error_code &ec) {
            std::cout<<"3 sec thread2\n"<<std::endl;
    });

    std::thread thread1{[&]() { io1.run(); }};
    std::thread thread2{[&]() { io2.run(); }};
    thread1.join();
    thread2.join();

    return 0;
}
