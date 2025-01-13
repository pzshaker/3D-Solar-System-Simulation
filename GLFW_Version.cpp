#include <glm/ext.hpp>
#include<iostream>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Sphere.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <ctime>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void renderPlanet(const glm::vec3& color, float orbitDistance, float scale, int planetIndex);
void renderSaturnWithRing(float orbitDistance, float scale, GLuint saturnTextureID, GLuint ringTextureID);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Sphere planet(0.05);

glm::mat4 model, view, projection;

GLuint vao, vbo, ibo, vPosition, vNormal, vTexCoord;
GLint texLoc;

float selfRotate = 0.f, orbitRotate = 0.f;

GLuint textureSun, textureMercury, textureVenus, textureEarth, textureMars, textureJupiter, textureSaturn, textureUranus, textureNeptune, textureRingSaturn, textureSaturnRing;

float orbitSpeeds[] = {
	1.90f,
	1.20f,
	1.00f,
	0.53f, 
	0.40f, 
	0.35f, 
	0.30f,
	0.25f 
};

float orbitRotations[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

// spaceship
Model spaceship;
glm::vec3 spaceshipPosition(0.0f, 0.0f, 0.0f);
float spaceshipFuel = 100;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Shader ourShader;

// fuel tank
Model fuelTank;
vector<glm::vec3> fuelTankPositions;

// asteroid
Model debris;
vector<glm::vec3> asteroidPositions;

// skybox
Shader skyboxShader;
GLuint skyboxVAO, skyboxVBO, skyboxTexture;

// total time
clock_t startTime;
clock_t endTime;
int elapsedTime;

float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

void calculateTime() {
	endTime = clock();
	elapsedTime = (endTime - startTime) / CLOCKS_PER_SEC;
	cout << "GAME OVER!" << endl;
	cout << "Score is: " << elapsedTime << " seconds." << endl;
	glfwTerminate();
	exit(0);
}

void initFuelTanks() {
	srand(time(0));
	for (int i = 0; i < 5; i++) {
		float x = -3 + rand() % 6;
		float y = -2 + rand() % 4;
		float z = -3 + rand() % 6;
		fuelTankPositions.push_back(glm::vec3(x, y, z));
	}
}

void renderFuelTanks() {
	if (fuelTankPositions.empty())
	{
		initFuelTanks();
	}

	for (glm::vec3 position : fuelTankPositions) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.005f));
		ourShader.setMat4("model", model);

		fuelTank.Draw(ourShader);
	}
}

void checkTankCollisions() {
	for (int i = 0; i < fuelTankPositions.size(); i++) {
		float distance = glm::distance(spaceshipPosition, fuelTankPositions[i]);
		if (distance < 0.5f) {
			cout << "Tank collected at position: " << fuelTankPositions[i].x << ", "
				<< fuelTankPositions[i].y << ", " << fuelTankPositions[i].z << endl;

			// Remove the tank
			fuelTankPositions.erase(fuelTankPositions.begin() + i);
			spaceshipFuel += 50;
			cout << "Fuel increased by 50 => " << spaceshipFuel << endl;
			break;
		}
	}
}

void initAsteroids() {
	srand(time(0));
	for (int i = 0; i < 3; i++) {
		float x = -3 + rand() % 6;
		float y = -1 + rand() % 2;
		float z = -3 + rand() % 6;
		asteroidPositions.push_back(glm::vec3(x, y, z));
	}
}

void renderAsteroids() {
	for (int i = 0; i < asteroidPositions.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, asteroidPositions[i]);
		model = glm::scale(model, glm::vec3(0.15f));
		ourShader.setMat4("model", model);
		debris.Draw(ourShader);
	}
}

void checkAsteroidCollisions() {
	for (int i = 0; i < asteroidPositions.size(); i++) {
		float distance = glm::distance(spaceshipPosition, asteroidPositions[i]);
		if (distance < 0.5f) {
			cout << "Spaceship crashed at position: " << asteroidPositions[i].x << ", "
				<< asteroidPositions[i].y << ", " << asteroidPositions[i].z << endl;

			calculateTime();
		}
	}
}

GLuint loadTexture(const char* filepath)
{
	GLuint textureID;
	int width, height, channels;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture: " << filepath << endl;
	}

	stbi_image_free(data);
	return textureID;
}

void initTexture()
{
	textureSun = loadTexture("planets_textures/2k_sun.jpg");
	textureMercury = loadTexture("planets_textures/2k_mercury.jpg");
	textureVenus = loadTexture("planets_textures/2k_venus_surface.jpg");
	textureEarth = loadTexture("planets_textures/2k_earth_daymap.jpg");
	textureMars = loadTexture("planets_textures/2k_mars.jpg");
	textureJupiter = loadTexture("planets_textures/2k_jupiter.jpg");
	textureSaturn = loadTexture("planets_textures/2k_saturn.jpg");
	textureUranus = loadTexture("planets_textures/2k_uranus.jpg");
	textureNeptune = loadTexture("planets_textures/2k_neptune.jpg");
	textureSaturnRing = loadTexture("planets_textures/2k_saturn_ring_alpha.png");
}

