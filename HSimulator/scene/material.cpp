#include <scene/material.h>
#include "material.h"
#include "material.h"
#include "material.h"

// HSim::Shader_Ptr HSim::Material::basicShader = std::make_shared<HSim::Shader>("./resources/shaders/basic.vs", "./resources/shaders/basic.fs");


HSim::Material::Material()
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
HSim::LineMaterial::LineMaterial(const Color &color_)
: color(color_)
{
	shader = std::make_shared<Shader>("./resources/shaders/line.vs", "./resources/shaders/line.fs");
}
HSim::LineMaterial::~LineMaterial()
{
}