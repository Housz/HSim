#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>

namespace HSim
{
	/**
	 * @brief 3-D Axis-aligned bounding box
	 */
	template <typename T>
	class AABB3
	{
	public:
		AABB3(){};
		~AABB3(){};

		AABB3(const Vec3<T> &lowerCorner_, const Vec3<T> &upperCorner_)
			: lowerCorner(lowerCorner_), upperCorner(upperCorner_)
		{
            // std::cout << "--------------------------------------------------\n";
			// std::cout << lowerCorner;
			// std::cout << upperCorner;
			// std::cout << lowerCorner_;
			// std::cout << upperCorner_;
		}

		AABB3(const AABB3<T> &boundingBox_)
			: lowerCorner(boundingBox_.lowerCorner), upperCorner(boundingBox_.upperCorner) {}

		/**
		 * @brief set BoundingBox3 with any two points
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

	public:
		bool isOverlap(const AABB3<T> &b_) const
		{
			if (upperCorner.x < b_.lowerCorner.x || lowerCorner.x > b_.upperCorner.x ||
				upperCorner.y < b_.lowerCorner.y || lowerCorner.y > b_.upperCorner.y ||
				upperCorner.z < b_.lowerCorner.z || lowerCorner.z > b_.upperCorner.z)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		bool isInside(const Vec3<T> &position_) const
		{
			auto p_ = position_;
			if (p_.x < lowerCorner.x || p_.y < lowerCorner.y || p_.z < lowerCorner.z ||
				p_.x > upperCorner.x || p_.y > upperCorner.y || p_.z > upperCorner.z)
			{
				return false;
			}
			else
				return true;
		}

	public:
		Vec3<T> lowerCorner = {0, 0, 0};
		Vec3<T> upperCorner = {1, 1, 1};

		// for rendering
	public:
		unsigned int vaoID = 0;
		unsigned int vboID = 0;

		size_t toVBO()
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

			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			std::cout << "AABB vbo " << std::endl;

			return vboID;
		}

		size_t toVAO()
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

		void draw()
		{
			// std::cout << "AABB draw" << std::endl;
			// std::cout << lowerCorner;
			// std::cout << upperCorner;

			if (!vaoID || !vboID)
			{
				vboID = toVBO();
				vaoID = toVAO();

				std::cout << "init draw" << std::endl;
			}

			std::cout << vaoID << std::endl;

			glBindVertexArray(vaoID);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			// unbind
			glBindVertexArray(0);
		}
	};

	template <typename T>
	using AABB3_Ptr = std::shared_ptr<AABB3<T>>;

} // namespace HSim
