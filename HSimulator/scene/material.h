#pragma once

#include <HSim/common.h>
#include <HSim/color.h>
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
		Material(const Material& material_);
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
		BasicMaterial(const BasicMaterial& basicMaterial_);
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
		LineMaterial(const LineMaterial& lineMaterial_);
		LineMaterial(const Color &color_);
		~LineMaterial();

	public:
		Color color = {0.5, 0.5, 0.5}; // default color
		
	};

	class LineFieldMaterial : public Material
	{
	public:
		LineFieldMaterial();
		LineFieldMaterial(const LineFieldMaterial& lineFieldMaterial_);
		LineFieldMaterial(const Color &color_);
		~LineFieldMaterial();

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
	using LineFieldMaterial_Ptr = std::shared_ptr<LineFieldMaterial>;
	using PointMaterial_Ptr = std::shared_ptr<PointMaterial>;

} // namespace HSim
