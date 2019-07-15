#include <boost/interprocess/sync/interprocess_semaphore.hpp>

static constexpr int NumItems = 10;

struct shared_memory_buffer {
    shared_memory_buffer(): mutex(1), nempty(NumItems), nstored(0) {}

    boost::interprocess::interprocess_semaphore mutex, nempty, nstored;

    int items[NumItems];
};
