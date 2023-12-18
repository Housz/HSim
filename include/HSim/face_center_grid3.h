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

        // data 
    public:
        Array3<T> _dataU;
        Array3<T> _dataV;
        Array3<T> _dataW;

    };
	

} // namespace HSim
