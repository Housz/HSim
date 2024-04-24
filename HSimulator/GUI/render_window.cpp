#include <GUI/render_window.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

HSim::RenderWindow::RenderWindow()
{
}

HSim::RenderWindow::~RenderWindow()
{
	// imgui clean

	// glfw destroy
}

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void HSim::RenderWindow::init(size_t width, size_t height)
{
	this->width = width;
	this->height = height;

	renderer = std::make_shared<Renderer>();

	camera = std::make_shared<OrbitCamera>();

	std::cout << "-- renderwinodw init" << std::endl;

	// glfw create window
	windowTitle = std::string("HSimulator");

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindow = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// if (glfwWindow == NULL)
	// 	return;

	if (glfwWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		// return -1;
	}

	GLFWimage images[1];
	images[0].pixels = stbi_load("./resources/imgs/nobita64.png", &images[0].width, &images[0].height, 0, 4); // rgba channels

	if (!images[0].pixels)
	{
		fprintf(stderr, "Failed to load icon image\n");
		// glfwTerminate();
		// return -1;
	}
	else
	{
		glfwSetWindowIcon(glfwWindow, 1, images);
		stbi_image_free(images[0].pixels);
	}

	initCallbacks();

	glfwMakeContextCurrent(glfwWindow);

	// if (!gladLoadGL()) {
	// 	std::cout << "Failed to load GLAD!" << std::endl;
	// 	//SPDLOG_CRITICAL("Failed to load GLAD!");
	// 	exit(-1);
	// }

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

		exit(-1);
		// return -1;
	}
	glEnable(GL_DEPTH_TEST);

	glfwSwapInterval(1); // Enable vsync

	glfwSetWindowUserPointer(glfwWindow, this);

	/********************** imgui *************************/
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init();
	/********************** imgui *************************/

	// Get Context scale
	float xscale, yscale;
	glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);

	// initialize rendering engine
	renderer->init();

	camera->setWidth(this->width);
	camera->setHeight(this->height);
}

