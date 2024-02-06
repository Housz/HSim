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
		unsigned int eboID = 0;

		const int numSectors = 30;
		const int numStacks = 30;

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

		size_t toVBO() const override
		{
			unsigned int vboID;
			glGenBuffers(1, &vboID);

			float vertices[(30 + 1) * (30 + 1) * 2 * 3];

			std::cout << " numStacks " << numStacks << std::endl;

			int index = 0;

			for (size_t stack = 0; stack <= numStacks; stack++)
			{
				
				float phi = PI_HALF - PI * ((float)stack / numStacks);
				float y = radius * std::sin(phi);

				for (size_t sector = 0; sector <= numSectors; sector++)
				{
					float theta = PI_DOUBLE * ((float)sector / numSectors);

					float x = radius * std::cos(phi) * std::sin(theta);
					float z = radius * std::cos(phi) * std::cos(theta);

					// position
					vertices[index++] = x;
					vertices[index++] = y;
					vertices[index++] = z;
					
					// norm (smooth shading)
					vertices[index++] = x / radius;
					vertices[index++] = y / radius;
					vertices[index++] = z / radius;
				}
			}

			std::cout << "index: " << index << std::endl;

			// for (int stack = 0; stack <= numStacks; ++stack)
			// {
			// 	for (int slice = 0; slice <= numSlices; ++slice)
			// 	{
			// 		float phi = stack * PI / numStacks;
			// 		float theta = slice * 2 * PI / numSlices;

			// 		float x = radius * std::sin(theta) * std::sin(phi);
			// 		float y = radius * std::cos(theta);
			// 		float z = radius * std::sin(theta) * std::cos(phi);

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

			// for (int i = 0; i <= numStacks; ++i)
			// {
			// 	float stackAngle = PI / 2.0f - i * PI / numStacks; // 纬度角度

			// 	for (int j = 0; j <= numSlices; ++j)
			// 	{
			// 		float sectorAngle = j * 2.0f * PI / numSlices; // 经度角度

			// 		float x = radius * std::cos(sectorAngle) * std::sin(stackAngle);
			// 		float y = radius * std::cos(stackAngle);
			// 		float z = radius * std::sin(sectorAngle) * std::sin(stackAngle);

			// 		vertices[index++] = x;
			// 		vertices[index++] = y;
			// 		vertices[index++] = z;

			// 		// norm
			// 		vertices[index++] = x / radius;
			// 		vertices[index++] = y / radius;
			// 		vertices[index++] = z / radius;
			// 	}
			// }

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

		size_t toEBO() const override
		{
			// float indices[];

			// k1--k1+1
			// |  / |
			// | /  |
			// k2--k2+1

			// std::vector<int> indices;
			unsigned int indices[5220];

			unsigned int k1, k2;
			int index = 0;
			for (size_t i = 0; i < numStacks; ++i)
			{
				k1 = i * (numSectors + 1);
				k2 = k1 + numSectors + 1;

				for (size_t j = 0; j < numSectors; j++)
				{
					if (i != 0)
					{
						// indices.push_back(k1);
						// indices.push_back(k2);
						// indices.push_back(k1 + 1);
						indices[index++] = k1;
						indices[index++] = k2;
						indices[index++] = k1 + 1;
					}

					if (i != (numStacks - 1))
					{
						// indices.push_back(k1 + 1);
						// indices.push_back(k2);
						// indices.push_back(k2 + 1);
						indices[index++] = k1 + 1;
						indices[index++] = k2;
						indices[index++] = k2 + 1;
					}
				}
			}

			std::cout << "indices " << sizeof(indices) / sizeof(int) << std::endl;

			// std::cout << "indices.size()" << indices.size() << std::endl;

			unsigned int eboID;
			glGenBuffers(1, &eboID);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
			// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			return eboID;
		}

		size_t toVAO() const override
		{
			unsigned int vaoID;
			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);

			// VAO layout 0 (positions)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);

			// VAO layout 1 (normals)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			// unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
   		 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			return vaoID;
		}

		void draw() override
		{
			if (!vboID || !eboID || !vaoID)
			{
				vboID = toVBO();
				eboID = toEBO();
				vaoID = toVAO();

				std::cout << "init draw" << std::endl;
			}
			
			std::cout << vboID << std::endl;
			std::cout << eboID << std::endl;
			std::cout << vaoID << std::endl;

			glBindVertexArray(vaoID);
			// glPointSize(10.0f);
			// glDrawArrays(GL_TRIANGLES, 0, 36);
			// glDrawArrays(GL_POINTS, 0, (30 + 1) * (30 + 1));
			// glDrawArrays(GL_TRIANGLE_FAN, 0, (30 + 1) * (30 + 1) / 3);

			std::cout << "(numStacks ) * (numSectors - 1) * 2 * 3 : " << (numStacks ) * (numSectors - 1) * 2 * 3 << std::endl;
			glDrawElements(GL_TRIANGLES, (numStacks) * (numSectors - 1) * 2 * 3, GL_UNSIGNED_INT, 0);

			// unbind
			glBindVertexArray(0);
		}
	};

	template <typename T>
	using Sphere3_Ptr = std::shared_ptr<Sphere3<T>>;

} // namespace HSim
