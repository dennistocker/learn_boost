#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <fstream>
#include <iostream>
#include <cstdio>

int main()
{
    using namespace boost::interprocess;
    try {
        struct file_remove {
            file_remove() { std::remove("filename"); }
            ~file_remove() { std::remove("filename"); }
        } file_remover;

        struct mutex_remove {
            mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
            ~mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
        } remover;

        named_mutex mutex(open_or_create, "fstream_named_mutex");
        std::ofstream file("filename");

        for (int i = 0; i < 10; ++i) {
            scoped_lock<named_mutex> lock(mutex);
            file << "Process name: ";
            file << "This is iteration # " << i;
            file << std::endl;
        }
    } catch (interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
