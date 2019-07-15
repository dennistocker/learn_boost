#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
    using namespace boost::interprocess;

    if (argc == 1) {
        struct shm_remove {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        shared_memory_object shm(create_only, "MySharedMemory", read_write);
        shm.truncate(100);

        mapped_region region(shm, read_write);
        std::memset(region.get_address(), 1, region.get_size());

        std::string s(argv[0]);
        s += " child";
        if (0 != std::system(s.c_str())) {
            return 1;
        }
    } else {
        shared_memory_object shm(open_only, "MySharedMemory", read_only);
        mapped_region region(shm, read_only);
        char *mem = static_cast<char *>(region.get_address());
        for (std::size_t i = 0; i < region.get_size(); ++i) {
            std::cout << (int)(*mem) << std::endl;
            if (*mem++ != 1) {
                return 1;
            }
        }
    }
    return 0;
}


