#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "shared_data.hpp"

using namespace boost::interprocess;

int main(int argc, char **argv)
{
    struct shm_remove {
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    shared_memory_object shm(open_only, "MySharedMemory", read_write);
    shm.truncate(sizeof(shared_memory_buffer));
    mapped_region region(shm, read_write);
    void *addr = region.get_address();
    shared_memory_buffer *data = static_cast<shared_memory_buffer *>(addr);

    static constexpr int NumMsg = 100;

    int extracted_data[NumMsg];

    for (int i = 0; i < NumMsg; ++i) {
        data->nstored.wait();
        data->mutex.wait();
        extracted_data[i] = data->items[i % NumItems];
        data->mutex.post();
        data->nempty.post();
    }
    return 0;
}
