#include <scene/material.h>

HSim::Shader_Ptr HSim::Material::basicShader = std::make_shared<Shader>("./resources/shaders/basic.vs", "./resources/shaders/basic.fs");

HSim::Material::Material()
{
}

HSim::Material::~Material()
{
}

HSim::BasicMaterial::BasicMaterial()
{
	shader = Material::basicShader;
}

HSim::BasicMaterial::BasicMaterial(const Color &color_, const bool wireframe_)
: color(color_), wireframe(wireframe_)
{
	shader = Material::basicShader;
}

HSim::BasicMaterial::~BasicMaterial()
{
}
