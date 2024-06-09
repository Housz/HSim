#pragma once

#include <HSim/scalar_grid3.h>

namespace HSim
{

	template <typename T>
	class CellCenterScalarGrid3 : public ScalarGrid3<T>
	{
	public:
		CellCenterScalarGrid3() {}

		CellCenterScalarGrid3(const CellCenterScalarGrid3<T> &cellCenterScalarGrid3_)
			: ScalarGrid3<T>(cellCenterScalarGrid3_)
		{
		}

		CellCenterScalarGrid3(size_t x, size_t y, size_t z)
			: ScalarGrid3<T>(x, y, z) {}
		
		// CellCenterScalarGrid3(const Size3 &resolution)
		// 	: ScalarGrid3<T>(resolution) {}

		CellCenterScalarGrid3(const Size3 &resolution, const Vec3<T> &gridOrigin = {0, 0, 0}, const Vec3<T> &gridSpacing = {1, 1, 1})
			: ScalarGrid3<T>(resolution, gridOrigin, gridSpacing) {}

		~CellCenterScalarGrid3() {}

		void clone(std::shared_ptr<SpaceObject3<T>> &ptr) override
		{
			ptr = std::make_shared<CellCenterScalarGrid3<T>>(*this);
		}

	public:
		Vec3i dataSize() const override
		{
			return gridResolution;
		}

		Vec3<T> dataOrigin() const override
		{
			return gridOrigin + gridSpacing * 0.5;
		}

		Vec3<T> positionAt(size_t i, size_t j, size_t k) const override
		{
			return gridOrigin + gridSpacing * 0.5 + Vec3<T>(i * gridSpacing.x, j * gridSpacing.y, k * gridSpacing.z);
		}

		T sample(const Vec3<T> &p) const override
		{
			size_t i = (size_t)((p.x - .5 * deltaX()) / deltaX());
			size_t j = (size_t)((p.y - .5 * deltaY()) / deltaY());
			size_t k = (size_t)((p.z - .5 * deltaZ()) / deltaZ());

			// if (!(i < gridResolution.x && j < gridResolution.y && k < gridResolution.z))
			// {
			// 	std::cout << p;
			// 	std::cout << i << " " << j << " " << k << "\n";
			// }
			i = clamp(i, 0, gridResolution.x - 1);
			j = clamp(j, 0, gridResolution.y - 1);
			k = clamp(k, 0, gridResolution.z - 1);
			// std::cout << i << " " << j << " " << k << "\n";

			auto t = p - positionAt(i, j, k);

			return lerp(dataAt(i, j, k), dataAt(i + 1, j, k), dataAt(i, j + 1, k), dataAt(i + 1, j + 1, k),
						dataAt(i, j, k + 1), dataAt(i + 1, j, k + 1), dataAt(i, j + 1, k + 1), dataAt(i + 1, j + 1, k + 1),
						t.x, t.y, t.z);
		}

		// for rendering
	public:
		// draw boundary
		void drawBoundary() override
		{
			std::cout << "drawBoundary cell_center_scalar_grid3" << std::endl;
			this->aabb->draw(); // set an updated flag?
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

			std::vector<float> vertices;

			auto func = [&](size_t i, size_t j, size_t k)
			{
				auto data = this->dataAt(i, j, k);
				if (data > 0)
				{
					vertices.push_back(i);
					vertices.push_back(j);
					vertices.push_back(k);
				}
			};

			this->forEachCell(func);

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
			// layout 1: normals
			// glEnableVertexAttribArray(1);
			// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			glBindVertexArray(0);
			// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			// glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(vao);

			std::cout << "data vao " << vao << " vbo " << vbo << std::endl;
			// std::cout << "vertices.size() " << vertices.size() << std::endl;

			glPointSize(4.0f);
			glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
			glBindVertexArray(0);
		}
	};

	template <typename T>
	using CellCenterScalarGrid3_Ptr = std::shared_ptr<CellCenterScalarGrid3<T>>;

} // namespace HSim
