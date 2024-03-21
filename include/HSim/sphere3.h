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

		AABB3<T> AABBLocal() const override
		{
			AABB3<T> aabb;
			auto offset = {radius, radius, radius};
			aabb.lowerCorner = center - offset;
			aabb.upperCorner = center + offset;
			return aabb;
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

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		int numSectors = 30;
		int numStacks = 30;

		void serialize() override
		{
			if (this->updated)
			{
				if (vaoID && vboID && eboID)
				{
					std::cout << "------------------glDeleteBuffers-------------------" << std::endl;
					glDeleteBuffers(1, &vboID);
					glDeleteVertexArrays(1, &vaoID);
					glDeleteBuffers(1, &eboID);
				}

				// std::unique_lock<std::mutex> lk(mtx);

				// std::cout << "serialize" << std::endl;
				// std::cout << transform.translation;

				vertices.clear();
				indices.clear();

				vboID = toVBO();
				vaoID = toVAO();
				eboID = toEBO();

				updated = false;

				// lk.unlock();
			}
			else
			{
				std::cout << "------- pass sphere serialize -------" << std::endl;
				return;
			}
		}

		size_t toVBO() override
		{
			vertices.clear();
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

			// std::cout << "index: " << index << std::endl;

			// transform
			for (size_t i = 0; i < vertices.size(); i += 3)
			{
				// vertices[i], vertices[i+1], vertices[i+2]
				auto v = transform.mul({vertices[i], vertices[i + 1], vertices[i + 2]});
				vertices[i] = v[0];
				vertices[i + 1] = v[1];
				vertices[i + 2] = v[2];
			}

			unsigned int vboID;
			glGenBuffers(1, &vboID);
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			// std::cout << "vbo" << std::endl;

			return vboID;
		}

		size_t toEBO() override
		{
			indices.clear();
			
			// float indices[];

			// k1--k1+1
			// |  / |
			// | /  |
			// k2--k2+1

			// unsigned int indices[5220];

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

			unsigned int eboID;
			glGenBuffers(1, &eboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			return eboID;
		}

		size_t toVAO() override
		{
			unsigned int vaoID;
			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			// unbind
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			return vaoID;
		}

		void draw() override
		{
			if (!vboID || !eboID || !vaoID)
			{
				vertices.clear();
				indices.clear();

				vboID = toVBO();
				eboID = toEBO();
				vaoID = toVAO();

				std::cout << "init draw" << std::endl;
			}

			std::cout << vboID << std::endl;
			std::cout << eboID << std::endl;
			std::cout << vaoID << std::endl;

			glBindVertexArray(vaoID);

			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	};

	template <typename T>
	using Sphere3_Ptr = std::shared_ptr<Sphere3<T>>;

} // namespace HSim
