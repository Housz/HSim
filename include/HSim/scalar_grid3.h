#pragma once

#include <HSim/grid3.h>

namespace HSim
{

	template <typename T>
	class ScalarGrid3 : public Grid3<T>
	{
		// constructor
	public:
		ScalarGrid3(){};
		~ScalarGrid3(){};

		ScalarGrid3(size_t, size_t, size_t);
		ScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1});
		

		// setter getter
	public:
		void setData(std::vector<T> &sourceData);
		void getData(std::vector<T> &targetData);

		T &operator()(size_t, size_t, size_t);

		T dataAt(size_t, size_t, size_t);

		void fill(T);

		// difference operators
	public:
		Vec3<T> gradientAt(size_t, size_t, size_t);
		T laplacianAt(size_t, size_t, size_t);

		// for subclasses
	public:
		virtual Vec3i dataSize() = 0;
		virtual Vec3<T> dataOrigin() = 0;

		// data
	public:
		std::vector<T> _data;
	};

	template <typename T>
	void ScalarGrid3<T>::setData(std::vector<T> &sourceData)
	{
		this->_data = sourceData;
	}

	template <typename T>
	void ScalarGrid3<T>::getData(std::vector<T> &sourceData)
	{
		sourceData = this->_data;
	}

	template <typename T>
	T ScalarGrid3<T>::dataAt(size_t i, size_t j, size_t k)
	{
		return _data[i + j * sizeX() + k * sizeX() * sizeY()];
	}

	template <typename T>
	T &ScalarGrid3<T>::operator()(size_t i, size_t j, size_t k)
	{
		return _data[i + j * sizeX() + k * sizeX() * sizeY()];
	}

	template <typename T>
	void ScalarGrid3<T>::fill(T value)
	{
		std::fill(_data.begin(), _data.end(), value);
	}

	template <typename T>
	ScalarGrid3<T>::ScalarGrid3(size_t x, size_t y, size_t z)
	:Grid3<T>(x, y, z)
	{
	}

	template <typename T>
	ScalarGrid3<T>::ScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1})
	:Grid3<T>(resolution, origin, gridSpacing)
	{
	}

	/**
	 * @brief Gradient (Central Finite Difference)
	 * df/dx = [f(x+h) - f(x-h)]/ 2h
	 * gradient f = (df/dx, df/dy, df/dz)
	 */
	template <typename T>
	Vec3<T> ScalarGrid3<T>::gradientAt(size_t i, size_t j, size_t k)
	{
		size_t sizeX = _gridResolution.x;
		size_t sizeY = _gridResolution.y;
		size_t sizeZ = _gridResolution.z;
		assert(i < sizeX && j < sizeY && k < sizeZ);

		Vec3<T> grad;

		T left 	= dataAt((i > 0) ? i - 1 : i, j, k);
		T right = dataAt((i + 1 < sizeX) ? i + 1 : i, j, k);
		T down 	= dataAt(i, (j > 0) ? j - 1 : j, k);
		T up 	= dataAt(i, (j + 1 < sizeY) ? j + 1 : j, k);
		T back 	= dataAt(i, j, (k > 0) ? k - 1 : k);
		T front = dataAt(i, j, (k + 1 < sizeY) ? k + 1 : k);

		grad.x = (right - left) / (2 * deltaX());
		grad.y = (up 	- down) / (2 * deltaY());
		grad.z = (front - back) / (2 * deltaZ());

		return grad;
	}

	/**
	 * @brief laplacian (Second-order Central Finite Difference)
	 * d2f/dx2 = [f(x+h) - 2f(x) + f(x-h)]/ h^2
	 * laplacian f = d2f/dx2 + d2f/dy2 + d2f/dz2
	 */
	template <typename T>
	T ScalarGrid3<T>::laplacianAt(size_t i, size_t j, size_t k)
	{
		size_t sizeX = _gridResolution.x;
		size_t sizeY = _gridResolution.y;
		size_t sizeZ = _gridResolution.z;
		assert(i < sizeX && j < sizeY && k < sizeZ);

		T center= dataAt(i, j, k);

		T left 	= dataAt((i > 0) ? i - 1 : i, j, k);
		T right = dataAt((i + 1 < sizeX) ? i + 1 : i, j, k);
		T down 	= dataAt(i, (j > 0) ? j - 1 : j, k);
		T up 	= dataAt(i, (j + 1 < sizeY) ? j + 1 : j, k);
		T back 	= dataAt(i, j, (k > 0) ? k - 1 : k);
		T front = dataAt(i, j, (k + 1 < sizeY) ? k + 1 : k);
		
		// d2f/dx2
		T ddx = ( right - 2*center + left ) / ( deltaX() * deltaX());
		// d2f/dx2
		T ddy = ( up 	- 2*center + down ) / ( deltaY() * deltaY());
		// d2f/dx2
		T ddz = ( front - 2*center + back ) / ( deltaZ() * deltaZ());

		T laplacian = ddx + ddy + ddz;

		return laplacian;
		
	}

} // namespace HSim
