/**
 * @file main.cpp
 * @author Housz (housz@housz.cn)
 * @brief
 * OpenGL code reference: https://learnopengl.com/
 * @date 2024-01-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "renderer/shader.h"
#include <scene/scene_graph.h>

#include <HSim/vec3.h>
#include <HSim/box3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 720;

unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

GLFWwindow *initGLFW()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HSimulator", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);
    }

    return window;
}

void initGLAD()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return -1;
    }

    glEnable(GL_DEPTH_TEST);
}

void setVBO(float *vertices, size_t size)
{
    unsigned int VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void bindData(float *vertices, size_t size)
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void updateData(float *vertices, unsigned int *indices)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initImGui(GLFWwindow *window)
{
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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void initImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void renderImGui(HSim::Shader &shader)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cleanupImGui()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void cleanupGLFW()
{
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void render(GLFWwindow *window)
{
    float vertices1[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    float vertices2[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    for (size_t i = 0; i < sizeof(vertices2); i++)
    {
        vertices2[i] += 1;
    }

    HSim::Shader shader1("./resources/shaders/camera.vs", "./resources/shaders/camera.fs");
    HSim::Shader shader2("./resources/shaders/vert.glsl", "./resources/shaders/frag.glsl");

    float uniformValueR = 0.5f;
    float uniformValueG = 0.5f;
    float uniformValueB = 0.5f;

    float uniformLightX = 5.f;
    float uniformLightY = 5.f;
    float uniformLightZ = 5.f;

    float f = 0.5f;

    float x = 0.5f;
    float y = 0.5f;
    float z = 0.5f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // std::cout << cameraPos.x << " " <<  cameraPos.y << " " << cameraPos.z << " \n" ;

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        initImGuiFrame();
        // ImGui::ShowDemoWindow(); // Show demo window! :)

        ImGui::Begin("Shader Settings");

        // 使用滑动条控制Uniform变量
        ImGui::SliderFloat("R", &uniformValueR, 0.0f, 1.0f);
        ImGui::SliderFloat("G", &uniformValueG, 0.0f, 1.0f);
        ImGui::SliderFloat("B", &uniformValueB, 0.0f, 1.0f);

        ImGui::SliderFloat("LX", &uniformLightX, 0.0f, 1.0f);
        ImGui::SliderFloat("LY", &uniformLightY, 0.0f, 1.0f);
        ImGui::SliderFloat("LZ", &uniformLightZ, 0.0f, 1.0f);

        // ImGui::SliderFloat("x", &x, 0.0f, 1.0f);
        ImGui::SliderFloat("x", vertices1 + 0, 0.0f, 1.0f);
        ImGui::SliderFloat("y", vertices1 + 1, 0.0f, 1.0f);
        ImGui::SliderFloat("z", vertices1 + 2, 0.0f, 1.0f);

        // float vertices[] = {
        //     // 0.5f, 0.5f, 0.0f,   // top right
        //     x, y, z,            // top right
        //     0.5f, -0.5f, 0.0f,  // bottom right
        //     -0.5f, -0.5f, 0.0f, // bottom left
        //     -0.5f, 0.5f, 0.0f   // top left
        // };

        ImGui::SliderFloat("f", &f, 0.0f, 1.0f);

        // updateData(vertices, indices);

        // 更新Shader的Uniform变量
        // shader.setFloat("your_uniform_name", uniformValue);

        ImGui::End();

        // input
        // -----
        processInput(window);

        // set uniforms in shader
        // -----
        // double timeValue = glfwGetTime();
        // float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        // float redValue = static_cast<float>(cos(timeValue) / 2.0 + 0.5);
        // shader.setFloat4("ourColor", redValue, greenValue, 0.0f, 0.0f);

        // render
        // ------
        glClearColor(f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // obj1
        shader1.use(); // glUseProgram(ID);
        shader1.setVec4("ourColor", uniformValueR, uniformValueG, uniformValueB, 0.0f);

        shader1.setVec3("lightPos", uniformLightX, uniformLightY, uniformLightZ);

        // VBO ------------------------------------------------------------------------
        setVBO(vertices1, sizeof(vertices1));
        // VBO ------------------------------------------------------------------------

        // VAO ------------------------------------------------------------------------
        unsigned int VAO1;

        glGenVertexArrays(1, &VAO1);
        glBindVertexArray(VAO1);

        // VAO layout 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // VAO layout 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // VAO ------------------------------------------------------------------------

        shader1.setVec3("viewPos", cameraPos);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader1.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader1.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        float angle = 10.f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader1.setMat4("model", model);

        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // obj2
        shader2.use(); // glUseProgram(ID);
        shader2.setFloat4("ourColor", uniformValueR, uniformValueG, uniformValueB, 0.0f);

        // bindData(vertices2, sizeof(vertices2));

        // VBO ------------------------------------------------------------------------
        setVBO(vertices2, sizeof(vertices2));
        // VBO ------------------------------------------------------------------------

        // VAO ------------------------------------------------------------------------
        unsigned int VAO2;

        glGenVertexArrays(1, &VAO2);
        glBindVertexArray(VAO2);

        // VAO layout 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // VAO ------------------------------------------------------------------------

        // pass projection matrix to shader (note that in this case it could change every frame)
        shader2.setMat4("projection", projection);

        // camera/view transformation
        shader2.setMat4("view", view);

        shader2.setMat4("model", model);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPointSize(10.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        renderImGui(shader1);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        // call event handler
        glfwPollEvents();
    }
}

void renderBox(GLFWwindow *window)
{
    // auto box = std::make_shared<HSim::Box3<float>>();

    // HSim::Shader shader("./resources/shaders/vert.glsl", "./resources/shaders/frag.glsl");
    HSim::Shader shader("./resources/shaders/camera.vs", "./resources/shaders/camera.fs");

    HSim::Vec3f lowerCorner = {0, 0, 0};
    HSim::Vec3f upperCorner = {1, 1, 1};

    float boxvertices[] = {
        // front
        lowerCorner.x, lowerCorner.y, lowerCorner.z, // LD
        upperCorner.x, lowerCorner.y, lowerCorner.z, // RD
        upperCorner.x, upperCorner.y, lowerCorner.z, // RU
        lowerCorner.x, upperCorner.y, lowerCorner.z, // LU

        // back
        lowerCorner.x, lowerCorner.y, upperCorner.z, // LD
        upperCorner.x, lowerCorner.y, upperCorner.z, // RD
        upperCorner.x, upperCorner.y, upperCorner.z, // RU
        lowerCorner.x, upperCorner.y, upperCorner.z  // LU
    };

    float vertices_all[] = {
        // 底部面
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

        upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

        // 顶部面
        lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

        // 其他四个面
        lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
        upperCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
        upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,

        upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,

        lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
        upperCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
        upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,

        upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
        lowerCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
        lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,

        // 左侧面
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,

        lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,

        // 右侧面
        upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,

        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f};

    unsigned int boxindices[] = {
        0, 1, 2, // front
        2, 3, 0,

        4, 5, 6, // back
        6, 7, 4,

        0, 3, 7, // left
        7, 4, 0,

        1, 2, 6, // right
        6, 5, 1,

        0, 1, 5, // down
        5, 4, 0,

        2, 3, 7, // up
        7, 6, 2};

    unsigned int vboID;
    glGenBuffers(1, &vboID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_all), vertices_all, GL_STATIC_DRAW);

    // unsigned int eboID;
    // glGenBuffers(1, &eboID);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxindices), boxindices, GL_STATIC_DRAW);

    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // VAO layout 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // VAO layout 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use(); // glUseProgram(ID);
        shader.setFloat4("ourColor", 0.5, 0.1, 0.3, 0.0f);
        shader.setVec3("lightPos", 5, 5, 5);

        shader.setVec3("viewPos", cameraPos);
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        float angle = 10.f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        glBindVertexArray(vaoID);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, boxindices);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void renderScene(GLFWwindow *window)
{
    HSim::Shader shader("./resources/shaders/camera.vs", "./resources/shaders/camera.fs");

    // scene test
    HSim::SceneGraph sg;

    auto root = std::make_shared<HSim::GameObject>();
    auto box1 = std::make_shared<HSim::Box3<float>>();
    HSim::Vec3<float> lower(1, 1, 1);
	HSim::Vec3<float> upper(5, 5, 5);
    auto box2 = std::make_shared<HSim::Box3<float>>(lower, upper);
    // root->surface_ptr = box;

    auto go1 = std::make_shared<HSim::GameObject>();
    auto go2 = std::make_shared<HSim::GameObject>();
    auto go3 = std::make_shared<HSim::GameObject>();

    go1->surface_ptr = box1;

    go3->surface_ptr = box2;

    root->drawable = false;
    go1->drawable = true;
    // go2->surface_ptr = box;
    go2->drawable = false;
    go3->drawable = true;

    // go3->surface_ptr = box;

    root->children.push_back(go1);
    root->children.push_back(go2);
    root->children.push_back(go3);

    std::function<void(HSim::GameObject_ptr)> callback_serialize = [](HSim::GameObject_ptr go)
    {
        if (go->surface_ptr != nullptr && go->drawable)
        {
            std::cout << "box" << std::endl;

            auto surface = go->surface_ptr;

            surface->serialize();
        }
    };

    std::function<void(HSim::GameObject_ptr)> callback_draw = [](HSim::GameObject_ptr go)
    {
        if (go->surface_ptr != nullptr && go->drawable)
        {
            std::cout << "drawcallback" << std::endl;

            auto surface = go->surface_ptr;

            surface->draw();
        }
    };

    sg.root = root;

    sg.traverse(callback_serialize);

    // sg.traverse(callback);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use(); // glUseProgram(ID);
        shader.setFloat4("ourColor", 0.5, 0.1, 0.3, 0.0f);
        shader.setVec3("lightPos", 20, 20, 20);

        shader.setVec3("viewPos", cameraPos);
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        float angle = 10.f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        sg.traverse(callback_draw);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main()
{
    auto window = initGLFW();

    // initImGui(window);

    initGLAD();

    // render(window);
    // renderBox(window);
    renderScene(window);

    // cleanupImGui();

    cleanupGLFW();

    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        std::cout << "w\n";
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

#include <scene/scene_graph.h>
#include <HSim/box3.h>

// void callback(HSim::GameObject_ptr go)
// {
//     if (go->surface_ptr != nullptr)
//     {
//         go->surface_ptr->serialize();
//     }
// }

int _main()
{
    HSim::SceneGraph sg;

    auto root = std::make_shared<HSim::GameObject>();
    auto box = std::make_shared<HSim::Box3<float>>();
    root->surface_ptr = box;

    auto go1 = std::make_shared<HSim::GameObject>();
    auto go2 = std::make_shared<HSim::GameObject>();
    auto go3 = std::make_shared<HSim::GameObject>();

    go1->surface_ptr = box;
    go1->drawable = true;
    go2->surface_ptr = box;
    go2->drawable = false;

    // go3->surface_ptr = box;

    root->children.push_back(go1);
    root->children.push_back(go2);
    root->children.push_back(go3);

    std::function<void(HSim::GameObject_ptr)> callback = [](HSim::GameObject_ptr go)
    {
        if (go->surface_ptr != nullptr)
        {
            go->surface_ptr->serialize();
        }
    };

    sg.root = root;

    sg.traverse(callback);

    return 0;
}
