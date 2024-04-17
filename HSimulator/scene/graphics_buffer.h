#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HSim
{
	struct GraphicsBuffer
	{
		virtual void create() = 0;
		virtual void release() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		GLuint id = GL_INVALID_INDEX;
		
		bool isValid() const;
	};

	struct VertexBufferObject;
	struct ElementBufferObject;

	// VAO
	struct VertexArrayObject : public GraphicsBuffer
	{
		void create() override;
		void release() override;

		void bind() override;
		void unbind() override;

		void bindEBO(VertexBufferObject &EBO);
		void bindVBO(
			VertexBufferObject &VBO,
			GLuint index, GLuint size, GLsizei stride, const void *pointer);

		
	};

	// VBO
	struct VertexBufferObject : public GraphicsBuffer
	{
		void create() override;
		void release() override;

		void bind() override;
		void unbind() override;

		void allocate(GLuint size_, GLenum usage_);
		void loadData(GLvoid* data_, GLuint size_, GLuint offset_);

		GLuint size;
		GLenum usage; // GL_STATIC_DRAW, GL_DYNAMIC_DRAW
	};

	// EBO
	struct ElementBufferObject : public GraphicsBuffer
	{
		void create() override;
		void release() override;

		void bind() override;
		void unbind() override;

		void allocate(GLuint size_, GLenum usage_);
		void loadData(GLvoid* data_, GLuint size_, GLuint offset_);

		GLuint size;
		GLenum usage;
	};

	

} // namespace HSim
