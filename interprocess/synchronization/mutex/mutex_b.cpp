#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "shared_log.hpp"
#include <iostream>
#include <cstdio>

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    struct shm_remove {
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    shared_memory_object shm(open_only, "MySharedMemory", read_write);
    mapped_region region(shm, read_write);

    void *addr = region.get_address();
    shared_memory_log *data = static_cast<shared_memory_log *>(addr);

    for (int i = 0; i < 100; ++i) {
        scoped_lock<interprocess_mutex> lock(data->mutex);
        std::sprintf(data->items[(data->current_line++) % NumItems], 
                "%s_%d", "process_b", i);
        if (i == NumItems - 1) {
            data->end_b = true;
        }
    }

    while (1) {
        scoped_lock<interprocess_mutex> lock(data->mutex);
        if (data->end_a) {
            break;
        }
    }

    return 0;
}
