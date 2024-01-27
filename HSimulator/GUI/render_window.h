#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GUI/orbit_camera.h>
#include <GUI/input.h>
#include <renderer/renderer.h>
#include <renderer/render_params.h>
#include <scene/scene_graph.h>
#include <scene/helper_object.h>

namespace HSim
{
	enum GLFWButtonState
	{
		GLFW_DOWN = 0,
		GLFW_UP
	};

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();

	public:
		void init(size_t width, size_t height);

		void mainLoop();

		void setScene(SceneGraph_ptr scene_);

		// interactions
	public:
		void initCallbacks();

		// callbacks for keyboard and mouse interactions
		static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void reshapeCallback(GLFWwindow *window, int w, int h);
		static void cursorPosCallback(GLFWwindow *window, double x, double y);
		static void cursorEnterCallback(GLFWwindow *window, int entered);
		static void scrollCallback(GLFWwindow *window, double offsetX, double OffsetY);

		void (*mMouseButtonFunc)(GLFWwindow *window, int button, int action, int mods);
		void (*mKeyboardFunc)(GLFWwindow *window, int key, int scancode, int action, int mods);
		void (*mReshapeFunc)(GLFWwindow *window, int w, int h);
		void (*mCursorPosFunc)(GLFWwindow *window, double x, double y);
		void (*mCursorEnterFunc)(GLFWwindow *window, int entered);
		void (*mScrollFunc)(GLFWwindow *window, double offsetX, double OffsetY);

		// cursor position
		float cursorPosX;
		float cursorPosY;
		double mCursorTempX = -1.0;

		// mouse state
		HMouseButtonType buttonType;
		HActionType buttonAction;
		HModifierKeyBits mdkeyBits;

		GLFWButtonState buttonState;

		// toggles
		bool animationToggle = false;

		void toggleAnimation()
		{
			std::cout << "toggleAnimation to : " << animationToggle << std::endl;
			animationToggle = !animationToggle;
			//todo selection state update 
		}

	// glfw
	public:
		GLFWwindow *glfwWindow = nullptr;

	public:
		std::string windowTitle;

	public:
		Renderer_ptr renderer; // mainloop(): call renderer.draw(sceneptr)

		RenderParams renderParams;

		OrbitCamera_ptr camera;
	
	public:
		SceneGraph_ptr scene;

	};

	using RenderWindow_ptr = std::shared_ptr<RenderWindow>;

} // namespace HSim
