#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Core/Shader.h"
#include "Core/VertexArray.h"
#include "Core/Texture.h"
#include "Core/Buffer.h"
#include "Core/Camera.h"

// Functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);
void camera_look(GLFWwindow* window, bool look);

// Settings
const uint32_t screenWidth = 800;
const uint32_t screenHeight = 600;
bool bCameraInput = true;
bool bCameraMovement = true;

// Camera Stuff
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Model Position
glm::vec3 modelpos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos(0.3f, 0.0f, 2.1f);

int main()
{
	// Initialize GLFW and Make profile Core
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "Demo", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // VSync On
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Tell GLFW to Capture Mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	// Setup Dear ImGui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	glEnable(GL_DEPTH_TEST);

	//////////////////////////////////////////////////////////////////
	float vertices[] = {
		// positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

	const Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	const Shader shader_tex("res/shaders/vertex_tex.glsl", "res/shaders/fragment_tex.glsl");
	const Shader shader_light("res/shaders/vertex_light.glsl", "res/shaders/fragment_light.glsl");
	const Shader shader_maps("res/shaders/vertex_mat_map.glsl", "res/shaders/fragment_mat_map.glsl");
	
	const Texture diffuseMap("res/textures/container2.png");
	const Texture specularMap("res/textures/container2_specular.png");

	const VertexBuffer VBO(vertices, sizeof(vertices));
	const VertexArray cubeVAO;
	const VertexArray lightVAO;

	//////////
	// Cube //
	//////////
	cubeVAO.Bind();
	VBO.Bind();

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	////////////////
	// Light Cube //
	////////////////
	lightVAO.Bind();
	VBO.Bind();

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Shader light properties
	glm::vec3 ambientLight = { 0.5f, 0.5f, 0.5f };
	glm::vec3 diffuseLight = { 0.2f, 0.2f, 0.2f };
	glm::vec3 specularLight = { 1.0f,1.0f,1.0f };
	float shininess = 64.0f;

	shader_maps.Bind();
	shader_maps.SetUniformInt("material.diffuse", 0);
	shader_maps.SetUniformInt("material.specular", 1);

	// Render Loop
	while(!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Render
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ImGui New Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Shader settings
		shader_maps.Bind();
		shader_maps.SetUniformFloat3("light.position" , lightPos);
		shader_maps.SetUniformFloat3("viewPos", camera.Position);

		shader_maps.SetUniformFloat3("light.ambient", ambientLight);
		shader_maps.SetUniformFloat3("light.diffuse", diffuseLight);
		shader_maps.SetUniformFloat3("light.specular", specularLight);

		shader_maps.SetUniformFloat("material.shininess", shininess);

		// View/projection transformation
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader_maps.SetUniformMat4("projection", projection);
		shader_maps.SetUniformMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, modelpos);
		shader_maps.SetUniformMat4("model", model);

		diffuseMap.Bind(0);
		specularMap.Bind(1);

		// draw cube
		cubeVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw light cube
		shader.Bind();
		shader.SetUniformMat4("projection", projection);
		shader.SetUniformMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		shader.SetUniformMat4("model", model);

		lightVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		ImGui::Begin("Test Menu");
		{
			if (ImGui::Button("Camera Look"))
			{
				camera_look(window, true);
			}
			ImGui::Separator();
			ImGui::Text("Cube Properties");
			if (ImGui::CollapsingHeader("Position"))
			{
				ImGui::SliderFloat3("Position(Light)", value_ptr(lightPos), -5, 5);
				ImGui::SliderFloat3("Position(Cube)", value_ptr(modelpos), -5, 5);
			}
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Event and Buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		camera_look(window, false);
	}

	if(bCameraMovement)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void camera_look(GLFWwindow* window, bool look) {
	if(look)
	{
		bCameraInput = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	if(!look)
	{
		bCameraInput = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	glm::vec2 position;
	position.x = static_cast<float>(xPos);
	position.y = static_cast<float>(yPos);

	if (firstMouse) {
		lastX = position.x;
		lastY = position.y;
		firstMouse = false;
	}
	float xoffset = position.x - lastX;
	float yoffset = lastY - position.y;

	lastX = position.x;
	lastY = position.y;
	if(bCameraInput)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}
