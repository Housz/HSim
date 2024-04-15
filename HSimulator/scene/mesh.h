#pragma once

#include <HSim/surface3.h>
#include <HSim/grid3.h>
#include <scene/material.h>
#include <config/numerical_config.h>

namespace HSim
{
	class Mesh
	{
	public:
		Mesh(){};
		~Mesh(){};

	public:
		void draw();

	public:
		// surface ptr
		std::shared_ptr<Surface3<PRECISION>> surface_ptr = nullptr;

		// grid ptr
		std::shared_ptr<Grid3<PRECISION>> grid_ptr = nullptr;

		Material_ptr material = nullptr;

		// id  vaoID
	};

	class SurfaceMesh : public Mesh
	{
	public:
		std::shared_ptr<Surface3<PRECISION>> surface_ptr = nullptr;
	};

	class GridMesh : public Mesh
	{
	public:
		std::shared_ptr<Grid3<PRECISION>> grid_ptr = nullptr;
	};

	class HelperMesh : public Mesh
	{
	public:
		// helper ptr
	};

} // namespace HSim
