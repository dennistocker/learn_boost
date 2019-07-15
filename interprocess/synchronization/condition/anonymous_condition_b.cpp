#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "shared_data.hpp"

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    shared_memory_object shm(open_only, "MySharedMemory", read_write);
    try {
        mapped_region region(shm, read_write);

        void *addr = region.get_address();
        trace_queue *data = static_cast<trace_queue *>(addr);

        bool end_loop = false;
        do {
            scoped_lock<interprocess_mutex> lock(data->mutext);
            if (!data->message_in) {
                data->cond_empty.wait(lock);
            }
            if (std::strcmp(data->items, "last message") == 0) {
                end_loop = true;
            } else {
                std::cout << data->items << std::endl;
                data->message_in = false;
                data->cond_full.notify_one();
            }
        } while (!end_loop);
    } catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
