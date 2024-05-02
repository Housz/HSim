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

	class LineMaterial : public Material
	{
	public:
		LineMaterial();
		LineMaterial(const Color &color_);
		~LineMaterial();

	public:
		Color color = {0.5, 0.5, 0.5}; // default color
		
	};

	class PointMaterial : public Material
	{
	public:
		PointMaterial();
		~PointMaterial();
	};

	using Material_Ptr = std::shared_ptr<Material>;
	using BasicMaterial_Ptr = std::shared_ptr<BasicMaterial>;
	using LineMaterial_Ptr = std::shared_ptr<LineMaterial>;
	using PointMaterial_Ptr = std::shared_ptr<PointMaterial>;

} // namespace HSim
