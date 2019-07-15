#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

static constexpr int LineSize = 100;

struct trace_queue {
    trace_queue(): message_in(false) {}

    boost::interprocess::interprocess_mutex mutext;
    boost::interprocess::interprocess_condition cond_empty;
    boost::interprocess::interprocess_condition cond_full;

    char items[LineSize];
    bool message_in;
};
