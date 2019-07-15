#include <boost/interprocess/sync/interprocess_mutex.hpp>

static constexpr int NumItems = 100;
static constexpr int LineSize = 100;

struct shared_memory_log {

    shared_memory_log()
        : current_line(0)
          , end_a(false)
          , end_b(false)
    {}

    boost::interprocess::interprocess_mutex mutex;

    char items[NumItems][LineSize];
    int current_line;
    bool end_a;
    bool end_b;
};
