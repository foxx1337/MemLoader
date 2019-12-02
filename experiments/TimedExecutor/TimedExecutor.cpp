#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

class auto_timer
{
public:
    auto_timer(std::chrono::high_resolution_clock::duration &duration) :
        start(std::chrono::high_resolution_clock::now()),
        duration(duration)
    {
        // nothing to do here
    }

    ~auto_timer()
    {
        const std::chrono::steady_clock::time_point stop = std::chrono::high_resolution_clock::now();
        duration = stop - start;
    }

private:
    const std::chrono::steady_clock::time_point start;
    std::chrono::high_resolution_clock::duration& duration;
};

class timed_executor
{
public:
    template <class _To, class T>
    static _To run_timed(T proc)
    {
        std::cout << "template version" << std::endl;
        const timed_executor executor(proc);
        return std::chrono::duration_cast<_To>(executor.duration);
    }

    template <class _To>
    static _To run_timed(std::function<void()> proc)
    {
        std::cout << "functional version" << std::endl;
        const timed_executor executor(proc);
        return std::chrono::duration_cast<_To>(executor.duration);
    }

private:
    std::chrono::high_resolution_clock::duration duration;

    template <class T>
    timed_executor(T proc) : duration(0)
    {
        auto_timer the_timer(this->duration);
        proc();
    }
};

int main()
{
    std::cout << "Hello World!\n";
    const auto fun = []()
    {
        std::cout << "from the lambda" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1230));
    };

    std::cout << "with class qualifier: " << timed_executor::run_timed<std::chrono::microseconds>(fun).count() << std::endl;
}
