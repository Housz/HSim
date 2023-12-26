#pragma once 

#include <HSim/array.h>
#include <HSim/vec2.h>

namespace HSim
{
	/**
	 * @brief 
	 * 
	 * @tparam T 
	 */
	template<typename T>
	class Array2 : public Array<T>
	{
	public:
		Array2() {}
		~Array2() {}

		Array2(Vec2i size)
        {
            _size = size;
            _data.resize(size.x * size.y);
        }

		Array2(size_t x, size_t y)
        {
            _size = {x, y};
            _data.resize(x * y);
        }

		// setter, getter
	public:
        T& operator()(size_t i, size_t j)
        {
            return dataAt(i, j);
        }

		T& dataAt(size_t i, size_t j)
        {
            assert( i<_size.x && j<_size.y);
            return _data[i + j * sizeX()];
        }

		T& operator()(size_t i)
        {
            return _data[i];
        }

        Size2 size() { return _size; }
        size_t sizeX() { return _size.x; }
        size_t sizeY() { return _size.y; }

        void resize(size_t x, size_t y)
        {   
            _size = {x, y};
            _data.resize(_size.x * _size.y);
        }

		void clear()
        {
            _size.set(0);
            _data.clear();
        }

    public:
        Size2 _size;
	};
	
} // namespace HSim
