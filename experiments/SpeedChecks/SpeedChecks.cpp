#include <iostream>
#include <thread>
#include <chrono>

class auto_timer
{
private:
    std::chrono::steady_clock::time_point start;
public:
    auto_timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~auto_timer()
    {
        std::chrono::steady_clock::time_point stop = std::chrono::high_resolution_clock::now();
        const auto result = stop - start;
        std::cout << "Operation took " << result.count() << std::endl;
        const std::chrono::duration<double> dResult = stop - start;
        std::cout << "... and as double " << dResult.count() << std::endl;
        const std::chrono::duration<float> fResult = stop - start;
        std::cout << "... and as float " << fResult.count() << std::endl;
        const std::chrono::duration<long, std::micro> microResult = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "... and as microseconds " << microResult.count() << std::endl;
        const std::chrono::duration<long, std::nano> nanoResult = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        std::cout << "... and as nanoseconds " << nanoResult.count() << std::endl;
    }
};

void test_sleep()
{
    using namespace std::chrono_literals;
    auto_timer timer;
    std::this_thread::sleep_for(1800123000ns);
}

int main()
{
    std::cout << "Hello World!\n";
    test_sleep();
}
