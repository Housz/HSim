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

} // namespace HSim
