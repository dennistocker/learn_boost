#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <functional>
#include <utility>
#include <iostream>

int main()
{
    using namespace boost::interprocess;

    struct shm_remove {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    managed_shared_memory segment(create_only, "MySharedMemory", 65536);

    using KeyType = int;
    using MappedType = float;
    using valueType = std::pair<const int, float>;
    using ShmemAllocator = allocator<valueType, managed_shared_memory::segment_manager>;
    using MyMap = map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator>;

    ShmemAllocator alloc_inst(segment.get_segment_manager());

    MyMap *mymap = segment.construct<MyMap>("MyMap")(std::less<int>(), alloc_inst);
    for (int i = 0; i < 100; ++i) {
        mymap->insert(std::pair<const int, float>(i, (float)i));
    }

    return 0;
}
