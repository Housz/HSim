#pragma once 

#include <HSim/vector_grid3.h>
#include <HSim/array3.h>

namespace HSim
{
	template <typename T>
    class FaceCenterGrid3 : public VectorGrid3<T>
    {
    public:
        FaceCenterGrid3() {}
        ~FaceCenterGrid3() {}

        FaceCenterGrid3(size_t, size_t, size_t);
		FaceCenterGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1});


	public:
        Array3<T>& dataU() { return _dataU; }
        Array3<T>& dataV() { return _dataV; }
        Array3<T>& dataW() { return _dataW; }

        Vec3i& dataSizeU() { return _dataU.size(); }
        Vec3i& dataSizeV() { return _dataV.size(); }
        Vec3i& dataSizeW() { return _dataW.size(); }

        Vec3<T>& dataOriginU() { return_dataOriginU; }
        Vec3<T>& dataOriginV() { return_dataOriginV; }
        Vec3<T>& dataOriginW() { return_dataOriginW; }

        // data 
    public:
        Array3<T> _dataU;
        Array3<T> _dataV;
        Array3<T> _dataW;
        
        Vec3<T> _dataOriginU;
        Vec3<T> _dataOriginV;
        Vec3<T> _dataOriginW;

    };

    template <typename T>
    FaceCenterGrid3<T>::FaceCenterGrid3(size_t x, size_t y, size_t z)
    :VectorGrid3<T>(x, y, z)
    {
    }

    template <typename T>
	FaceCenterGrid3<T>::FaceCenterGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1})
	:VectorGrid3<T>(resolution, origin, gridSpacing)
	{
	}

	

} // namespace HSim
