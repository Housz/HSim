#include "render_window.h"

HSim::RenderWindow::RenderWindow()
{

}

HSim::RenderWindow::~RenderWindow()
{
	// imgui clean

	// glfw destroy
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void HSim::RenderWindow::init(size_t width, size_t height)
{
	//glfw create window
	windowTitle = std::string("HSimulator");

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindow = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	if (glfwWindow == NULL)
		return;

	initCallbacks();

	glfwMakeContextCurrent(glfwWindow);
		
	if (!gladLoadGL()) {
		std::cout << "Failed to load GLAD!" << std::endl;
		//SPDLOG_CRITICAL("Failed to load GLAD!");
		exit(-1);
	}

	glfwSwapInterval(1); // Enable vsync

	glfwSetWindowUserPointer(glfwWindow, this);

// imgui

	// Get Context scale
	float xscale, yscale;
	glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);

	// initialize rendering engine
	renderer->init();

	camera->setWidth(width);
	camera->setHeight(height);
}

void HSim::RenderWindow::mainLoop()
{
	// renderer.draw()

	// main loop
	while (!glfwWindowShouldClose(glfwWindow))
	{
		glfwPollEvents();

		if (animationToggle)
		{
			// advance ani
		}

		// camera
		camera->viewportWidth();
		camera->viewportHeight();

		// renderer draw scenegraph;

		// imgui


		

	}


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
	RenderWindow* activeWindow = (RenderWindow*)glfwGetWindowUserPointer(window);

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
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_SPACE:
			activeWindow->toggleAnimation();
			break;
		case GLFW_KEY_LEFT:
			break;
		case GLFW_KEY_RIGHT:
			break;
		case GLFW_KEY_UP:
			break;
		case GLFW_KEY_DOWN:
			break;
		case GLFW_KEY_PAGE_UP:
			break;
		case GLFW_KEY_PAGE_DOWN:
			break;
		case GLFW_KEY_N:
// scene update 
			// activeScene->takeOneFrame();
			// activeScene->updateGraphicsContext();
			break;
		case GLFW_KEY_F1:
// imgui
			// activeWindow->toggleImGUI();
			break;
		default:
			break;
	}

}

void HSim::RenderWindow::reshapeCallback(GLFWwindow *window, int w, int h)
{
	RenderWindow* activeWindow = (RenderWindow*)glfwGetWindowUserPointer(window);
	activeWindow->camera->setWidth(w);
	activeWindow->camera->setHeight(h);
}

void HSim::RenderWindow::cursorPosCallback(GLFWwindow *window, double x, double y)
{
	RenderWindow* activeWindow = (RenderWindow*)glfwGetWindowUserPointer(window);
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
		camera->rotateToPoint(x, y);
	}
	else if (
		activeWindow->buttonType == GLFW_MOUSE_BUTTON_MIDDLE &&
		activeWindow->buttonState == GLFW_DOWN && 
		activeWindow->mdkeyBits == GLFW_MOD_ALT 
		// && !activeWindow->mImWindow.cameraLocked()
		) 
	{
		camera->translateToPoint(x, y);
	}
	else if (
		activeWindow->buttonType == GLFW_MOUSE_BUTTON_RIGHT &&
		activeWindow->buttonState == GLFW_DOWN &&
		activeWindow->mdkeyBits == GLFW_MOD_ALT 
		// && !activeWindow->mImWindow.cameraLocked()
		)////
	{
		if (activeWindow->mCursorTempX != -1) 
		{
			camera->zoom(-0.005 * (x - activeWindow->mCursorTempX));
			activeWindow->mCursorTempX = x;
		}
	}
	// activeWindow->imWindow()->mouseMoveEvent(mouseEvent); 	
}	

void HSim::RenderWindow::scrollCallback(GLFWwindow *window, double offsetX, double OffsetY)
{
	RenderWindow* activeWindow = (RenderWindow*)glfwGetWindowUserPointer(window);
	auto camera = activeWindow->camera;

// imgui
	// if (!activeWindow->mImWindow.cameraLocked())
	// {
		int state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
		int altState = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
		//If the left control key is pressed, slow the zoom speed. 
		if (state == GLFW_PRESS && altState == GLFW_PRESS)
			camera->zoom(-0.1*OffsetY);
		else if (altState == GLFW_PRESS)
			camera->zoom(-OffsetY);
	// }

	
}

void HSim::RenderWindow::cursorEnterCallback(GLFWwindow *window, int entered)
{
	// todo
}
