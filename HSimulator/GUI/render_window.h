#pragma once 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/renderer.h>
#include <GUI/orbit_camera.h>
#include <renderer/render_params.h>

namespace HSim
{

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();

	public:
		void init(size_t width, size_t height);

		void mainLoop();

	// glfw 
	public:
		GLFWwindow* glfwWindow = nullptr;
	

	// callbacks for keyboard and mouse interactions
	public:
		


	public:
		Renderer_ptr renderer;

		RenderParams renderParams;

		OrbitCamera_ptr camera;
	};
	
	using RenderWindow_ptr = std::shared_ptr<RenderWindow>;
	

} // namespace HSim
