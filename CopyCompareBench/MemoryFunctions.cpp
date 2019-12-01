#include <algorithm>
#include "MemoryFunctions.h"
#include <functional>

using namespace std;

namespace MemLoader {
    vector<unsigned int>::const_iterator clamp_to_end(const vector<unsigned int> &source, const vector<unsigned int> &destination, size_t offset)
    {
        return source.begin() + min(source.size(), destination.size() - offset);
    }

    void std_copy(const vector<unsigned int> &source, vector<unsigned int> &destination, size_t offset)
    {
        const vector<unsigned int>::const_iterator source_end =
            clamp_to_end(source, destination, offset);

        copy(source.begin(), source_end, destination.begin() + offset);
    }

    void manual_copy(const vector<unsigned int> &source, vector<unsigned int> &destination, size_t offset)
    {
        size_t position = offset;
        for (auto i = source.begin(); i != source.end() && position < destination.size(); ++i)
        {
            destination[position++] = *i;
        }
    }

    void mech_copy(const vector<unsigned int> &source, vector<unsigned int> &destination, size_t offset)
    {
        memcpy(
            destination.data() + offset,
            source.data(),
            min(
                source.size(),
                destination.size() - offset) * sizeof(unsigned int));
    }

    bool std_equals(const vector<unsigned int> &source, const vector<unsigned int> &destination, size_t offset)
    {
        const vector<unsigned int>::const_iterator source_end =
            clamp_to_end(source, destination, offset);

        return equal(source.begin(), source_end, destination.begin() + offset);
    }

    bool manual_equals(const vector<unsigned int> &source, const vector<unsigned int> &destination, size_t offset)
    {
        size_t position = offset;
        for (auto i = source.begin(); i != source.end() && position < destination.size(); ++i)
        {
            if (destination[position++] != *i)
            {
                return false;
            }
        }

        return true;
    }

    bool mech_equals(const vector<unsigned int> &source, const vector<unsigned int> &destination, size_t offset)
    {
        return memcmp(
            source.data(),
            destination.data() + offset,
            min(
                source.size(), 
                destination.size() - offset) * sizeof(unsigned int)) != 0;
    }

    void pad(
        const vector<unsigned int>& source,
        vector<unsigned int>& destination,
        function<void(const vector<unsigned int>&, vector<unsigned int>&, size_t)> copy_function)
    {
        for (size_t i = 0; i + source.size() <= destination.size(); i += source.size())
        {
            copy_function(source, destination, i);
        }
    }
}
