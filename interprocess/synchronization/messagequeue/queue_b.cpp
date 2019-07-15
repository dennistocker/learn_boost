#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    try {
        message_queue mq(open_only, "message_queue");

        unsigned int priority;
        message_queue::size_type recv_size;

        for (int i = 0; i < 100; ++i) {
            int number;
            mq.receive(&number, sizeof(number), recv_size, priority);
            std::cout << priority << ": "  << number << std::endl;
            if (number != i || recv_size != sizeof(number)) {
                return 1;
            }
        }
    } catch(interprocess_exception &ex) {
        message_queue::remove("message_queue");
        std::cout << ex.what() << std::endl;
        return 1;
    }
    message_queue::remove("message_queue");
    return 0;
}
