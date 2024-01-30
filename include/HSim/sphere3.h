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

		// data
	public:
		// center in local frame;
		Vec3<T> center = {0, 0, 0};
		T radius = 1;

		// for rendering
	public:
		unsigned int vaoID = 0;
		unsigned int vboID = 0;

		void serialize() override
		{
			if (this->updated)
			{
				if (vaoID && vboID)
				{
					std::cout << "------------------glDeleteBuffers-------------------" << std::endl;
					glDeleteBuffers(1, &vboID);
					glDeleteVertexArrays(1, &vaoID);
				}

				// std::unique_lock<std::mutex> lk(mtx);

				// std::cout << "serialize" << std::endl;
				// std::cout << transform.translation;

				vboID = toVBO();
				vaoID = toVAO();

				updated = false;

				// lk.unlock();
			}
			else
			{
				std::cout << "------- pass -------" << std::endl;
				return;
			}
		}

		size_t toVBO() const override
		{
			unsigned int vboID;
			glGenBuffers(1, &vboID);

			const int numSlices = 30;
			const int numStacks = 30;
			float vertices[(numSlices + 1) * (numStacks + 1) * 3 * 2];

			int index = 0;
			// for (int stack = 0; stack <= numStacks; ++stack)
			// {
			// 	for (int slice = 0; slice <= numSlices; ++slice)
			// 	{
			// 		float theta = stack * PI / numStacks;
			// 		float phi = slice * 2 * PI / numSlices;

			// 		float x = radius * std::sin(theta) * std::cos(phi);
			// 		float y = radius * std::cos(theta);
			// 		float z = radius * std::sin(theta) * std::sin(phi);

			// 		// position
			// 		vertices[index++] = x;
			// 		vertices[index++] = y;
			// 		vertices[index++] = z;

			// 		// norm
			// 		vertices[index++] = x / radius;
			// 		vertices[index++] = y / radius;
			// 		vertices[index++] = z / radius;
			// 	}
			// }

			for (int i = 0; i <= numStacks; ++i)
			{
				float stackAngle = PI / 2.0f - i * PI / numStacks; // 纬度角度

				for (int j = 0; j <= numSlices; ++j)
				{
					float sectorAngle = j * 2.0f * PI / numSlices; // 经度角度

					float x = radius * std::cos(sectorAngle) * std::sin(stackAngle);
					float y = radius * std::cos(stackAngle);
					float z = radius * std::sin(sectorAngle) * std::sin(stackAngle);

					vertices[index++] = x;
					vertices[index++] = y;
					vertices[index++] = z;

					// norm
					vertices[index++] = x / radius;
					vertices[index++] = y / radius;
					vertices[index++] = z / radius;
				}
			}

			// transform
			for (size_t i = 0; i < sizeof(vertices) / sizeof(float); i += 3)
			{
				// vertices[i], vertices[i+1], vertices[i+2]
				auto v = transform.mul({vertices[i], vertices[i + 1], vertices[i + 2]});
				vertices[i] = v[0];
				vertices[i + 1] = v[1];
				vertices[i + 2] = v[2];
			}

			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			std::cout << "vbo" << std::endl;

			return vboID;
		}

		// Deprecated
		size_t toEBO() const override
		{
			return 0;
		}

		size_t toVAO() const override
		{
			unsigned int vaoID;
			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);

			// VAO layout 0
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);

			// VAO layout 1
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			// unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			return vaoID;
		}

		void draw() const override
		{
			std::cout << vaoID << std::endl;

			glBindVertexArray(vaoID);

			// glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawArrays(GL_POINTS, 0, (30 + 1) * (30 + 1));
			// glDrawArrays(GL_TRIANGLE_FAN, 0, (30 + 1) * (30 + 1) / 3);

			// unbind
			glBindVertexArray(0);
		}
	};

	template <typename T>
	using Sphere3_Ptr = std::shared_ptr<Sphere3<T>>;

} // namespace HSim
