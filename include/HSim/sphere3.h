#pragma once

#include <HSim/common.h>
#include <HSim/surface3.h>
#include <HSim/vec3.h>

namespace HSim
{
	template <typename T>
	class Sphere3 : public Surface3<T>
	{
	public:
		Sphere3(){};
		~Sphere3(){};

		// Sphere3(const Vec3<T> &center_, const T radius_) : center(center_), radius(radius_) {}

		Sphere3(const Transform3<T> &transform_) : Surface3<T>(transform_){};

		Sphere3(const Vec3<T> &center_, const T radius_, const Transform3<T> &transform_)
			: Surface3<T>(transform_), center(center_), radius(radius_) {}

		Sphere3(const Sphere3<T> &sphere_)
			: Surface3<T>(sphere_.transform), center(sphere_.center), radius(sphere_.radius) {}

		void setCenter(const Vec3<T> center_) { center = center_; }
		void setRadius(const T radius_) { radius = radius_; }
		void setTransform(const Transform3<T> transform_) { transform = transform_; }

		// center in world frame;
		Vec3<T> getCenter() const
		{
			return transform.toWorld(center);
		}

		// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			return center + radius * closestNormalLocal(positionInLocal_);
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			if (center.isSimilar(positionInLocal_))
			{
				return Vec3<T>(1, 0, 0); // ?
			}
			else
			{
				return (positionInLocal_ - center).getNormalized();
			}
		}

		AABB3<T> AABBLocal()  override
		{
			AABB3<T> aabb;
			auto offset = {radius, radius, radius};
			aabb.lowerCorner = center - offset;
			aabb.upperCorner = center + offset;
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
		// center in local frame;
		Vec3<T> center = {0, 0, 0};
		T radius = 1;

		// for rendering
		// todo
	public:
		unsigned int vaoID = 0;
		unsigned int vboID = 0;
		unsigned int eboID = 0;

		size_t numSectors = 30;
		size_t numStacks = 30;

		std::vector<float> buildVertices()
		{
			std::vector<float> vertices;

			float radius = 1;

			for (size_t stack = 0; stack <= numStacks; stack++)
			{
				float phi = PI_HALF - PI * ((float)stack / numStacks);
				float y = radius * std::sin(phi);

				for (size_t sector = 0; sector <= numSectors; sector++)
				{
					float theta = PI_DOUBLE * ((float)sector / numSectors);

					float x = radius * std::cos(phi) * std::sin(theta);
					float z = radius * std::cos(phi) * std::cos(theta);

					// position for layout 0
					vertices.push_back(x);
					vertices.push_back(y);
					vertices.push_back(z);

					// position for layout 1
					vertices.push_back(x / radius);
					vertices.push_back(y / radius);
					vertices.push_back(z / radius);
				}
			}

			// transform
			for (size_t i = 0; i < vertices.size(); i += 3)
			{
				auto v = transform.mul({vertices[i], vertices[i + 1], vertices[i + 2]});

				vertices[i] = v[0];
				vertices[i + 1] = v[1];
				vertices[i + 2] = v[2];
			}

			return vertices;
		}

		std::vector<unsigned int> buildIndices()
		{
			std::vector<unsigned int> indices;
			// indices
			//  k1--k1+1
			//  |  / |
			//  | /  |
			//  k2--k2+1
			unsigned int k1, k2;

			for (size_t i = 0; i < numStacks; ++i)
			{
				k1 = i * (numSectors + 1);
				k2 = k1 + numSectors + 1;

				for (size_t j = 0; j < numSectors; j++, k1++, k2++)
				{
					if (i != 0)
					{
						indices.push_back(k1);
						indices.push_back(k2);
						indices.push_back(k1 + 1);
					}

					if (i != (numStacks - 1))
					{
						indices.push_back(k1 + 1);
						indices.push_back(k2);
						indices.push_back(k2 + 1);
					}
				}
			}

			return indices;
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

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			auto indices = buildIndices();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			vaoID = vao;
			vboID = vbo;
			eboID = ebo;
		}

		void serialize() override
		{
			// if (this->updated)
			if (this->renderingDataNeedUpdate)
			{
				if (vaoID && vboID && eboID)
				{
					std::cout << "------------------glDeleteBuffers-------------------" << std::endl;
					glDeleteBuffers(1, &vboID);
					glDeleteVertexArrays(1, &vaoID);
					glDeleteBuffers(1, &eboID);
				}

				buildRenderingData();

				renderingDataNeedUpdate = false;

			}
			else
			{
				std::cout << "------- pass sphere serialize -------" << std::endl;
				return;
			}
		}

		void draw() override
		{
			if (!vboID || !eboID || !vaoID)
			{
				buildRenderingData();
				std::cout << "sphere init draw" << std::endl;
			}

			std::cout << vboID << std::endl;

			glBindVertexArray(vaoID);

			// glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDrawElements(GL_TRIANGLES, numSectors * numStacks * 2 * 3, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	};

	template <typename T>
	using Sphere3_Ptr = std::shared_ptr<Sphere3<T>>;

} // namespace HSim
