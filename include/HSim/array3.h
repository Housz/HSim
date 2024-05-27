#pragma once

#include <HSim/array.h>
#include <HSim/vec3.h>

namespace HSim
{
    template <typename T>
    class Array3 : public Array<T>
    {
    public:
        Array3() {}

        Array3(const Array3<T> &other)
            : Array<T>(other), size(other.size)
        {
        }

        Array3(Vec3i size_)
            : size(size_)
        {
            _data.resize(size.x * size.y * size.z);
        }

        Array3(size_t x, size_t y, size_t z)
            : size({x, y, z})
        {
            _data.resize(x * y * z);
        }

        ~Array3() {}

        // setter, getter
    public:
        T &operator()(size_t i, size_t j, size_t k)
        {
            return dataAt(i, j, k);
        }

        T operator()(size_t i, size_t j, size_t k) const
        {
            return dataAt(i, j, k);
        }

        T &dataAt(size_t i, size_t j, size_t k)
        {
            if (!(i < size.x && j < size.y && k < size.z))
            {
                std::cout << "err " << i << " " << j << " " << k << "\n";
            }

            assert(i < size.x && j < size.y && k < size.z);
            return _data[i + j * sizeX() + k * sizeX() * sizeY()];
        }

        T dataAt(size_t i, size_t j, size_t k) const
        {

            assert(i < size.x && j < size.y && k < size.z);
            return _data[i + j * sizeX() + k * sizeX() * sizeY()];
        }

        T &operator()(size_t i)
        {
            return _data[i];
        }

        T operator()(size_t i) const
        {
            return _data[i];
        }

        // Size3 size() { return size; }
        size_t sizeX() const { return size.x; }
        size_t sizeY() const { return size.y; }
        size_t sizeZ() const { return size.z; }

        void resize(size_t x, size_t y, size_t z)
        {
            size = {x, y, z};
            _data.resize(size.x * size.y * size.z);
        }

        void resize(const Size3 &other)
        {
            size = other;
            _data.resize(size.x * size.y * size.z);
        }

        void clear()
        {
            size.set(0);
            _data.clear();
        }

        void parallelForEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            tbb::parallel_for(tbb::blocked_range3d<size_t>(0, size.x, 0, size.y, 0, size.z),
                              [&](tbb::blocked_range3d<size_t> r)
                              {
                                  for (size_t i = r.pages().begin(); i < r.pages().end(); i++)
                                  {
                                      for (size_t j = r.rows().begin(); j < r.rows().end(); j++)
                                      {
                                          for (size_t k = r.cols().begin(); k < r.cols().end(); k++)
                                          {
                                              func(i, j, k);
                                          }
                                      }
                                  }
                              });
        }

    public:
        Size3 size;
    };

} // namespace HSim
