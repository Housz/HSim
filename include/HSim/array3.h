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
        ~Array3() {}

        Array3(Vec3i size)
        {
            _size = size;
            _data.resize(size.x * size.y * size.z);
        }

        Array3(size_t x, size_t y, size_t z)
        {
            _size = {x, y, z};
            _data.resize(x * y * z);
        }

        // setter, getter
    public:
        T operator()(size_t i, size_t j, size_t k)
        {
            return dataAt(i, j, k);
        }

        T dataAt(size_t i, size_t j, size_t k)
        {
            return _data[i + j * sizeX() + k * sizeX() * sizeY()];
        }

        T operator()(size_t i)
        {
            return _data[i];
        }

        size_t sizeX() { return _size.x; }
        size_t sizeY() { return _size.y; }
        size_t sizeZ() { return _size.z; }

        void clear()
        {
            _size.set(0);
            _data.clear();
        }

    public:
        Vec3i _size;
    };

} // namespace HSim
