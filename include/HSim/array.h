#pragma once

#include <HSim/common.h>
#include <HSim/parallel.h>

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

        Array(const Array<T> &array_)
            : _data(array_._data)
        {
        }

        ~Array() {}

    public:
        void setData(const std::vector<T> &data) 
        { 
            _data = data;
        }

        std::vector<T> getData() 
        { 
            return _data;
        }

        void fill(T value) 
        { 
            std::fill(_data.begin(), _data.end(), value);
        }

        void parallelForEach(const std::function<void(T&)> &func)
        {
            tbb::parallel_for(tbb::blocked_range<size_t>(0, _data.size()),
            [&](tbb::blocked_range<size_t>& range) {
               for (size_t i = range.begin(); i != range.end(); i++)
               {
                    func(_data[i]);
               }
            });
        }

    public:
        std::vector<T> _data;
    };

} // namespace HSim
