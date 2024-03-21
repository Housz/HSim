#pragma once 

#include <HSim/common.h>
#include <HSim/vec2.h>
#include <HSim/vec3.h>
#include <HSim/transform3.h>
#include <HSim/surface3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace HSim
{
	template <typename T>
	class TriangleMesh3 : public Surface3<T>
	{
	public:
		// default constructor
		TriangleMesh3() {}
		TriangleMesh3(const Transform3<T> &transform_) : transform(transform_) {}
		// TriangleMesh3(points, normals ...)

		~TriangleMesh3() {}

	// manipulators
	public:
		void addPoint(const Vec3<T>& point_)
		{
			points.push_back(point_);
		} 

		void addNormal(const Vec3<T>& normal_)
		{
			normals.push_back(normal_);
		}

		void addUv(const Vec2<T>& uv_)
		{
			uvs.push_back(uv_);
		}

		// void addTriangle()


	// IO .obj file
	public:

	// in local frame
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo
			return {0, 0, 0};
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo
			return {0, 0, 0};
		}

		AABB3<T> AABBLocal() const override
		{
			// todo
			AABB3 aabb;

			return aabb;
		}


	// data
	public:
		// Transform3<T> transform Inherited from Surface3;

		// points
		std::vector<Vec3<T>> points;
		// normals
		std::vector<Vec3<T>> normals;
		// uvs
		std::vector<Vec2<T>> uvs;

		// pointIndices
		std::vector<Vec3ui> pointIndices;
		// normalIndices
		std::vector<Vec3ui> normalIndices;
		// uvIndices
		std::vector<Vec3ui> uvIndices;

	
	// for rendering
	public:

	};
	
	

	// ORM to .obj

	// to VAO  VBO EBO
	
} // namespace HSim
