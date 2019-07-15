#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cstdio>
#include "shared_data.hpp"

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    struct shm_remove {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    shared_memory_object shm(create_only, "MySharedMemory", read_write);
    try {
        shm.truncate(sizeof(trace_queue));
        mapped_region region(shm, read_write);

        void *addr = region.get_address();
        trace_queue *data = new (addr) trace_queue;

        constexpr int NumMsg = 100;
        for (int i = 0; i < NumMsg; ++i) {
            scoped_lock<interprocess_mutex> lock(data->mutext);
            if (data->message_in) {
                data->cond_full.wait(lock);
            }
            if (i == NumMsg - 1) {
                std::sprintf(data->items, "%s", "last message");
            } else {
                std::sprintf(data->items, "%s_%d", "my_trace", i);
            }

            data->cond_empty.notify_one();

            data->message_in = true;
        }
    } catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
