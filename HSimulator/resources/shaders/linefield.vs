#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// out vec3 FragPos;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// FragPos = vec3(model * vec4(aPos, 1.0));
	Color = vec4(aColor, 1.0);
	 
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}