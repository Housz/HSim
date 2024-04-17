#include <scene/graphics_buffer.h>

bool HSim::GraphicsBuffer::isValid() const
{
	return id != GL_INVALID_INDEX;
}

/*************************************************************************/

void HSim::VertexArrayObject::create()
{
	glGenVertexArrays(1, &id);
}

void HSim::VertexArrayObject::release()
{
	glDeleteVertexArrays(1, &id);

	id = GL_INVALID_INDEX;
}

void HSim::VertexArrayObject::bind()
{
	glBindVertexArray(id);
}

void HSim::VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

void HSim::VertexArrayObject::bindEBO(VertexBufferObject &EBO)
{
	bind();
	if (EBO.isValid())
	{
		EBO.bind();
	}
	unbind();
}

void HSim::VertexArrayObject::bindVBO(VertexBufferObject &VBO, GLuint index, GLuint size, GLsizei stride, const void *pointer)
{
	bind();
	if (VBO.isValid())
	{
		VBO.bind();
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
		// glVertexAttribDivisor(index, divisor); // todo instanced
	}
	unbind();
}

/*************************************************************************/

void HSim::VertexBufferObject::create()
{
	glGenBuffers(1, &id);
}

void HSim::VertexBufferObject::release()
{
	glDeleteBuffers(1, &id);
	id = GL_INVALID_INDEX;
}

void HSim::VertexBufferObject::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void HSim::VertexBufferObject::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HSim::VertexBufferObject::allocate(GLuint size_, GLenum usage_)
{
	size = size_;
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, 0, usage_);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HSim::VertexBufferObject::loadData(GLvoid *data_, GLuint size_, GLuint offset_)
{
	if ((size_ + offset_) > size)
		allocate(size_ + offset_, usage);

	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, offset_, size_, data_);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*************************************************************************/

void HSim::ElementBufferObject::create()
{
	glGenBuffers(1, &id);
}

void HSim::ElementBufferObject::release()
{
	glDeleteBuffers(1, &id);
	id = GL_INVALID_INDEX;
}

void HSim::ElementBufferObject::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void HSim::ElementBufferObject::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HSim::ElementBufferObject::allocate(GLuint size_, GLenum usage_)
{
	size = size_;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, usage_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HSim::ElementBufferObject::loadData(GLvoid *data_, GLuint size_, GLuint offset_)
{
	if ((size_ + offset_) > size)
		allocate(size_ + offset_, usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset_, size_, data_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
