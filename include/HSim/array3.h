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

        Array3(const Array3<T> &array3_)
            : Array<T>(array3_), _size(array3_._size)
        {
        }

        Array3(Vec3i size)
            : _size(size)
        {

            _data.resize(size.x * size.y * size.z);
        }

        Array3(size_t x, size_t y, size_t z)
            : _size({x, y, z})
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

        T &dataAt(size_t i, size_t j, size_t k)
        {
            assert(i < _size.x && j < _size.y && k < _size.z);
            return _data[i + j * sizeX() + k * sizeX() * sizeY()];
        }

        T &operator()(size_t i)
        {
            return _data[i];
        }

        Size3 size() { return _size; }
        size_t sizeX() { return _size.x; }
        size_t sizeY() { return _size.y; }
        size_t sizeZ() { return _size.z; }

        void resize(size_t x, size_t y, size_t z)
        {
            _size = {x, y, z};
            _data.resize(_size.x * _size.y * _size.z);
        }

        void resize(const Size3& other)
        {
            _size = other;
            _data.resize(_size.x * _size.y * _size.z);
        }


        void clear()
        {
            _size.set(0);
            _data.clear();
        }

        void parallelForEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            tbb::parallel_for(tbb::blocked_range3d<size_t>(0, _size.x, 0, _size.y, 0, _size.z),
                              [&](tbb::blocked_range3d<size_t> r)
                              {
                                  for (size_t k = r.pages().begin(); k < r.pages().end(); k++)
                                  {
                                      for (size_t j = r.cols().begin(); j < r.cols().end(); j++)
                                      {
                                          for (size_t i = r.rows().begin(); i < r.rows().end(); i++)
                                          {
                                              func(i, j, k);
                                          }
                                      }
                                  }
                              });
        }

    public:
        Size3 _size;
    };

} // namespace HSim
