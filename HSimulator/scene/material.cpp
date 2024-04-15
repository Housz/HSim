#include "material.h"

HSim::Material::Material()
{
}

HSim::Material::~Material()
{
}

HSim::BasicMaterial::BasicMaterial()
{
}

HSim::BasicMaterial::BasicMaterial(const Color &color_, const bool wireframe_)
: color(color_), wireframe(wireframe_)
{

}

HSim::BasicMaterial::~BasicMaterial()
{
}
