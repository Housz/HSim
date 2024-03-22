#pragma once

#include <HSim/common.h>
#include <HSim/vec2.h>
#include <HSim/vec3.h>
#include <HSim/transform3.h>
#include <HSim/surface3.h>
#include <HSim/triangle3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace HSim
{
	template <typename T>
	class TriangleMesh3 : public Surface3<T>
	{
	public:
		TriangleMesh3() {}
		TriangleMesh3(const Transform3<T> &transform_) : transform(transform_) {}
		TriangleMesh3(
			const std::vector<Vec3<T>> &points_, const std::vector<Vec3<T>> &normals_, const std::vector<Vec2<T>> &uvs_,
			const std::vector<Vec3ui> pointIndices_, const std::vector<Vec3ui> normalIndices_, const std::vector<Vec3ui> uvIndices_,
			const Transform3<T> &transform_)
			: points(points), normals(normals_), uvs(uvs_),
			  pointIndices(pointIndices_), normalIndices(normalIndices_), uvIndices(uvIndices_),
			  transform(transform_)
		{
		}

		~TriangleMesh3() {}

		// manipulators
	public:
		void addPoint(const Vec3<T> &point_)
		{
			points.push_back(point_);
		}

		void addNormal(const Vec3<T> &normal_)
		{
			normals.push_back(normal_);
		}

		void addUV(const Vec2<T> &uv_)
		{
			uvs.push_back(uv_);
		}

		void addTriangle(const Triangle3<T> &triangle_)
		{
		}

		// getters setters
	public:
		T area() const
		{
			// todo
			return 0;
		}

		T volume() const
		{
			// todo
			return 0;
		}

		Vec3<T> point(size_t i) const
		{
			return points[i];
		}

		Vec3<T> &point(size_t i)
		{
			return points[i];
		}

		Vec3<T> normal(size_t i) const
		{
			return normals[i];
		}

		Vec3<T> &normal(size_t i)
		{
			return normals[i];
		}

		Vec2<T> uv(size_t i) const
		{
			return uvs[i];
		}

		Vec2<T> &uv(size_t i)
		{
			return uvs[i];
		}

		Triangle3<T> trianlge(size_t t) const
		{
			// todo
			Triangle3<T> tri;

			return tri;
		}

		// IO .obj file
	public:
		bool readOBJ(const std::string &filename)
		{
			// https://github.com/tinyobjloader/tinyobjloader

			tinyobj::ObjReaderConfig reader_config;
			reader_config.mtl_search_path = "./";

			tinyobj::ObjReader reader;

			if (!reader.ParseFromFile(filename, reader_config))
			{
				if (!reader.Error().empty())
				{
					std::cerr << "TinyObjReader: " << reader.Error();
				}
				return false;
			}

			if (!reader.Warning().empty())
			{
				std::cout << "TinyObjReader: " << reader.Warning();
				return false;
			}

			auto &attrib = reader.GetAttrib();
			auto &shapes = reader.GetShapes();
			auto &materials = reader.GetMaterials();

			// Read vertices
			for (size_t i = 0; i < attrib.vertices.size(); i += 3)
			{
				auto vx = attrib.vertices[i + 0];
				auto vy = attrib.vertices[i + 1];
				auto vz = attrib.vertices[i + 2];

				addPoint({vx, vy, vz});
			}

			// Read normals
			for (size_t i = 0; i < attrib.normals.size(); i += 3)
			{
				auto nx = attrib.normals[i + 0];
				auto ny = attrib.normals[i + 1];
				auto nz = attrib.normals[i + 2];

				addNormal({nx, ny, nz});
			}

			// Read UVs
			for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
			{
				auto u = attrib.texcoords[i + 0];
				auto v = attrib.texcoords[i + 1];

				addUV({u, v});
			}

			// Read faces (triangles)
			for (auto& shape : shapes)
			{
				
			}


			return true;
		}

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
			AABB3<T> aabb;

			return aabb;
		}

		bool intersectedLocal(const Ray3<T> &ray) const override
		{
			return false;
		}

		IntersectionInfo interactLocal(const Ray3<T> &ray) const override
		{
			IntersectionInfo intersectionInfo;

			return intersectionInfo;
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

	using TriangleMesh3f = TriangleMesh3<float>;
	using TriangleMesh3d = TriangleMesh3<double>;

	// ORM to .obj

	// to VAO  VBO EBO

} // namespace HSim
