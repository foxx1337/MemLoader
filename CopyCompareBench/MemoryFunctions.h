#pragma once

#include <vector>
#include <functional>

namespace MemLoader
{
    std::vector<unsigned int>::const_iterator clamp_to_end(const std::vector<unsigned int> &source, const std::vector<unsigned int> &destination, size_t offset);
    void std_copy(const std::vector<unsigned int> &source, std::vector<unsigned int> &destination, size_t offset);
    void manual_copy(const std::vector<unsigned int> &source, std::vector<unsigned int> &destination, size_t offset);
    void mech_copy(const std::vector<unsigned int> &source, std::vector<unsigned int> &destination, size_t offset);

    bool std_equals(const std::vector<unsigned int>& source, const std::vector<unsigned int>& destination, size_t offset);
    bool manual_equals(const std::vector<unsigned int>& source, const std::vector<unsigned int>& destination, size_t offset);
    bool mech_equals(const std::vector<unsigned int>& source, const std::vector<unsigned int>& destination, size_t offset);

    void pad(
        const std::vector<unsigned int>& source,
        std::vector<unsigned int>& destination,
        std::function<void(const std::vector<unsigned int>&, std::vector<unsigned int>&, size_t)> copy_function);
}
