#pragma once 

#include <HSim/common.h>
#include <HSim/vec3.h>


namespace HSim
{
    /**
     * @brief 3D cartesian grid base class
     * 
     * @tparam T: float or double
     */
    template<typename T>
    class Grid3
    {
    public:
        Grid3() {}
        ~Grid3() {}
    
    public:
        virtual Vec3<T> origin();
        // virtual Vec3<T> 

    };

} // namespace HSim
