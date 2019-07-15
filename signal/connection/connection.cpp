#include <boost/signals2.hpp>
#include <iostream>

struct HelloWorld {
    void operator()() const {
        std::cout << "Hello, World!" << std::endl;
    }
};

int main(int argc, char **argv)
{
    // connection
    boost::signals2::signal<void()> sig;
    boost::signals2::connection c = sig.connect(HelloWorld());
    std::cout << "c is connected" << std::endl;
    sig();

    c.disconnect();
    std::cout << "c is disconnected" << std::endl;
    sig(); // does nothing


    // block
    c = sig.connect(HelloWorld());
    std::cout << "c is not blocked" << std::endl;
    sig();
    {
        // temporarily blocked
        boost::signals2::shared_connection_block block(c);
        std::cout << "c is blocked" << std::endl;
        sig();
        // block.unblock();
    }
    std::cout << "c is not blocked" << std::endl;
    sig();
    c.disconnect();

    // scoped connections
    {
        boost::signals2::scoped_connection c(sig.connect(HelloWorld()));
        std::cout << "c is connected" << std::endl;
        sig();
    }
    std::cout << "c is not connected" << std::endl;
    sig();

    return 0;
}


