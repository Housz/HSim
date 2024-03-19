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

        Triangle3(const Transform3<T>& transform_) : Surface3(transform_) {}

        Triangle3(
            const std::array<Vec3<T>, 3>& points_, 
            const std::array<Vec3<T>, 3>& normals_,
            const std::array<Vec2<T>, 3>& uvs_,
            const Transform3<T>& transform_
            ) : points(points_), normals(normals_), uvs(uvs_), Surface3(transform_) {};
        
        template<typename U>
        Triangle3(const Triangle3<U> triangle_) 
        : points(triangle_.points), 
        normals(triangle_.normals), 
        uvs(triangle_.uvs),
        Surface3(triangle_.transform) {}

        ~Triangle3() {};
    

    public:
        T area() const
        {
            return 0.5 * (points[1] - points[0]).cross(points[2] - points[0]).length();
        }

        Vec3<T> faceNormal() const
        {
            return (points[1] - points[0]).cross(points[2] - points[0]).getNormalized();
        }

    // in local frame
    public:
        Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
        {
            auto A = points[0];
            auto B = points[1];
            auto C = points[2];

            /**
             *  closest distance d from point P to triangle ABC:
             *  d = (P - A) * N, where N is the normalized face normal of ABC
             */
            auto N = faceNormal();
            auto P = positionInLocal_;

            auto d = (P - A).dot(N);
            // std::cout << "closestDistance: " << d << std::endl;

            /**
             *  The projection point projP of P on the plane of triangle ABC is
             *  projP = P - d * N
             */
            auto projP = P - d * N;
            // std::cout << "projection: " << projP << std::endl;


            /**
             *  closest position closestP to triangle ABC
             * 
             *  1. projP is outside the triangle ABC
             * 
             *  ab = (B - A) x (projP - A)
             *      if ab < 0, closestP = closestPointOnLine(A, B, projP)
             *  bc = (C - B) x (projP - B)
             *      if bc < 0, closestP = closestPointOnLine(B, C, projP)
             *  ca = (A - C) x (projP - A)
             *      if ca < 0, closestP = closestPointOnLine(C, A, projP)
             *  
             *  2. projP is inside the triangle ABC
             *  closestP = projP
             */

            auto ab = (B - A).cross(projP - A);
            if (ab < EPSILON)
            {
                // return 
            }
            


            return {0, 0, 0};
        }

        Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
        {
            return {0, 0, 0};
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