void loadSkyboxTextures() {
	vector<string> faces{
		"blue_skybox/right.png",
		"blue_skybox/left.png",
		"blue_skybox/top.png",
		"blue_skybox/bot.png",
		"blue_skybox/front.png",
		"blue_skybox/back.png"
	};

	glGenTextures(1, &skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	int width, height, nrChannels;
	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			cout << "Cubemap tex failed to load at path: " << faces[i] << endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void initSkybox() {
	// Generate VAO and VBO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);

	// Bind and fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	// Set vertex attribute pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	loadSkyboxTextures();
}

void renderSkyBox() {
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 skyboxView = camera.GetViewMatrix();
	glm::mat4 skyboxProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 skyboxModel = glm::mat4(1.0f);
	skyboxModel = glm::translate(skyboxModel, camera.Position);
	skyboxModel = glm::scale(skyboxModel, glm::vec3(10.0f, 10.0f, 10.0f));

	skyboxShader.use();
	skyboxShader.setMat4("view", skyboxView);
	skyboxShader.setMat4("projection", skyboxProjection);
	skyboxShader.setMat4("model", skyboxModel);

	// Bind the texture
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	skyboxShader.setInt("skybox", 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void init()
{
	ourShader.use();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, planet.getInterleavedVertexSize(), planet.getInterleavedVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planet.getIndexSize(), planet.getIndices(), GL_STATIC_DRAW);

	// Set up vertex attribute pointers using ourShader
	GLint vPosition = glGetAttribLocation(ourShader.ID, "vertexPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, planet.getInterleavedStride(), (void*)0);

	GLint vNormal = glGetAttribLocation(ourShader.ID, "vertexNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, planet.getInterleavedStride(), (void*)(3 * sizeof(float)));

	GLint vTexCoord = glGetAttribLocation(ourShader.ID, "vertexTexture");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, planet.getInterleavedStride(), (void*)(6 * sizeof(float)));

	// Set up material and light properties
	ourShader.setVec4("material.ambient", 1.0f, 1.0f, 1.0f, 1.0f);
	ourShader.setFloat("material.shininess", 32.0f);

	ourShader.setVec4("pointLights[0].ambient", 0.6f, 0.6f, 0.6f, 1.0f);
	ourShader.setVec4("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	ourShader.setVec4("pointLights[0].specular", 1.5f, 1.5f, 1.5f, 1.0f);
	ourShader.setVec4("pointLights[0].position", 0.0f, 0.0f, 0.0f, 1.0f);

	ourShader.setFloat("pointLights[0].constant", 1.0f);
	ourShader.setFloat("pointLights[0].linear", 0.03f);
	ourShader.setFloat("pointLights[0].quadratic", 0.03f);

	initTexture();
	initSkybox();
	initFuelTanks();
	initAsteroids();
}

void renderSpaceship() {
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// Update spaceship position relative to the camera
	spaceshipPosition = camera.Position + camera.Front * 2.0f;

	// Camera looks at the spaceship
	glm::mat4 view = glm::lookAt(camera.Position, spaceshipPosition, camera.Up);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, spaceshipPosition);

	// Build rotation from camera's orientation
	glm::mat4 rotation(glm::vec4(camera.Right, 0.0f), glm::vec4(camera.Up, 0.0f), glm::vec4(-camera.Front, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	// Apply rotation to the spaceship
	model = model * rotation;
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);
	ourShader.setMat4("model", model);
	ourShader.setVec3("camera", camera.Position);

	// Render the spaceship model
	spaceship.Draw(ourShader);
}

void renderSolarSystem() {
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureSun);
	ourShader.setInt("tex", 0);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	ourShader.setMat4("model", model);
	glm::vec3 sunColor(1.0f, 1.0f, 0.0f);
	ourShader.setVec4("material.diffuse", sunColor.r, sunColor.g, sunColor.b, 1.0f);

	glDrawElements(GL_TRIANGLES, planet.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

	// Mercury
	glBindTexture(GL_TEXTURE_2D, textureMercury);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(0.5f), 0.2f, 0.45f, 0);

	// Venus
	glBindTexture(GL_TEXTURE_2D, textureVenus);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(1.0f, 0.9f, 0.65f), 0.3f, 0.6f, 1);

	// Earth
	glBindTexture(GL_TEXTURE_2D, textureEarth);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(0.0f, 0.5f, 1.0f), 0.4f, 0.7f, 2);

	// Mars
	glBindTexture(GL_TEXTURE_2D, textureMars);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(0.8f, 0.3f, 0.2f), 0.5f, 0.45f, 3);

	// Jupiter
	glBindTexture(GL_TEXTURE_2D, textureJupiter);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(0.9f, 0.6f, 0.3f), 0.6f, 0.95f, 4);

	// Saturn (with ring)
	renderSaturnWithRing(0.73f, 1.0f, textureSaturn, textureSaturnRing);

	// Uranus
	glBindTexture(GL_TEXTURE_2D, textureUranus);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(0.5f, 0.8f, 0.9f), 0.85f, 0.7f, 6);

	// Neptune
	glBindTexture(GL_TEXTURE_2D, textureNeptune);
	ourShader.setInt("tex", 0);
	renderPlanet(glm::vec3(0.3f, 0.4f, 0.8f), 0.95f, 0.7f, 7);
}

