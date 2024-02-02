#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>
#include <HSim/surface3.h>
#include <HSim/plane3.h>
#include <HSim/math_utils.h>

namespace HSim
{
	template <typename T>
	class Box3 : public Surface3<T>
	{
	public:
		Box3(){};
		~Box3(){};

		Box3(const Transform3<T> transform_) : transform(transform_) {}

		Box3(const Vec3<T> &lowerCorner_, const Vec3<T> &upperCorner_)
			: lowerCorner(lowerCorner_), upperCorner(upperCorner_) {}

		Box3(const Vec3<T> &lowerCorner_, const Vec3<T> &upperCorner_, const Transform3<T> transform_)
			: lowerCorner(lowerCorner_), upperCorner(upperCorner_), transform(transform_) {}

		Box3(const Box3<T> &box_) : lowerCorner(box_.lowerCorner), upperCorner(box_.upperCorner) {}

		/**
		 * @brief set Box3 with any two points
		 */
		void set(const Vec3<T> &p1, const Vec3<T> &p2)
		{
			lowerCorner = {std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)};
			upperCorner = {std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)};
		}

		void setLowerCorner(const Vec3<T> &lowerCorner_)
		{
			assert(lowerCorner_.x < upperCorner.x && lowerCorner_.y < upperCorner.y && lowerCorner_.z < upperCorner.z);
			lowerCorner = lowerCorner_;
		}

		void setUpperCorner(const Vec3<T> &upperCorner_)
		{
			assert(lowerCorner.x < upperCorner_.x && lowerCorner.y < upperCorner_.y && lowerCorner.z < upperCorner_.z);
			upperCorner = upperCorner_;
		}

		T width() const { return upperCorner.x - lowerCorner.x; }
		T height() const { return upperCorner.y - lowerCorner.y; }
		T depth() const { return upperCorner.z - lowerCorner.z; }

		bool _isInsideLocal(const Vec3<T> &positionInLocal_) const
		{
			auto p_ = positionInLocal_;
			if (p_.x < lowerCorner.x || p_.y < lowerCorner.y || p_.z < lowerCorner.z ||
				p_.x > upperCorner.x || p_.y > upperCorner.y || p_.z > upperCorner.z)
			{
				return false;
			}
			else
				return true;
		}

		// bool isInsideWorld(const Vec3<T> &positionInWorld_) const
		// {
		// 	return isInsideLocal(transform.toLocal(positionInWorld_));
		// }

		// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			// if positionInLocal_ is inside this box
			if (_isInsideLocal(positionInLocal_))
			{
				Plane3<T> planes[6] = {
					Plane3<T>({1, 0, 0}, upperCorner),	// right
					Plane3<T>({0, 1, 0}, upperCorner),	// up
					Plane3<T>({0, 0, 1}, upperCorner),	// front
					Plane3<T>({-1, 0, 0}, lowerCorner), // left
					Plane3<T>({0, -1, 0}, lowerCorner), // down
					Plane3<T>({0, 0, -1}, lowerCorner)	// back
				};

				Vec3<T> closestPosition = planes[0].closestPositionLocal(positionInLocal_);
				T closestDistance = planes[0].closestDistanceLocal(positionInLocal_);
				for (size_t i = 1; i < 6; i++)
				{
					auto currDistance = planes[i].closestDistanceLocal(positionInLocal_);
					if (currDistance < closestDistance)
					{
						closestDistance = currDistance;

						auto currPosition = planes[i].closestPositionLocal(positionInLocal_);
						closestPosition = currPosition;
					}
				}

				return closestPosition;
			}
			// if positionInLocal_ is outside this box
			else
			{
				return clamp(positionInLocal_, lowerCorner, upperCorner);
			}
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			Plane3<T> planes[6] = {
				Plane3<T>({1, 0, 0}, upperCorner),	// right
				Plane3<T>({0, 1, 0}, upperCorner),	// up
				Plane3<T>({0, 0, 1}, upperCorner),	// front
				Plane3<T>({-1, 0, 0}, lowerCorner), // left
				Plane3<T>({0, -1, 0}, lowerCorner), // down
				Plane3<T>({0, 0, -1}, lowerCorner)	// back
			};

			auto closestNormal = planes[0].normal;
			T closestDistance = planes[0].closestDistanceLocal(positionInLocal_);
			for (size_t i = 1; i < 6; i++)
			{
				auto currDistance = planes[i].closestDistanceLocal(positionInLocal_);
				if (currDistance < closestDistance)
				{
					closestDistance = currDistance;
					closestNormal = planes[i].normal;
				}
			}

			return closestNormal;
		}

	public:
		Vec3<T> lowerCorner = {0, 0, 0};
		Vec3<T> upperCorner = {1, 1, 1};

		// for rendering
	public:
		unsigned int vaoID = 0;
		unsigned int vboID = 0;

		// std::queue<unsigned int> vaoBuffer;
		// std::queue<unsigned int> vboBuffer;

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

			// vaoBuffer.push(vaoID);
			// vboBuffer.push(vboID);

			std::cout << vaoID << std::endl;
		}

		size_t toVBO() const override
		{
			unsigned int vboID;
			glGenBuffers(1, &vboID);

			float vertices[] = {

				lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
				upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
				upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

				upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
				lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
				lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

				lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
				upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
				upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

				upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
				lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
				lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

				lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
				upperCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
				upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,

				upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
				lowerCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
				lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,

				lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
				upperCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
				upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,

				upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
				lowerCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
				lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,

				lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
				lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
				lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,

				lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
				lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
				lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,

				upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
				upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
				upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,

				upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
				upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
				upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f};

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
			unsigned int eboID;
			glGenBuffers(1, &eboID);

			float indices[] = {
				0, 1, 2, // front
				2, 3, 0,

				4, 5, 6, // back
				6, 7, 4,

				0, 3, 7, // left
				7, 4, 0,

				1, 2, 6, // right
				6, 5, 1,

				0, 1, 5, // down
				5, 4, 0,

				2, 3, 7, // up
				7, 6, 2};

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			return eboID;
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

		void draw() override
		{
			// toVBO();
			// toEBO();
			// toVAO();

			if (!vaoID || !vboID)
			{
				vboID = toVBO();
				vaoID = toVAO();

				std::cout << "init draw" << std::endl;
			}
			

			std::cout << vaoID << std::endl;

			// if (!vaoBuffer.empty())
			// {
			// 	if (vaoBuffer.size() == 1)
			// 	{

			// 	}
			// }

			glBindVertexArray(vaoID);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			// unbind
			glBindVertexArray(0);
		}
	};

	// template <typename T>
	// using Box3_Ptr = std::make_shared<Box3<T>>;

} // namespace HSim
