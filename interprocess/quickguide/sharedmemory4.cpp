#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace boost::interprocess;

using ShmemAllocator = allocator<int, managed_shared_memory::segment_manager>;
using MyVector = vector<int, ShmemAllocator>;

int main(int argc, char **argv)
{
    if (argc == 1) {
        struct shm_remove {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        managed_shared_memory segment(create_only, "MySharedMemory", 65536);
        const ShmemAllocator alloc_inst(segment.get_segment_manager());
        MyVector *myvector = segment.construct<MyVector>("MyVector")(alloc_inst);

        for (int i = 0; i < 100; ++i) {
            myvector->emplace_back(i);
        }

        std::string s(argv[0]);
        s += " child";
        if (0 != std::system(s.c_str())) {
            return 1;
        }

        if (segment.find<MyVector>("MyVector").first) {
            return 1;
        }
    } else {
        managed_shared_memory segment(open_only, "MySharedMemory");
        MyVector *myvector = segment.find<MyVector>("MyVector").first;
        std::sort(myvector->rbegin(), myvector->rend());
        for (const auto&i : *myvector) {
            std::cout << i << std::endl;
        }

        segment.destroy<MyVector>("MyVector");
    }
    return 0;
}

