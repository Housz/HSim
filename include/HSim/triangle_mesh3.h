#pragma once

// todo vertices transform

/**
 * 
*/

#include <HSim/common.h>
#include <HSim/vec2.h>
#include <HSim/vec3.h>
#include <HSim/transform3.h>
#include <HSim/surface3.h>
#include <HSim/triangle3.h>
#include <HSim/bvh3.h>
#include <HSim/parallel.h>

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
			bvh.reset();

			// create primitiveIndices (triangles indices) and primitivesAABBs (triangles AABBs)
			std::vector<size_t> primitiveIndices(numTrianlges());
			std::iota(primitiveIndices.begin(), primitiveIndices.end(), 0);

			std::vector<AABB3<T>> primitivesAABBs(numTrianlges());
			parallelFor(size_t(0), numTrianlges(), [&] (size_t i){
				auto trianlge = getTrianlgeByIndex(i);
				auto triangleAABB = trianlge.AABBLocal();
				primitivesAABBs[i] = triangleAABB;
			});

			// debug
			// for(auto aabb : primitivesAABBs)
			// {
			// 	std::cout << aabb.lowerCorner;
			// 	std::cout << aabb.upperCorner;
			// }			

			bvh.build(primitiveIndices, primitivesAABBs);
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
			for (auto &shape : shapes)
			{
				size_t idx = 0;

				for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
				{
					const size_t fv = shape.mesh.num_face_vertices[f];

					if (fv == 3)
					{
						if (!attrib.vertices.empty())
						{
							addTrianglePointIndices(
								{shape.mesh.indices[idx].vertex_index,
								 shape.mesh.indices[idx + 1].vertex_index,
								 shape.mesh.indices[idx + 2].vertex_index});
						}

						if (!attrib.normals.empty())
						{
							addTriangleNormalIndices(
								{shape.mesh.indices[idx].normal_index,
								 shape.mesh.indices[idx + 1].normal_index,
								 shape.mesh.indices[idx + 2].normal_index});
						}

						if (!attrib.texcoords.empty())
						{
							addTriangleUVIndices({shape.mesh.indices[idx].texcoord_index,
												  shape.mesh.indices[idx + 1].texcoord_index,
												  shape.mesh.indices[idx + 2].texcoord_index});
						}
					} // if (fv == 3)

					idx += fv;
				}
			}

			resetStatusFlags();

			return true;

		} // readOBJ

		// in local frame
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo
			// invoke bvh closestPosition(positionInLocal_)
			return {0, 0, 0};
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo
			// invoke bvh closestNormal(positionInLocal_)
			return {0, 0, 0};
		}

		AABB3<T> AABBLocal()  override
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

		BVH3<T> bvh;

		// for rendering
	public:
		unsigned int vaoID = 0;
		unsigned int vboID = 0;
		unsigned int eboID = 0;

		// flatten
		template <typename U>
		std::vector<U> flatten(const std::vector<Vec3<U>> &v_)
		{
			std::vector<U> v;
			for (auto element : v_)
			{
				v.push_back(element.x);
				v.push_back(element.y);
				v.push_back(element.z);
			}
			return v;
		}

		std::vector<float> buildVertices()
		{
			std::vector<float> vertices;

			std::vector<Vec3<float>> tempNormals;
			if (!hasNormal())
			{
				// compute normals
				// todo Flat-shading https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ

				tempNormals.resize(numPoints());

				for (size_t triIdx = 0; triIdx < numTrianlges(); triIdx++)
				{
					auto aIndex = pointIndices[triIdx][0];
					auto bIndex = pointIndices[triIdx][1];
					auto cIndex = pointIndices[triIdx][2];

					auto a = points[aIndex];
					auto b = points[bIndex];
					auto c = points[cIndex];

					auto normal = (b - a).cross(c - a).getNormalized();

					tempNormals[aIndex] = normal;
					tempNormals[bIndex] = normal;
					tempNormals[cIndex] = normal;
				}
			}

			for (size_t i = 0; i < numPoints(); i++)
			{
				auto point = points[i];

				vertices.push_back(point.x);
				vertices.push_back(point.y);
				vertices.push_back(point.z);

				Vec3<T> normal;
				if (hasNormal())
				{
					normal = normals[i];
				}
				else
				{
					normal = tempNormals[i];
				}
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}

			return vertices;
		}

		std::vector<float> buildVerticesFlat()
		{
			std::vector<float> vertices;

			for (size_t triIdx = 0; triIdx < numTrianlges(); triIdx++)
			{
				auto aIndex = pointIndices[triIdx][0];
				auto bIndex = pointIndices[triIdx][1];
				auto cIndex = pointIndices[triIdx][2];

				auto a = points[aIndex];
				auto b = points[bIndex];
				auto c = points[cIndex];

				auto normal = (b - a).cross(c - a).getNormalized();

				vertices.push_back(a.x);
				vertices.push_back(a.y);
				vertices.push_back(a.z);
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
				vertices.push_back(b.x);
				vertices.push_back(b.y);
				vertices.push_back(b.z);
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
				vertices.push_back(c.x);
				vertices.push_back(c.y);
				vertices.push_back(c.z);
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}

			return vertices;
		}

		std::vector<float> buildVerticesSmooth()
		{
			std::vector<float> vertices;

			std::vector<Vec3<float>> smoothNormals(numPoints());
			std::vector<size_t> normalCount(numPoints());

			for (size_t triIdx = 0; triIdx < numTrianlges(); triIdx++)
			{
				auto aIndex = pointIndices[triIdx][0];
				auto bIndex = pointIndices[triIdx][1];
				auto cIndex = pointIndices[triIdx][2];

				auto a = points[aIndex];
				auto b = points[bIndex];
				auto c = points[cIndex];

				auto normal = (b - a).cross(c - a).getNormalized();

				normalCount[aIndex]++;
				normalCount[bIndex]++;
				normalCount[cIndex]++;

				smoothNormals[aIndex] = normal / normalCount[aIndex] +
										smoothNormals[aIndex] * (normalCount[aIndex] - 1) / normalCount[aIndex];
				smoothNormals[bIndex] = normal / normalCount[bIndex] +
										smoothNormals[bIndex] * (normalCount[bIndex] - 1) / normalCount[bIndex];
				smoothNormals[cIndex] = normal / normalCount[cIndex] +
										smoothNormals[cIndex] * (normalCount[cIndex] - 1) / normalCount[cIndex];
			}

			for (size_t i = 0; i < numPoints(); i++)
			{
				auto point = points[i];

				// transform
				point = transform.mul(point);

				vertices.push_back(point.x);
				vertices.push_back(point.y);
				vertices.push_back(point.z);

				auto normal = smoothNormals[i];

				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}

			return vertices;
		}

		std::vector<unsigned int> buildIndices()
		{
			// return flatten(pointIndices); // error! size_t 2 unsigned int

			std::vector<unsigned int> v;
			for (auto element : pointIndices)
			{
				v.push_back((unsigned int)element[0]);
				v.push_back((unsigned int)element[1]);
				v.push_back((unsigned int)element[2]);
			}
			return v;
		}

		void buildRenderingData()
		{
			unsigned int vao;
			unsigned int vbo;
			unsigned int ebo;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);

			auto vertices = buildVertices();

			/** debug
			std::cout << "vertices:\n";
			size_t i = 0;
			for (auto ele : vertices)
			{
				std::cout << ele << " ";
				i++;
				if (!(i % 6))
					std::cout << std::endl;
			}
			std::cout << std::endl;
			*/

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			auto indices = buildIndices();

			/** debug
			std::cout << "\nindices:\n";
			i = 0;
			for (auto ele : indices)
			{
				std::cout << ele << " ";
				i++;
				if (!(i % 3))
					std::cout << std::endl;
			}
			std::cout << std::endl;
			*/

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			vaoID = vao;
			vboID = vbo;
			eboID = ebo;
		}

		void buildRenderingDataFlat()
		{
			unsigned int vao;
			unsigned int vbo;

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);
			auto vertices = buildVerticesFlat();

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			// unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			vaoID = vao;
			vboID = vbo;
		}

		void buildRenderingDataSmooth()
		{
			unsigned int vao;
			unsigned int vbo;
			unsigned int ebo;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindVertexArray(vao);

			auto vertices = buildVerticesSmooth();

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			auto indices = buildIndices();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			// unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			vaoID = vao;
			vboID = vbo;
			eboID = ebo;
		}

		/**
		 * Determine whether it is updated before each rendering.
		 * If updated, the render data is regenerated (VAO, VBO, EBO).
		 */
		void serialize() override
		{
			if (this->renderingDataNeedUpdate)
			{
				// Delete previous buffers
				if (vaoID && vboID && eboID)
				{
					std::cout << "------------------glDeleteBuffers-------------------" << std::endl;
					glDeleteBuffers(1, &vboID);
					glDeleteVertexArrays(1, &vaoID);
					glDeleteBuffers(1, &eboID);
				}

				// Build new buffers
#ifdef NAIVE_RENDERING
				buildRenderingData();
#endif // NAIVE_RENDERING

#ifdef FLAT_RENDERING
				buildRenderingDataFlat();
#endif // FLAT_RENDERING

#ifdef SMOOTH_RENDERING
				buildRenderingDataSmooth();
#endif // SMOOTH_RENDERING

				renderingDataNeedUpdate = false;
			}
			else
			{
				// std::cout << "------- pass triangle_mesh3 serialize -------" << std::endl;
				return;
			}
		}


		void draw() override
		{
			serialize();

#ifdef NAIVE_RENDERING
			if (!vboID || !vaoID || !eboID)
			{
				buildRenderingData();
				std::cout << "triangle_mesh init draw naive" << std::endl;
			}
			std::cout << vaoID << std::endl;
			glBindVertexArray(vaoID);

			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, numTrianlges() * 3, GL_UNSIGNED_INT, 0);
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glBindVertexArray(0);