void HSim::RenderWindow::mainLoop()
{
	// std::cout << "window.mainLoop() in" << std::endl;

	scene->serialize();

	// timing
	float currentFrame = 0.0f;
	float deltaTime = 0.0f; // time between current frame and last frame
	float lastFrame = 0.0f;

	// opengl main loop
	while (!glfwWindowShouldClose(glfwWindow))
	{
		// std::cout << "main loop" << std::endl;

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// std::cout << "deltaTime: " << deltaTime << std::endl;
		// std::cout << "FPS: " << 1.0 / deltaTime << std::endl;



		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(.667f, 0.8f, 1.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (animationToggle)
		{
			// advance ani
		}

		// camera state -> renderParams

		renderParams.width = camera->viewportWidth();
		renderParams.height = camera->viewportHeight();
		renderParams.transforms.model = glm::mat4(1);
		renderParams.transforms.view = camera->getViewMat();
		renderParams.transforms.proj = camera->getProjMat();

		// renderer draw scenegraph;
		// renderer draw(scenegraph, renderParams)

		renderer->draw(renderParams);

		
		/********************** imgui *************************/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)

		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/********************** imgui *************************/

		// imgui

		// ground.draw();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void HSim::RenderWindow::setScene(SceneGraph_ptr scene_)
{
	this->scene = scene_;

	this->renderer->setScene(scene_);
}

void HSim::RenderWindow::initCallbacks()
{
	mMouseButtonFunc = RenderWindow::mouseButtonCallback;
	mKeyboardFunc = RenderWindow::keyboardCallback;
	mReshapeFunc = RenderWindow::reshapeCallback;
	mCursorPosFunc = RenderWindow::cursorPosCallback;
	mCursorEnterFunc = RenderWindow::cursorEnterCallback;
	mScrollFunc = RenderWindow::scrollCallback;

	glfwSetMouseButtonCallback(glfwWindow, mMouseButtonFunc);
	glfwSetKeyCallback(glfwWindow, mKeyboardFunc);
	glfwSetFramebufferSizeCallback(glfwWindow, mReshapeFunc);
	glfwSetCursorPosCallback(glfwWindow, mCursorPosFunc);
	glfwSetCursorEnterCallback(glfwWindow, mCursorEnterFunc);
	glfwSetScrollCallback(glfwWindow, mScrollFunc);
}

void HSim::RenderWindow::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	RenderWindow *activeWindow = (RenderWindow *)glfwGetWindowUserPointer(window);

	// handle picking
	// if (activeWindow->getButtonType() == GLFW_MOUSE_BUTTON_LEFT &&
	// 	activeWindow->getButtonAction() == GLFW_PRESS &&
	// 	activeWindow->getButtonMode() == 0 &&
	// 	action == GLFW_RELEASE)
	// {

	// 	// in picking
	// 	int x = fmin(xpos, activeWindow->getCursorPosX());
	// 	int y = fmax(ypos, activeWindow->getCursorPosY());
	// 	int w = fabs(xpos - activeWindow->getCursorPosX());
	// 	int h = fabs(ypos - activeWindow->getCursorPosY());
	// 	// flip y to texture space...
	// 	y = activeWindow->getHeight() - y - 1;

	// 	const auto &selection = activeWindow->select(x, y, w, h);
	// }

	// auto camera = activeWindow->getCamera();
	auto camera = activeWindow->camera;

	// activeWindow->setButtonType(button);
	// activeWindow->setButtonAction(action);
	// activeWindow->setButtonMode(mods);
	activeWindow->buttonType = (HMouseButtonType)button;
	activeWindow->buttonAction = (HActionType)action;
	activeWindow->mdkeyBits = (HModifierKeyBits)mods;

	HMouseEvent mouseEvent;
	mouseEvent.ray = camera->castRayInWorldSpace((float)xpos, (float)ypos);
	mouseEvent.mouseButtonType = (HMouseButtonType)button;
	mouseEvent.mouseAction = (HActionType)action;
	mouseEvent.mdkeyBits = (HModifierKeyBits)mods;
	mouseEvent.camera = camera;
	mouseEvent.x = (float)xpos;
	mouseEvent.y = (float)ypos;

	// scene handle event
	// auto activeScene = SceneGraphFactory::instance()->active();

	// activeScene->onMouseEvent(mouseEvent);

	if (action == GLFW_PRESS)
	{
		// std::cout << "GLFW_PRESS" << std::endl;
		// if(mOpenCameraRotate)
		camera->registerPoint((float)xpos, (float)ypos);
		// activeWindow->setButtonState(GLFW_DOWN);
		activeWindow->buttonState = GLFW_DOWN;

		// imgui
		// activeWindow->imWindow()->mousePressEvent(mouseEvent);
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (action == GLFW_RELEASE)
	{
		// std::cout << "GLFW_RELEASE" << std::endl;
		// activeWindow->setButtonState(GLFW_UP);
		activeWindow->buttonState = GLFW_UP;

		// imgui
		// activeWindow->imWindow()->mouseReleaseEvent(mouseEvent);
	}

	// update cursor position record

	if (action == GLFW_PRESS)
	{
		// activeWindow->setCursorPos(xpos, ypos);
		activeWindow->cursorPosX = xpos;
		activeWindow->cursorPosY = ypos;

		activeWindow->mCursorTempX = xpos;
	}
	else
	{
		// activeWindow->setCursorPos(-1, -1);
		activeWindow->cursorPosX = -1;
		activeWindow->cursorPosY = -1;
		activeWindow->mCursorTempX = -1;
	}
}

void HSim::RenderWindow::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	RenderWindow *activeWindow = (RenderWindow *)glfwGetWindowUserPointer(window);

	HKeyboardEvent keyEvent;
	keyEvent.keyType = (HKeyboardType)key;
	keyEvent.keyActionType = (HActionType)action;
	keyEvent.mdkeyBits = (HModifierKeyBits)mods;

	// scene handle event
	// auto activeScene = SceneGraphFactory::instance()->active();
	// activeScene->onKeyboardEvent(keyEvent);

	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		std::cout << "GLFW_KEY_ESCAPE" << std::endl;
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_SPACE:
		std::cout << "GLFW_KEY_SPACE" << std::endl;
		activeWindow->toggleAnimation();
		break;
	case GLFW_KEY_LEFT:
		std::cout << "GLFW_KEY_LEFT" << std::endl;
		break;
	case GLFW_KEY_RIGHT:
		std::cout << "GLFW_KEY_RIGHT" << std::endl;
		break;
	case GLFW_KEY_UP:
		std::cout << "GLFW_KEY_UP" << std::endl;
		break;
	case GLFW_KEY_DOWN:
		std::cout << "GLFW_KEY_DOWN" << std::endl;
		break;
	case GLFW_KEY_PAGE_UP:
		std::cout << "GLFW_KEY_PAGE_UP" << std::endl;
		break;
	case GLFW_KEY_PAGE_DOWN:
		std::cout << "GLFW_KEY_PAGE_DOWN" << std::endl;
		break;
	case GLFW_KEY_N:
		std::cout << "GLFW_KEY_N" << std::endl;
		// scene update
		// activeScene->takeOneFrame();
		// activeScene->updateGraphicsContext();
		break;
	case GLFW_KEY_F1:
		std::cout << "GLFW_KEY_F1" << std::endl;
		// imgui
		// activeWindow->toggleImGUI();
		break;
	default:
		break;
	}
}

