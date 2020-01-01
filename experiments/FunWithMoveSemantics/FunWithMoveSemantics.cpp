#include <memory>
#include <vector>
#include <iostream>

using namespace std;

struct pattern { virtual ~pattern() = default; };
struct count_pattern : public pattern {};

struct run
{
    unique_ptr<pattern> pat;
    size_t repetitions;

};

vector<run> execution_plan()
{
    vector<run> ret;
    run x = run { unique_ptr<pattern>(new count_pattern), 1 };

    // error C2280: 'run::run(const run &)': attempting to reference a deleted function
    //ret.emplace_back(x);

    // Succeeds:
    ret.emplace_back(std::move(x));

    // error C2280: 'run::run(const run &)': attempting to reference a deleted function
    //ret.push_back(x);

    // Succeeds:
    ret.push_back(std::move(x));

    // Succeeds:
    ret.push_back({ unique_ptr<pattern>(new count_pattern), 1 });

    // Succeeds:
    ret.push_back(run{ unique_ptr<pattern>(new count_pattern), 1 });

    return ret;
}

int main()
{
    vector<run> runs = execution_plan();
    cout << "This was fun!" << endl;
    return 0;
}
