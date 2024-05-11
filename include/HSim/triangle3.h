#pragma once

#include <HSim/common.h>
#include <HSim/vec2.h>
#include <HSim/vec3.h>
#include <HSim/math_utils.h>
#include <HSim/transform3.h>
#include <HSim/surface3.h>

namespace HSim
{
    template <typename T>
    class Triangle3 : public Surface3<T>
    {
    public:
        Triangle3() { SurfaceType = SurfaceType::TRIANGLE; }

        Triangle3(const Triangle3<T> &triangle3_)
            : Surface3<T>(triangle3_),
              points(triangle3_.points), normals(triangle3_.normals), uvs(triangle3_.uvs)
        {
        }

        Triangle3(const Transform3<T> &transform_) : Surface3(transform_) { SurfaceType = SurfaceType::TRIANGLE; }

        Triangle3(
            const std::array<Vec3<T>, 3> &points_,
            const std::array<Vec3<T>, 3> &normals_,
            const std::array<Vec2<T>, 3> &uvs_,
            const Transform3<T> &transform_)
            : points(points_), normals(normals_), uvs(uvs_), Surface3(transform_)
        {
            SurfaceType = SurfaceType::TRIANGLE;
        }

        template <typename U>
        Triangle3(const Triangle3<U> triangle_)
            : points(triangle_.points),
              normals(triangle_.normals),
              uvs(triangle_.uvs),
              Surface3(triangle_.transform)
        {
            SurfaceType = SurfaceType::TRIANGLE;
        }

        ~Triangle3(){};

        void clone(std::shared_ptr<SpaceObject3<T>>& ptr) override
		{
			ptr = std::make_shared<Triangle3<T>>(*this);
		}

    public:
        T area() const
        {
            return 0.5 * (points[1] - points[0]).cross(points[2] - points[0]).length();
        }

        Vec3<T> faceNormal() const
        {
            return (points[1] - points[0]).cross(points[2] - points[0]).getNormalized();
        }

        void setNormalsToFaceNormal()
        {
            normals[0] = normals[1] = normals[2] = faceNormal();
        }

        Vec3<T> barycentricCoords(const Vec3<T> &p)
        {
            auto A = points[0];
            auto B = points[1];
            auto C = points[2];

            auto ab = (B - A).cross(p - A);
            auto bc = (C - B).cross(p - B);
            auto ca = (A - C).cross(p - A);

            auto ta = (0.5 * bc.length()) / area();
            auto tb = (0.5 * ca.length()) / area();
            auto tc = (0.5 * ab.length()) / area();

            return {ta, tb, tc};
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
            if (N.dot(ab) < 0.)
            {
                return closestPositionOnLineSegment(A, B, P);
            }

            auto bc = (C - B).cross(projP - B);
            if (N.dot(bc) < 0.)
            {
                return closestPositionOnLineSegment(B, C, P);
            }

            auto ca = (A - C).cross(projP - A);
            if (N.dot(ca) < 0.)
            {
                return closestPositionOnLineSegment(A, C, P);
            }

            return projP;
        }

        Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
        {
            auto A = points[0];
            auto B = points[1];
            auto C = points[2];

            auto ANormal = normals[0];
            auto BNormal = normals[1];
            auto CNormal = normals[2];

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
             *  closest normal closestNormal to triangle ABC
             *
             *  1. projP is outside the triangle ABC
             *
             *  ab = (B - A) x (projP - A)
             *      if ab < 0, closestNormal = closestNormalOnLineSegment(A, ANormal, B, BNormal, P)
             *  bc = (C - B) x (projP - B)
             *      if bc < 0, closestNormal = closestNormalOnLineSegment(B, BNormal, C, CNormal, P)
             *  ca = (A - C) x (projP - A)
             *      if ca < 0, closestNormal = closestNormalOnLineSegment(A, ANormal, C, CNormal, P)
             *
             *  2. projP is inside the triangle ABC
             *  closestNormal = (ta * ANormal + tb * BNormal + tc * CNormal).getNormalized()
             *  ta = area(BPC) / area(ABC), tb = area(APC) / area(ABC), tc = area(ABP) / area(ABC)
             */
            auto ab = (B - A).cross(projP - A);
            if (N.dot(ab) < 0.)
            {
                return closestNormalOnLineSegment(A, ANormal, B, BNormal, P);
            }

            auto bc = (C - B).cross(projP - B);
            if (N.dot(bc) < 0.)
            {
                return closestNormalOnLineSegment(B, BNormal, C, CNormal, P);
            }

            auto ca = (A - C).cross(projP - A);
            if (N.dot(ca) < 0.)
            {
                return closestNormalOnLineSegment(A, ANormal, C, CNormal, P);
            }

            auto ta = (0.5 * bc.length()) / area();
            auto tb = (0.5 * ca.length()) / area();
            auto tc = (0.5 * ab.length()) / area();

            // std::cout << ta << " " << tb << " "<< tc << " " << std::endl;
            // std::cout << (ta * ANormal + tb * BNormal + tc * CNormal);

            return (ta * ANormal + tb * BNormal + tc * CNormal).getNormalized();
        }

