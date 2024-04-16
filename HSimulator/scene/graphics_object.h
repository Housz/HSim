#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HSim
{
	struct GraphicsObject
	{
		virtual void create() = 0;
		virtual void release() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		unsigned int id;
	};

	struct VertexArrayObject : public GraphicsObject
	{

	};

	struct VertexBufferObject : public GraphicsObject
	{

	};

	struct ElementBufferObject : public GraphicsObject
	{
		
	};

} // namespace HSim
