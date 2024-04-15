#pragma once

#include <HSim/common.h>
#include <scene/color.h>

namespace HSim
{
	class Material
	{
	public:
		Material();
		~Material();

	};

	class BasicMaterial : public Material
	{
	public:
		BasicMaterial();
		BasicMaterial(const Color& color_, const bool wireframe);
		~BasicMaterial();
	
	public:
		Color color;
		bool wireframe = false;
	};
	
	using Material_ptr = std::shared_ptr<Material>;
	
	
} // namespace HSim
