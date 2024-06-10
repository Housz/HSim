#pragma once

#include <HSim/common.h>
#include <HSim/vec2.h>
#include <HSim/vec3.h>
#include <HSim/transform3.h>
#include <HSim/surface3.h>
#include <HSim/triangle3.h>
#include <HSim/bvh3.h>
#include <HSim/parallel.h>

// #define TINYOBJLOADER_IMPLEMENTATION
// #include "tiny_obj_loader.h"

namespace HSim
{
	template <typename T>
	class TriangleMesh3 : public Surface3<T>
	{
	public:
		TriangleMesh3()
		{
			SurfaceType = SurfaceType::TRIANGLE_MESH;

			bvh = std::make_shared<BVH3<T>>();
		}

		TriangleMesh3(const TriangleMesh3<T>& triangleMesh3_) 
			: Surface3<T>(triangleMesh3_),
			  points(triangleMesh3_.points), normals(triangleMesh3_.normals), uvs(triangleMesh3_.uvs),
			  pointIndices(triangleMesh3_.pointIndices), normalIndices(triangleMesh3_.normalIndices), uvIndices(triangleMesh3_.uvIndices)
		{
		}

		TriangleMesh3(const Transform3<T> &transform_) : transform(transform_)
		{
			SurfaceType = SurfaceType::TRIANGLE_MESH;

			bvh = std::make_shared<BVH3<T>>();
		}

		TriangleMesh3(
			const std::vector<Vec3<T>> &points_, const std::vector<Vec3<T>> &normals_, const std::vector<Vec2<T>> &uvs_,
			const std::vector<Vec3ui> pointIndices_, const std::vector<Vec3ui> normalIndices_, const std::vector<Vec3ui> uvIndices_,
			const Transform3<T> &transform_)
			: points(points_), normals(normals_), uvs(uvs_),
			  pointIndices(pointIndices_), normalIndices(normalIndices_), uvIndices(uvIndices_),
			  transform(transform_)
		{
			SurfaceType = SurfaceType::TRIANGLE_MESH;

			bvh = std::make_shared<BVH3<T>>();
		}

		~TriangleMesh3(){};

		void clone(std::shared_ptr<SpaceObject3<T>>& ptr) override
		{
			ptr = std::make_shared<TriangleMesh3<T>>(*this);
		}

		// manipulators
	public:
		void addPoint(const Vec3<T> &point_)
		{
			points.push_back(point_);
			resetStatusFlags();
		}

		void addNormal(const Vec3<T> &normal_)
		{
			normals.push_back(normal_);
			resetStatusFlags();
		}

		void addUV(const Vec2<T> &uv_)
		{
			uvs.push_back(uv_);
			resetStatusFlags();
		}

		void addTriangle(const Triangle3<T> &triangle_)
		{
			auto pointIndex = points.size();
			addPoint(triangle_.points[0]);
			addPoint(triangle_.points[1]);
			addPoint(triangle_.points[2]);
			pointIndices.push_back({pointIndex, pointIndex + 1, pointIndex + 2});

			if (!empty() && hasNormal())
			{
				assert(triangle_.normals.size() == 3);

				auto normalIndex = normals.size();
				addNormal(triangle_.normals[0]);
				addNormal(triangle_.normals[1]);
				addNormal(triangle_.normals[2]);
				normalIndices.push_back({normalIndex, normalIndex + 1, normalIndex + 2});
			}

			if (!empty() && hasUV())
			{
				assert(triangle_.uvs.size() == 3);

				auto uvIndex = uvs.size();
				addUV(triangle_.uvs[0]);
				addUV(triangle_.uvs[1]);
				addUV(triangle_.uvs[2]);
				uvIndices.push_back({uvIndex, uvIndex + 1, uvIndex + 2});
			}

			resetStatusFlags();
		}

		void addTrianglePointIndices(const Vec3ui &trianglePointIndices)
		{
			pointIndices.push_back(trianglePointIndices);
			resetStatusFlags();
		}

		void addTriangleNormalIndices(const Vec3ui &triangleNormalIndices)
		{
			normalIndices.push_back(triangleNormalIndices);
			resetStatusFlags();
		}

