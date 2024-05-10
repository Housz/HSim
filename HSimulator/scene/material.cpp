#include <scene/material.h>
#include "material.h"

// HSim::Shader_Ptr HSim::Material::basicShader = std::make_shared<HSim::Shader>("./resources/shaders/basic.vs", "./resources/shaders/basic.fs");


HSim::Material::Material()
{
}

HSim::Material::Material(const Material &material_)
	: shader(material_.shader)
{
}

HSim::Material::~Material()
{
}

/********************************************************************/

HSim::BasicMaterial::BasicMaterial()
{
	// shader = Material::basicShader;
	shader = std::make_shared<Shader>("./resources/shaders/basic.vs", "./resources/shaders/basic.fs");
	// shader = _basicShader;
}

HSim::BasicMaterial::BasicMaterial(const BasicMaterial &basicMaterial_)
	: Material(basicMaterial_), 
	  color(basicMaterial_.color), 
	  wireframe(basicMaterial_.wireframe),
	  renderingType(basicMaterial_.renderingType)
{
}

HSim::BasicMaterial::BasicMaterial(const Color &color_, const bool wireframe_)
: color(color_), wireframe(wireframe_)
{
	// shader = Material::basicShader;
	shader = std::make_shared<Shader>("./resources/shaders/basic.vs", "./resources/shaders/basic.fs");
	// shader = _basicShader;


}

HSim::BasicMaterial::~BasicMaterial()
{
}

/********************************************************************/

HSim::LineMaterial::LineMaterial()
{
	shader = std::make_shared<Shader>("./resources/shaders/line.vs", "./resources/shaders/line.fs");
}

HSim::LineMaterial::LineMaterial(const LineMaterial &lineMaterial_)
	: Material(lineMaterial_), color(lineMaterial_.color)
{
}

HSim::LineMaterial::LineMaterial(const Color &color_)
	: color(color_)
{
	shader = std::make_shared<Shader>("./resources/shaders/line.vs", "./resources/shaders/line.fs");
}
HSim::LineMaterial::~LineMaterial()
{
}

/********************************************************************/

HSim::PointMaterial::PointMaterial()
{
	shader = std::make_shared<Shader>("./resources/shaders/point.vs", "./resources/shaders/point.fs");
}

HSim::PointMaterial::~PointMaterial()
{
}