        AABB3<T> AABBLocal() override
        {
            AABB3<T> aabb(points[0], points[1]);
            aabb.merge(points[2]);

            return aabb;
        }

        bool intersectedLocal(const Ray3<T> &ray) const override
        {
            // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

            auto A = points[0];
            auto B = points[1];
            auto C = points[2];
            auto origin = ray.origin;
            auto direction = ray.direction;

            auto AB = B - A;
            auto AC = C - A;

            auto ray_cross_AC = direction.cross(AC);
            auto det = AB.dot(ray_cross_AC);

            // The ray is parallel to the triangle.
            if (det > -EPSILON && det < EPSILON)
            {
                return false;
            }

            auto inv_det = 1.0 / det;
            auto s = origin - A;
            auto u = inv_det * s.dot(AC);

            if (u < 0 || u > 1)
            {
                return false;
            }

            auto s_cross_AB = s.cross(AB);
            auto v = inv_det * direction.dot(s_cross_AB);

            if (v < 0 || u + v > 1)
            {
                return false;
            }

            auto t = inv_det * AC.dot(s_cross_AB);

            if (t > EPSILON)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        IntersectionInfo interactLocal(const Ray3<T> &ray) const override
        {
            // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

            IntersectionInfo intersectionInfo;

            auto A = points[0];
            auto B = points[1];
            auto C = points[2];
            auto origin = ray.origin;
            auto direction = ray.direction;

            auto AB = B - A;
            auto AC = C - A;

            auto ray_cross_AC = direction.cross(AC);
            auto det = AB.dot(ray_cross_AC);

            // The ray is parallel to the triangle.
            if (det > -EPSILON && det < EPSILON)
            {
                return intersectionInfo;
            }

            auto inv_det = 1.0 / det;
            auto s = origin - A;
            auto u = inv_det * s.dot(AC);

            if (u < 0 || u > 1)
            {
                return intersectionInfo;
            }

            auto s_cross_AB = s.cross(AB);
            auto v = inv_det * direction.dot(s_cross_AB);

            if (v < 0 || u + v > 1)
            {
                return intersectionInfo;
            }

            auto t = inv_det * AC.dot(s_cross_AB);

            if (t > EPSILON)
            {
                auto P = ray.pointAt(t);
                auto ab = (B - A).cross(P - A);
                auto bc = (C - B).cross(P - B);
                auto ca = (A - C).cross(P - A);

                auto ta = (0.5 * bc.length()) / area();
                auto tb = (0.5 * ca.length()) / area();
                auto tc = (0.5 * ab.length()) / area();

                auto ANormal = normals[0];
                auto BNormal = normals[1];
                auto CNormal = normals[2];

                intersectionInfo.isIntersected = true;
                intersectionInfo.distance = t;
                intersectionInfo.position = P;
                intersectionInfo.normal = (ta * ANormal + tb * BNormal + tc * CNormal).getNormalized();

                return intersectionInfo;
            }
            else
            {
                return intersectionInfo;
            }
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

    using Triangle3f = Triangle3<float>;
    using Triangle3d = Triangle3<double>;

    template <typename T>
    using Triangle3_Ptr = std::shared_ptr<Triangle3<T>>;

} // namespace HSim