void HSim::RenderWindow::reshapeCallback(GLFWwindow *window, int w, int h)
{
	std::cout << "reshape: w = " << w << "  h = " << h << std::endl;
	RenderWindow *activeWindow = (RenderWindow *)glfwGetWindowUserPointer(window);
	activeWindow->camera->setWidth(w);
	activeWindow->camera->setHeight(h);
}

void HSim::RenderWindow::cursorPosCallback(GLFWwindow *window, double x, double y)
{
	// std::cout << "cursorPos: x = " << x << "  y = " << y << std::endl;
	RenderWindow *activeWindow = (RenderWindow *)glfwGetWindowUserPointer(window);
	auto camera = activeWindow->camera;

	HMouseEvent mouseEvent;
	mouseEvent.ray = camera->castRayInWorldSpace((float)x, (float)y);
	mouseEvent.mouseButtonType = (HMouseButtonType)activeWindow->buttonType;
	mouseEvent.mouseAction = HActionType::ACTION_REPEAT;
	mouseEvent.mdkeyBits = (HModifierKeyBits)activeWindow->mdkeyBits;
	mouseEvent.camera = camera;
	mouseEvent.x = (float)x;
	mouseEvent.y = (float)y;

	// scene handle event
	// auto activeScene = SceneGraphFactory::instance()->active();
	// activeScene->onMouseEvent(mouseEvent);

	if (activeWindow->buttonType == GLFW_MOUSE_BUTTON_LEFT &&
		activeWindow->buttonState == GLFW_DOWN &&
		activeWindow->mdkeyBits == GLFW_MOD_ALT
		// &&!activeWindow->mImWindow.cameraLocked()
	)
	{
		// std::cout << "camera->rotateToPoint(x, y);" << x << " " << y << std::endl;
		camera->rotateToPoint(x, y);
	}
	else if (
		activeWindow->buttonType == GLFW_MOUSE_BUTTON_RIGHT &&
		activeWindow->buttonState == GLFW_DOWN &&
		activeWindow->mdkeyBits == GLFW_MOD_ALT
		// && !activeWindow->mImWindow.cameraLocked()
	)
	{
		// std::cout << "camera->translateToPoint(x, y);" << std::endl;
		camera->translateToPoint(x, y);
	}
	else if (
		activeWindow->buttonType == GLFW_MOUSE_BUTTON_MIDDLE &&
		activeWindow->buttonState == GLFW_DOWN &&
		activeWindow->mdkeyBits == GLFW_MOD_ALT
		// && !activeWindow->mImWindow.cameraLocked()
		) ////
	{
		if (activeWindow->mCursorTempX != -1)
		{
			// std::cout << "camera->zoom(-0.005 * (x - activeWindow->mCursorTempX));" << std::endl;

			camera->zoom(-0.005 * (x - activeWindow->mCursorTempX));
			activeWindow->mCursorTempX = x;
		}
	}
	// activeWindow->imWindow()->mouseMoveEvent(mouseEvent);
}

void HSim::RenderWindow::scrollCallback(GLFWwindow *window, double offsetX, double OffsetY)
{
	// std::cout << "scroll: offsetX = " << offsetX << " offsetY = " << OffsetY << std::endl;
	RenderWindow *activeWindow = (RenderWindow *)glfwGetWindowUserPointer(window);
	auto camera = activeWindow->camera;

	// imgui
	// if (!activeWindow->mImWindow.cameraLocked())
	// {
	int state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	int altState = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
	// If the left control key is pressed, slow the zoom speed.
	if (state == GLFW_PRESS && altState == GLFW_PRESS)
		camera->zoom(-0.1 * OffsetY);
	else if (altState == GLFW_PRESS)
		camera->zoom(-OffsetY);
	// }
}

void HSim::RenderWindow::cursorEnterCallback(GLFWwindow *window, int entered)
{
	// todo

	// cursor in window
}
