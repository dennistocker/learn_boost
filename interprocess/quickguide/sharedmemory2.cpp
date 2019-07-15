#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> // std::system
#include <cstddef>
#include <cassert>
#include <utility>
#include <iostream>

int main(int argc, char **argv)
{
    using namespace boost::interprocess;
    typedef std::pair<double, int> MyType;

    if (argc == 1) {
        struct shm_remove {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        managed_shared_memory segment(create_only, "MySharedMemory", 65536);

        MyType *instance = segment.construct<MyType>
            ("MyType instance") // name of the object
            (0.0, 0);        // ctor first argument

        MyType *array = segment.construct<MyType>
            ("MyType array") // name of the object
            [10]
            (0.0, 0);

        float float_initializer[3] = {0.0, 1.0, 2.0};
        int int_initializer[3] = {0, 1, 2};

        MyType *array_it = segment.construct_it<MyType>
            ("MyType array from it")
            [3]
            ( &float_initializer[0],  &int_initializer[0] );

        // launch child process
        std::string s(argv[0]);
        s += " child";
        if (0 != std::system(s.c_str())) {
            return 1;
        }

        if (segment.find<MyType>("MyType instance").first ||
                segment.find<MyType>("MyType array").first ||
                segment.find<MyType>("MyType array from it").first) {
            return 1;
        }
    } else {
        managed_shared_memory segment(open_only, "MySharedMemory");
        std::pair<MyType *, managed_shared_memory::size_type> res;

        res = segment.find<MyType>("MyType instance");
        if (res.second != 1) return 1;
        std::cout << "instance float: " << res.first->first << std::endl;

        res = segment.find<MyType>("MyType array");
        if (res.second != 10) return 1;
        std::cout << "array size second float: " <<  (res.first + 1)->first << std::endl;

        res = segment.find<MyType>("MyType array from it");
        if (res.second != 3) return 1;
        std::cout << "array from it third float: " << (res.first + 2)->first << std::endl;

        segment.destroy<MyType>("MyType instance");
        segment.destroy<MyType>("MyType array");
        segment.destroy<MyType>("MyType array from it");
    }

    return 0;
}