#endif // NAIVE_RENDERING

#ifdef FLAT_RENDERING
			if (!vboID || !vaoID)
			{
				buildRenderingDataFlat();

				std::cout << "triangle_mesh init draw flat" << std::endl;
			}

			std::cout << vaoID << std::endl;

			glBindVertexArray(vaoID);

			glDrawArrays(GL_TRIANGLES, 0, numTrianlges() * 3);

			glBindVertexArray(0);
#endif // FLAT_RENDERING

#ifdef SMOOTH_RENDERING
			if (!vboID || !vaoID || !eboID)
			{
				buildRenderingDataSmooth();
				std::cout << "triangle_mesh init draw smooth" << std::endl;
			}
			std::cout << vaoID << std::endl;
			glBindVertexArray(vaoID);

			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, numTrianlges() * 3, GL_UNSIGNED_INT, 0);
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glBindVertexArray(0);
#endif // SMOOTH_RENDERING
		}


		// void drawBoundary() override
		// {
		// 	if (aabbNeedUpdate)
		// 	{
		// 		buildAABB();

		// 		aabbNeedUpdate = false;

		// 	}

		// 	aabb.draw();
		// 	// AABB().draw();
			
		// }

		// draw bvh by traversing nodes of bvh
		// void drawBoundary() override
		// {
		// 	if (aabbNeedUpdate)
		// 	{
		// 		buildBVH();

		// 		// bvh draw()

		// 		aabbNeedUpdate = false;
		// 	}


		// 	std::function<void(BVH3Node_Ptr)> callback = [&](BVH3Node_Ptr node)
		// 	{
		// 		node->aabb.draw();
		// 	};

		// 	bvh.traverse(callback);
		// }

		// draw bvh
		void drawBoundary() override
		{
			bvh.draw();
		}



	};

	using TriangleMesh3f = TriangleMesh3<float>;
	using TriangleMesh3d = TriangleMesh3<double>;

} // namespace HSim