		void addTriangleUVIndices(const Vec3ui &triangleUVIndices)
		{
			uvIndices.push_back(triangleUVIndices);
			resetStatusFlags();
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

		Triangle3<T> getTrianlgeByIndex(size_t t) const
		{
			Triangle3<T> tri;

			auto a = points[pointIndices[t][0]];
			auto b = points[pointIndices[t][1]];
			auto c = points[pointIndices[t][2]];

			tri.points[0] = a;
			tri.points[1] = b;
			tri.points[2] = c;

			if (hasNormal())
			{
				tri.normals[0] = normals[normalIndices[t][0]];
				tri.normals[1] = normals[normalIndices[t][1]];
				tri.normals[2] = normals[normalIndices[t][2]];
			}
			else
			{
				tri.setNormalsToFaceNormal();
			}

			if (hasUV())
			{
				tri.uvs[0] = uvs[uvIndices[t][0]];
				tri.uvs[1] = uvs[uvIndices[t][1]];
				tri.uvs[2] = uvs[uvIndices[t][2]];
			}

			return tri;
		}

		bool hasNormal() const
		{
			return normals.size() > 0;
		}

		bool hasUV() const
		{
			return uvs.size() > 0;
		}

		bool empty() const
		{
			return points.empty();
		}

		size_t numPoints() const
		{
			return points.size();
		}

		size_t numNormals() const
		{
			return normals.size();
		}

		size_t numUVs() const
		{
			return uvs.size();
		}

		size_t numTrianlges() const
		{
			return pointIndices.size();
		}

		void buildAABB()
		{
			aabb.reset();

			for (size_t i = 0; i < numTrianlges(); i++)
			{
				auto trianlge = getTrianlgeByIndex(i);
				aabb.merge(trianlge.AABBLocal());
			}
		}

		void buildBVH()
		{
			bvh->reset();

			// create primitiveIndices (triangles indices) and primitivesAABBs (triangles AABBs)
			std::vector<size_t> primitiveIndices(numTrianlges());
			std::iota(primitiveIndices.begin(), primitiveIndices.end(), 0);

			std::vector<AABB3<T>> primitivesAABBs(numTrianlges());
			parallelFor(size_t(0), numTrianlges(), [&](size_t i)
						{
				auto trianlge = getTrianlgeByIndex(i);
				auto triangleAABB = trianlge.AABBLocal();
				primitivesAABBs[i] = triangleAABB; });

			// debug
			// for(auto aabb : primitivesAABBs)
			// {
			// 	std::cout << aabb.lowerCorner;
			// 	std::cout << aabb.upperCorner;
			// }

			bvh->build(primitiveIndices, primitivesAABBs);
		}

		// IO .obj file
	public:
		// bool readOBJ(const std::string &filename)
		// {
		// 	// https://github.com/tinyobjloader/tinyobjloader

		// 	tinyobj::ObjReaderConfig reader_config;
		// 	reader_config.mtl_search_path = "./";

		// 	tinyobj::ObjReader reader;

		// 	if (!reader.ParseFromFile(filename, reader_config))
		// 	{
		// 		if (!reader.Error().empty())
		// 		{
		// 			std::cerr << "TinyObjReader: " << reader.Error();
		// 		}
		// 		return false;
		// 	}

		// 	if (!reader.Warning().empty())
		// 	{
		// 		std::cout << "TinyObjReader: " << reader.Warning();
		// 		return false;
		// 	}

		// 	auto &attrib = reader.GetAttrib();
		// 	auto &shapes = reader.GetShapes();
		// 	auto &materials = reader.GetMaterials();

		// 	// Read vertices
		// 	for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		// 	{
		// 		auto vx = attrib.vertices[i + 0];
		// 		auto vy = attrib.vertices[i + 1];
		// 		auto vz = attrib.vertices[i + 2];

		// 		addPoint({vx, vy, vz});
		// 	}

		// 	// Read normals
		// 	for (size_t i = 0; i < attrib.normals.size(); i += 3)
		// 	{
		// 		auto nx = attrib.normals[i + 0];
		// 		auto ny = attrib.normals[i + 1];
		// 		auto nz = attrib.normals[i + 2];

		// 		addNormal({nx, ny, nz});
		// 	}

		// 	// Read UVs
		// 	for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
		// 	{
		// 		auto u = attrib.texcoords[i + 0];
		// 		auto v = attrib.texcoords[i + 1];

		// 		addUV({u, v});
		// 	}

		// 	// Read faces (triangles)
		// 	for (auto &shape : shapes)
		// 	{
		// 		size_t idx = 0;

		// 		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
		// 		{
		// 			const size_t fv = shape.mesh.num_face_vertices[f];

		// 			if (fv == 3)
		// 			{
		// 				if (!attrib.vertices.empty())
		// 				{
		// 					addTrianglePointIndices(
		// 						{shape.mesh.indices[idx].vertex_index,
		// 						 shape.mesh.indices[idx + 1].vertex_index,
		// 						 shape.mesh.indices[idx + 2].vertex_index});
		// 				}

		// 				if (!attrib.normals.empty())
		// 				{
		// 					addTriangleNormalIndices(
		// 						{shape.mesh.indices[idx].normal_index,
		// 						 shape.mesh.indices[idx + 1].normal_index,
		// 						 shape.mesh.indices[idx + 2].normal_index});
		// 				}

		// 				if (!attrib.texcoords.empty())
		// 				{
		// 					addTriangleUVIndices({shape.mesh.indices[idx].texcoord_index,
		// 										  shape.mesh.indices[idx + 1].texcoord_index,
		// 										  shape.mesh.indices[idx + 2].texcoord_index});
		// 				}
		// 			} // if (fv == 3)

		// 			idx += fv;
		// 		}
		// 	}

		// 	resetStatusFlags();

		// 	return true;

		// } // readOBJ

		// in local frame
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			// std::function<float(const HSim::Vec3f& position, const size_t primitiveIndex)> distanceFunction =
			auto distanceFunction =
				[&](const HSim::Vec3<float> &position, const size_t primitiveIndex) -> float
			{
				auto triangle = getTrianlgeByIndex(primitiveIndex);
				auto closestPositionInTriangle = triangle.closestPositionLocal(position);
				return position.distanceTo(closestPositionInTriangle);
			};

			auto closestPrimitiveInfo = bvh->closestPrimitive(positionInLocal_, distanceFunction);

			std::cout << closestPrimitiveInfo;

			auto closestTriangle = getTrianlgeByIndex(closestPrimitiveInfo.primitiveIndex);
			auto closestPosition = closestTriangle.closestPositionLocal(positionInLocal_);

			return closestPosition;
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			auto distanceFunction =
				[&](const HSim::Vec3<float> &position, const size_t primitiveIndex) -> float
			{
				auto triangle = getTrianlgeByIndex(primitiveIndex);
				auto closestPositionInTriangle = triangle.closestPositionLocal(position);
				return position.distanceTo(closestPositionInTriangle);
			};

			auto closestPrimitiveInfo = bvh->closestPrimitive(positionInLocal_, distanceFunction);

			std::cout << closestPrimitiveInfo;

			auto closestTriangle = getTrianlgeByIndex(closestPrimitiveInfo.primitiveIndex);
			auto closestNormal = closestTriangle.faceNormal();

			return closestNormal;
		}

