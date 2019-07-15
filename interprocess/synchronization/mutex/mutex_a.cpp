#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "shared_log.hpp"
#include <iostream>
#include <cstdio>

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    try {

    struct shm_remove {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    shared_memory_object shm(create_only, "MySharedMemory", read_write);
    shm.truncate(sizeof(shared_memory_log));
    std::cout << "size: " << sizeof(shared_memory_log) << std::endl;

    mapped_region region(shm, read_write);
    void *addr = region.get_address();
    shared_memory_log *data = new (addr) shared_memory_log;

    for (int i = 0; i < NumItems; ++i) {
        scoped_lock<interprocess_mutex> lock(data->mutex);
        std::sprintf(data->items[(data->current_line++) % NumItems], "%s_%d", "process_a", i);
        if (i == NumItems - 1) {
            data->end_a = true;
        }
    }

    while (1) {
        scoped_lock<interprocess_mutex> lock(data->mutex);
        if (data->end_b) {
            break;
        }
    }

    for (int i = 0; i < NumItems; ++i) {
        std::cout << data->items[i] << std::endl;
    }

    } catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

