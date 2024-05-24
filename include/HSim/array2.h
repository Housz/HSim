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
        // todo copy
		~Array2() {}

		Array2(Vec2i size_) 
            : size(size_)
        {
            _data.resize(size.x * size.y);
        }

		Array2(size_t x, size_t y)
        {
            size = {x, y};
            _data.resize(x * y);
        }

		// setter, getter
	public:
        T& operator()(size_t i, size_t j)
        {
            return dataAt(i, j);
        }

        T operator()(size_t i, size_t j) const
        {
            return dataAt(i, j);
        }

		T& dataAt(size_t i, size_t j)
        {
            assert( i<size.x && j<size.y);
            return _data[i + j * sizeX()];
        }

		T& operator()(size_t i)
        {
            return _data[i];
        }
        
        T operator()(size_t i) const
        {
            return _data[i];
        }

        // Size2 size() { return size; }
        size_t sizeX() const { return size.x; }
        size_t sizeY() const { return size.y; }

        void resize(size_t x, size_t y)
        {   
            size = {x, y};
            _data.resize(size.x * size.y);
        }

		void clear()
        {
            size.set(0);
            _data.clear();
        }

    public:
        Size2 size;
	};
	
} // namespace HSim