		AABB3<T> AABBLocal() override
		{
			if (aabbNeedUpdate)
			{
				buildAABB();
				aabbNeedUpdate = false;
			}

			return aabb;
		}

		bool intersectedLocal(const Ray3<T> &ray) const override
		{
			// todo
			// bvh intersects(ray, testfunc)
			return false;
		}

		IntersectionInfo interactLocal(const Ray3<T> &ray) const override
		{
			// todo
			IntersectionInfo intersectionInfo;

			return intersectionInfo;
		}

		bool isInsideLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo

			return false;
		}

		// data
	public:
		// Transform3<T> transform Inherited from Surface3;

		// points (positions)
		std::vector<Vec3<T>> points;
		// normals
		std::vector<Vec3<T>> normals;
		// uvs
		std::vector<Vec2<T>> uvs;

		// pointIndices (positionIndices)
		std::vector<Vec3ui> pointIndices;
		// normalIndices
		std::vector<Vec3ui> normalIndices;
		// uvIndices
		std::vector<Vec3ui> uvIndices;

		AABB3<T> aabb;

		// BVH3<T> bvh; // in local frame
		BVH3_Ptr<T> bvh;


	};

	using TriangleMesh3f = TriangleMesh3<float>;
	using TriangleMesh3d = TriangleMesh3<double>;

	template <typename T>
	using TriangleMesh3_Ptr = std::shared_ptr<TriangleMesh3<T>>;

} // namespace HSim
