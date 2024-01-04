#pragma once 

#include <HSim/surface3.h>

namespace HSim
{
    template <typename T>
    class Triangle3 : public Surface3<T>
    {
    public:
        Triangle3() {};
        ~Triangle3() {};
    
    public:
        // points

        // normals

        // uvs
        
    };

    template <typename T>
	using Triangle3_Ptr = std::shared_ptr<Triangle3<T>>;
    
    

} // namespace HSim
