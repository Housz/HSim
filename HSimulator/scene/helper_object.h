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
			std::cout << "GroundHelper" << std::endl;

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
					vertices.push_back(glm::vec3(x - slices/2, y, z- slices/2));
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

		GLuint vao;
		size_t length;
	};

	class AABBHelper
	{
	};

} // namespace HSim
