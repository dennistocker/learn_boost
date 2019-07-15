#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>

int main(int argc, char **argv)
{
    using namespace boost::interprocess;

    const char *filename = "file.bin";
    const std::size_t filesize = 10000;

    size_t page_size = mapped_region::get_page_size();
    std::cout << "page size: " << page_size << std::endl;

    if (argc == 1) {
        file_mapping::remove(filename);
        std::filebuf fbuf;
        fbuf.open(filename, std::ios_base::in |
                std::ios_base::out |
                std::ios_base::trunc |
                std::ios_base::binary);
        fbuf.pubseekoff(filesize-1, std::ios_base::beg);
        fbuf.sputc(0);
        fbuf.pubsync();

        struct file_remove {
            file_remove(const char *filename) : _filename(filename) {}
            ~file_remove(){ file_mapping::remove(_filename); }
            const char *_filename;
        } remover(filename);

        file_mapping m_file(filename, read_write);
        mapped_region region(m_file, read_write);

        void *addr = region.get_address();
        std::size_t size = region.get_size();
        std::cout << "addr: " << addr <<  " size: " << size << std::endl;
        std::memset(addr, 1, size);

        std::string s(argv[0]);
        s += " child";
        if (0 != std::system(s.c_str())) {
            return 1;
        }
    } else {
        file_mapping m_file(filename, read_only);
        mapped_region region(m_file, read_only);

        void *addr = region.get_address();
        size_t size = region.get_size();
        std::cout << "addr: " << addr <<  " size: " << size << std::endl;
        const char *mem = static_cast<char *>(addr);
        for (size_t i = 0; i < size; ++i) {
            if (*mem++ != 1) {
                return 1;
            }
        }

        std::filebuf fbuf;
        fbuf.open(filename, std::ios_base::in | std::ios_base::binary);
        std::vector<char> vec(filesize, 0);
        fbuf.sgetn(&vec[0], std::streamsize(vec.size()));

        const char *fmem = static_cast<char *>(&vec[0]);
        for (size_t i = 0; i < filesize; ++i) {
            if (*fmem++ != 1) {
                return 1;
            }
        }
    }

    return 0;
} 

