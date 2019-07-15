#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "shared_data.hpp"

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    struct shm_remove {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    shared_memory_object shm(create_only, "MySharedMemory", read_write);
    shm.truncate(sizeof(shared_memory_buffer));
    mapped_region region(shm, read_write);
    void *addr = region.get_address();
    shared_memory_buffer *data = new (addr) shared_memory_buffer;

    static constexpr int NumMsg = 100;

    for (int i = 0; i < NumMsg; ++i) {
        data->nempty.wait();
        data->mutex.wait();
        data->items[i % NumItems] = i;
        data->mutex.post();
        data->nstored.post();
    }
    return 0;
}
