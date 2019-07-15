#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    try {
        message_queue::remove("message_queue");

        message_queue mq(create_only, "message_queue", 100, sizeof(int));
        for (int i = 0; i < 100; ++i) {
            mq.send(&i, sizeof(i), 0);
        }
    } catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
