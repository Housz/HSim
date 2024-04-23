#pragma once

#include <HSim/common.h>
#include <scene/color.h>
#include <renderer/shader.h>

namespace HSim
{
	enum class RenderingType
	{
		NAIVE,
		FLAT,
		SMOOTH
	};
	
	class Material
	{
	public:
		Material();
		~Material();

	public:
		Shader_Ptr shader = nullptr;

		// static Shader_Ptr basicShader;
		// todo: more types of shader
	};

	class BasicMaterial : public Material
	{
	public:
		BasicMaterial();
		BasicMaterial(const Color &color_, const bool wireframe_);
		~BasicMaterial();

	public:
		Color color = {0.5, 0.5, 0.5}; // default color
		bool wireframe = false;

		RenderingType renderingType = RenderingType::NAIVE;
	};

	using Material_Ptr = std::shared_ptr<Material>;
	using BasicMaterial_Ptr = std::shared_ptr<BasicMaterial>;

} // namespace HSim
