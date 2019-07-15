#include <boost/signals2.hpp>
#include <iostream>

struct Hello {
    void operator()() const
    {
        std::cout << "Hello" ;
    }
};

struct World {
    void operator()() const
    {
        std::cout << ", World" << std::endl;
    }
};

void print_args(float x, float y)
{
    std::cout << "The arguments are " << x << " and "  << y << std::endl;
}

void print_sum(float x, float y)
{
    std::cout << "The sum is " << x + y << std::endl;
}

template<typename T>
struct walker {
    typedef void result_type; // needed
    template<typename InputIterator>
    void operator()(InputIterator first, InputIterator last) const {
        if (first == last) return;
        while (first != last) {
            std::cout << *first << std::endl;
            ++first;
        }
    }
};

int main(int argc, char **argv)
{
    boost::signals2::signal<void ()> sig;

    //
    sig.connect(Hello());
    sig.connect(World());

    // group
    sig.connect(1, World());
    sig.connect(0, Hello());

    // font & back
    std::function<void ()> begin = [](){std::cout << "Begin" << std::endl;};
    sig.connect(begin, boost::signals2::at_front);
    sig.connect([](){std::cout << "End" << std::endl;}, boost::signals2::at_back);

    sig();
    sig.disconnect_all_slots();

    // arguments
    boost::signals2::signal<void(float, float)> sig2;
    sig2.connect(print_args);
    sig2.connect(print_sum);
    sig2(5.0, 3.0);
    sig2.disconnect_all_slots();

    // return value
    boost::signals2::signal<float(float, float), walker<float>> sig3;
    sig3.connect([](float x, float y){ return x + y; });
    sig3.connect([](float x, float y){ return x - y; });
    sig3.connect([](float x, float y){ return x * y; });
    sig3.connect([](float x, float y){ return x / y; });
    sig3(10, 2);

    return 0;
}


