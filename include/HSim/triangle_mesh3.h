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
		void addPoint(const Vec3<T>& point)
		{
			_points.push_back(point);
		} 

		void addNormal(const Vec3<T>& normal)
		{
			_normals.push_back(normal);
		}

		void addUv(const Vec2<T>& uv)
		{
			_uvs.push_back(uv);
		}

		// void addTriangle()


	// IO .obj file
	public:


	// data
	public:
		// Transform3<T> transform Inherited from Surface3;

		// points
		std::vector<Vec3<T>> _points;
		// normals
		std::vector<Vec3<T>> _normals;
		// uvs
		std::vector<Vec2<T>> _uvs;

		// pointIndices
		std::vector<Vec3ui> _pointIndices;
		// normalIndices
		std::vector<Vec3ui> _normalIndices;
		// uvIndices
		std::vector<Vec3ui> _uvIndices;

	
	// for rendering
	public:

	};
	
	

	// ORM to .obj

	// to VAO  VBO EBO
	
} // namespace HSim
