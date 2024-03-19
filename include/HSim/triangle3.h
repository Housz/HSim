#pragma once 

#include <HSim/common.h>
#include <HSim/vec2.h>
#include <HSim/vec3.h>
#include <HSim/transform3.h>
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
        T area() const
        {
            
        }


    // data
    public:
        // Transform3<T> transform Inherited from Surface3;

		std::array<Vec3<T>, 3> points;
		std::array<Vec3<T>, 3> normals;
		std::array<Vec2<T>, 3> uvs;

    // for rendering
    public:

        
    };

    template <typename T>
	using Triangle3_Ptr = std::shared_ptr<Triangle3<T>>;
    
    

} // namespace HSim
