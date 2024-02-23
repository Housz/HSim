#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace HSim
{

	class GroundHelper
	{
	public:
		void draw()
		{
			// std::cout << "GroundHelper" << std::endl;

			std::vector<glm::vec3> vertices;
			std::vector<glm::uvec4> indices;

			size_t slices = 100;

			for (int j = 0; j <= slices; ++j)
			{
				for (int i = 0; i <= slices; ++i)
				{
					float x = (float)i;
					float y = 0;
					float z = (float)j;
					vertices.push_back(glm::vec3(x - slices / 2, y, z - slices / 2));
				}
			}

			for (int j = 0; j < slices; ++j)
			{
				for (int i = 0; i < slices; ++i)
				{

					int row1 = j * (slices + 1);
					int row2 = (j + 1) * (slices + 1);

					indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
					indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
				}
			}
			GLuint vao;

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			GLuint ibo;
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			length = (GLuint)indices.size() * 4;

			glBindVertexArray(vao);

			glLineWidth(3.0f);

			glDrawElements(GL_LINES, length, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}

		void init(float length, size_t divisions)
		{
		}

		// GLuint vao;
		size_t length;
	};

	class AABBHelper
	{
	};

	class PlaneHelper
	{
	public:
		void draw()
		{
		}
	};

	class SphereHelper
	{
	public:
		void draw()
		{
			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			size_t numSectors = 30;
			size_t numStacks = 30;

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

			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			unsigned int ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(vao);

			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

			// glPointSize(10.0f);
			// glDrawArrays(GL_POINTS, 0, vertices.size()/6);

			glBindVertexArray(0);
		}
	};

} // namespace HSim
