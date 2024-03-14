#pragma once

#include <HSim/scalar_grid3.h>

namespace HSim
{

	template <typename T>
	class CellCenterScalarGrid3 : public ScalarGrid3<T>
	{
	public:
		CellCenterScalarGrid3() {}
		~CellCenterScalarGrid3() {}

		CellCenterScalarGrid3(size_t x, size_t y, size_t z)
			: ScalarGrid3<T>(x, y, z) {}

		CellCenterScalarGrid3(Vec3i resolution, Vec3<T> origin = {0, 0, 0}, Vec3<T> gridSpacing = {1, 1, 1})
			: ScalarGrid3<T>(resolution, origin, gridSpacing) {}

	public:
		Vec3i dataSize() override
		{
			return _gridResolution;
		}

		Vec3<T> dataOrigin() override
		{
			return _girdOrigin + _gridSpacing * 0.5;
		}

		// for rendering
	public:
		// draw boundary
		void drawBoundary() override
		{
			std::cout << "drawBoundary cell_center_scalar_grid3" << std::endl;
			this->aabb->draw();
		}

		// draw grid
		void drawGrid() override
		{
			// todo
			std::cout << "drawGrid cell_center_scalar_grid3" << std::endl;
		}

		// draw data
		// put all data to a VBO, glDrawArrays()
		void drawData() override
		{
			std::cout << "drawData cell_center_scalar_grid3" << std::endl;

			// vbo
			std::vector<float> vertices;

			auto func = [&](size_t i, size_t j, size_t k)
			{
				// (*grid)(i, j, k) = dice_roll;
				auto data = this->dataAt(i, j, k);
				if (data > 0)
				{
					// std::cout << "data  " << data << std::endl;
					vertices.push_back(i);
					vertices.push_back(j);
					vertices.push_back(k);

					// normal
					vertices.push_back(1);
					vertices.push_back(0);
					vertices.push_back(0);
				}
			};

			this->forEachCell(func);

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

				
			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(vao);

			std::cout << "data vao " << vao << " vbo " << vbo << std::endl;
			std::cout << "vertices.size() " << vertices.size() << std::endl;

			glPointSize(10.0f);
			glDrawArrays(GL_POINTS, 0, vertices.size()/6);
			glBindVertexArray(0);
		}
	};

} // namespace HSim
