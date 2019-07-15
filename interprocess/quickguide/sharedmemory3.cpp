#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <iostream>

using namespace boost::interprocess;

struct list_node {
    offset_ptr<list_node> next;
    int value;
};

int main()
{
    struct shm_remove {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    managed_shared_memory segment(create_only, "MySharedMemory", 65535);

    offset_ptr<list_node> prev = 0, current, first;

    for (int i = 0; i < 10; ++i, prev = current) {
        current = static_cast<list_node *>(segment.allocate(sizeof(list_node)));
        current->value = i;
        current->next = 0;

        if (!prev) {
            first = current;
        } else {
            prev->next = current;
        }
    }

    for (current = first; current;) {
        prev = current;
        current = current->next;
        std::cout << prev->value << std::endl;
        segment.deallocate(prev.get());
    }

    return 0;
}