void display(void)
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderSkyBox();

	ourShader.use();

	renderSpaceship();

	renderFuelTanks();

	renderAsteroids();

	checkTankCollisions();

	checkAsteroidCollisions();

	renderSolarSystem();

}

void idle()
{
	selfRotate += 1.5f * deltaTime;

	for (int i = 0; i < 8; i++) {
		orbitRotations[i] += orbitSpeeds[i] * deltaTime;
	}
}

void renderPlanet(const glm::vec3& color, float orbitDistance, float scale, int planetIndex)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, orbitRotations[planetIndex], glm::vec3(0.0f, 1.0f, 0.0f)); // Orbit around the Sun
	model = glm::translate(model, glm::vec3(orbitDistance, 0.0f, 0.0f));  // Position in orbit
	model = glm::rotate(model, selfRotate, glm::vec3(0.0f, 1.0f, 0.0f));  // Self rotation
	model = glm::scale(model, glm::vec3(scale, scale, scale));

	// Set model matrix and diffuse color using ourShader
	ourShader.setMat4("model", model);
	ourShader.setVec4("material.diffuse", color.r, color.g, color.b, 1.0f);

	glDrawElements(GL_TRIANGLES, planet.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}

void renderSaturnWithRing(float orbitDistance, float scale, GLuint saturnTextureID, GLuint ringTextureID)
{
	// Bind Saturn texture
	glBindTexture(GL_TEXTURE_2D, saturnTextureID);
	ourShader.setInt("tex", 0);

	// Render Saturn
	glm::vec3 saturnColor = glm::vec3(0.9f, 0.8f, 0.5f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, orbitRotations[5], glm::vec3(0.0f, 1.0f, 0.0f)); // Orbit around the Sun
	model = glm::translate(model, glm::vec3(orbitDistance, 0.0f, 0.0f));  // Position in orbit
	model = glm::rotate(model, selfRotate, glm::vec3(0.0f, 1.0f, 0.0f));  // Self rotation
	model = glm::scale(model, glm::vec3(scale, scale, scale));             // Scale Saturn

	ourShader.setMat4("model", model);
	ourShader.setVec4("material.diffuse", saturnColor.r, saturnColor.g, saturnColor.b, 1.0f);
	glDrawElements(GL_TRIANGLES, planet.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

	// Bind ring texture
	glBindTexture(GL_TEXTURE_2D, ringTextureID);
	ourShader.setInt("tex", 0);

	glm::vec3 ringColor = glm::vec3(0.7f, 0.7f, 0.7f);
	glm::mat4 ringModel = model;
	ringModel = glm::scale(ringModel, glm::vec3(1.5f, 0.05f, 1.5f));

	ourShader.setMat4("model", ringModel);
	ourShader.setVec4("material.diffuse", ringColor.r, ringColor.g, ringColor.b, 1.0f);
	glDrawElements(GL_TRIANGLES, planet.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		cerr << "Error: " << glewGetErrorString(err) << endl;

	}
	cerr << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	// Initialize shader and model (ONE TIME ONLY)
	ourShader = Shader("vshader.glsl", "fshader.glsl");
	spaceship = Model("spaceship/prometheus.obj");
	skyboxShader = Shader("skybox_vshader.glsl", "skybox_fshader.glsl");
	fuelTank = Model("fuel_tank/fuel_can.fbx");
	debris = Model("asteroid/asteroid.fbx");

	init();

	startTime = clock();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Process user input
		processInput(window);

		if (spaceshipFuel <= 0)
		{
			calculateTime();
		}

		// Rendering logic
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourShader.use();

		idle();

		display();

		// Swap front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	endTime = clock();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	bool isKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
		isKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		isKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		isKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		isKeyPressed = true;
	}

	if (isKeyPressed)
	{
		spaceshipFuel -= 0.25;
		cout << "fuel now: " << spaceshipFuel << endl;
	}

	float velocity = camera.MovementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.Position += camera.Up * velocity;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		camera.Position -= camera.Up * velocity;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
