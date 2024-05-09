#pragma once

#include <HSim/common.h>

namespace HSim
{
    /**
     * @brief 2D/3D array base class
     * 
     * @tparam T 
     */
    template <typename T>
    class Array
    {
    public:
        Array() {}

        Array(const Array<T>& array_)
            : _data(array_._data)
        {
        }

        ~Array() {}

    public:
        void setData(std::vector<T> data) { _data = data; }
        std::vector<T> getData() { return _data; }

        void fill(T value) { std::fill(_data.begin(), _data.end(), value); }

    public:
        std::vector<T> _data;

    };

} // namespace HSim
