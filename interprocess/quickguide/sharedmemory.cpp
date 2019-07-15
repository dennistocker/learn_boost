#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> // std::system
#include <sstream>
#include <iostream>
#include <strings.h>

int main(int argc, char **argv)
{
    using namespace boost::interprocess;

    if (argc == 1) {
        struct shm_remove {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        managed_shared_memory segment(create_only, "MySharedMemory", 65536);

        managed_shared_memory::size_type free_memory = segment.get_free_memory();
        void *shptr = segment.allocate(1024);
        strcpy((char *)shptr, "hello");

        // check invariant
        if (free_memory <= segment.get_free_memory()) {
            return 1;
        }

        managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
        std::stringstream s;
        s << argv[0] << " " << handle;
        s << std::ends;

        // std::cout << s.str() << std::endl;

        // launch child process
        if (0 != std::system(s.str().c_str())) {
            return 1;
        }

        // check memory  has been freed
        if (free_memory != segment.get_free_memory()) {
            return 1;
        }
    } else {
        managed_shared_memory segment(open_only, "MySharedMemory");

        managed_shared_memory::handle_t handle = 0;

        std::stringstream s;
        s << argv[1];
        s >> handle;

        std::cout << handle << std::endl;

        void *msg = segment.get_address_from_handle(handle);
        std::cout << std::string((char *)msg) << std::endl;

        segment.deallocate(msg);
    }

    return 0;
}
