#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include<stack>
#include<ctime>

typedef glm::vec4 point4;
typedef glm::vec4 color4;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void displayFloor();
void displayRobot();
unsigned int loadTexture(char const * path);
void torso();
void head();
void upperLeftArm();
void lowerLeftArm();
void upperRightArm();
void lowerRightArm();
void upperLeftLeg();
void lowerLeftLeg();
void upperRightLeg();
void lowerRightLeg();
void displayLight();
void printHelp();
void timerCallBack(void f());
void moveBack();
void moveForward();
void bound();
void displayBox();


// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cameraOnRobot = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float lastCallTime = 0.0f;

bool autoRun = true;

// RGBA olors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

//robot attributes

float speed = 2;
float boundSpeed = 0;
glm::vec3 robotPos = glm::vec3(0.0f, 0.0f, 0.0f);


// walk theta
float theta = 50;
// texture
unsigned texture1, texture2, texture3;

// VAO, VBO
unsigned int floorVBO, floorVAO;
unsigned int robotVBO, robotVAO;
unsigned int lightVBO, lightVAO;

// world space positions of our cubes
std::vector<glm::vec3> cubePositions;

// model stack
std::stack<glm::mat4> mvstack;
glm::mat4 globleModelMatrix;

//shadow
glm::mat4 shadowProjMatrix(1.0);

//box
const int BOX_NUMBER = 20;
glm::vec3 boxesPos[BOX_NUMBER];
bool hasBox[22][22];

//displayment
float s;
bool canBound;

float floorVertices[] = {
	//	  x		 y		z		u	  v
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
};

// shader

Shader robot_shader;
Shader floor_shader;
Shader lightShader;
Shader shadowShader;
// light
glm::vec3 lightPos = glm::vec3(0, 5, 0);

// box
int box_x, box_z;

// score
unsigned int score = 0;
float squareVertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
// robot color
color4 colors[36];


/*
	flag:
	0: right leg,left hand
	1: left leg, right hand
*/

int flag = 0;

// robot direction
glm::vec3 forwardDir = glm::vec3(0, 0, -0.5);	//forword;

void displayShadow(glm::mat4 model = globleModelMatrix);
bool inBox(glm::vec3 p=robotPos);
void init() {

	camera.Position = glm::vec3(1, 1, 1);
	camera.Front = glm::vec3(0, 0, -1);
	shadowProjMatrix = glm::mat4(1.0);
	shadowProjMatrix[1][3] = -1.0 / lightPos[1];
	shadowProjMatrix[3][3] = 0;

	globleModelMatrix = glm::translate(glm::mat4(1), robotPos);
	//globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(180.0), glm::vec3(0, 1, 0));
	robot_shader = Shader("robotShader.vs", "robotShader.fs");
	floor_shader = Shader("floorShader.vs", "floorShader.fs");
	lightShader = Shader("lightShade.vs", "lightShade.fs");
	shadowShader = Shader("shadowShader.vs", "shadowShader.fs");
	srand((unsigned)time(0));

	//generate box pos
	for (int i = 0; i < BOX_NUMBER; i++) {
		int flag = rand() % 2 == 1 ? -1 : 1;
		int x = rand() % 9 + 1;
		x *= flag;
		int z = rand() % 9 + 1;
		flag = rand() % 2 == 1 ? -1 : 1;
		z *= flag;

		boxesPos[i] = glm::vec3(x, 0, z);
		hasBox[x][z] = true;
	}
	for (int i = 0; i < 36; i++) {
		colors[i] = vertex_colors[1];
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);

	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	texture1 = loadTexture("resources/textures/brickwall.jpg");
	texture2 = loadTexture("resources/textures/awesomeface.png");
	texture3 = loadTexture("resources/textures/marble.jpg");

	//-------------

	glGenVertexArrays(1, &robotVAO);
	glBindVertexArray(robotVAO);

	glGenBuffers(1, &robotVBO);
	glBindBuffer(GL_ARRAY_BUFFER, robotVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(squareVertices), squareVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices), sizeof(colors),
		colors);
	// position attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	// nomorls attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0,
		(void*)sizeof(squareVertices));

	// --LIGHT
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(squareVertices), squareVertices);
	// position attribute
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(5);

	printHelp();
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2017152021_FinalProject", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
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
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	for (int i = -10; i < 10; i++) {
		for (int j = -10; j < 10; j++) {
			cubePositions.push_back(glm::vec3(i, 0, j));
		}
	}

	init();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		//camera.Position = glm::vec3(robotPos.x/2.0,robotPos.y/2.0,robotPos.z/2.0) + glm::vec3(0,1.6,0);
		// per-frame time logic
		// --------------------

		bound();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (autoRun)
			timerCallBack(moveForward);
		displayFloor();
		displayRobot();
		displayLight();
		displayBox();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &floorVAO);
	glDeleteBuffers(1, &floorVBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	//如果输入的是空格
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		boundSpeed = 3.0;
		s = 0;
		canBound = true;
		float bound_x = robotPos.x;
		float bound_z = robotPos.z;
		if (inBox(glm::vec3(bound_x, 0, bound_z))) {
			canBound = false;
		}
	}
	//如果输入的是退出键
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		std::cout << "final score: " << (1.0 / glfwGetTime()*score*score) * 1000 << "\n";
	}
	if (!cameraOnRobot) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		autoRun = false;
		timerCallBack(moveForward);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		timerCallBack(moveBack);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(0.5), glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(-0.5), glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		speed += 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		if (speed > 0.1)
			speed -= 0.1;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		globleModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(180.0), glm::vec3(0, 1, 0));
		robotPos = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		cameraOnRobot = true;
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		cameraOnRobot = false;
	}
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!cameraOnRobot) {
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
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!cameraOnRobot) {
		camera.ProcessMouseScroll(yoffset);
	}
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// 纹理的环绕方式
		// for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);

		//线性过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void displayFloor() {

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------

	// activate shader
	floor_shader.use();

	floor_shader.setInt("texture1", 0);
	floor_shader.setInt("texture2", 1);

	floor_shader.setVec3("viewPos", camera.Position);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);


	// 传入透视投影矩阵
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	floor_shader.setMat4("projection", projection);

	// 传入视图矩阵
	glm::mat4 view = camera.GetViewMatrix();
	floor_shader.setMat4("view", view);

	// render boxes
	glBindVertexArray(floorVAO);
	for (unsigned int i = 0; i < cubePositions.size(); i++)
	{
		// 计算每个地板块的位置（模型矩阵）
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		floor_shader.setMat4("model", model*glm::translate(glm::mat4(1.0), glm::vec3(0, 0.49, 0)));

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}


void displayBox() {
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
// -------------------------------------------------------------------------------------------
	floor_shader.use();
	floor_shader.setInt("texture1", 0);
	floor_shader.setInt("texture2", 1);

	floor_shader.setVec3("viewPos", camera.Position);
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// activate shader
	floor_shader.use();

	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	floor_shader.setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	floor_shader.setMat4("view", view);

	// render boxes
	glBindVertexArray(robotVAO);
	for (unsigned int i = 0; i < BOX_NUMBER; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		int x = boxesPos[i].x;
		int z = boxesPos[i].z;
		if (!hasBox[x][z]) {
			continue;
		}
		glm::mat4 model;
		model = glm::translate(model, boxesPos[i] + glm::vec3(0, 0.49, 0));
		floor_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	for (unsigned int i = 0; i < BOX_NUMBER; i++) {
		int x = boxesPos[i].x;
		int z = boxesPos[i].z;
		if (!hasBox[x][z]) {
			continue;
		}
		glm::mat4 model;
		model = glm::translate(model, boxesPos[i] + glm::vec3(0, 0.49, 0));
		displayShadow(model);
	}

}
void displayRobot() {
	robot_shader.use();
	robot_shader.setInt("flag", 0);
	glBindVertexArray(robotVAO);
	torso();
}

void torso() {
	//保存父节点的变换矩阵
	mvstack.push(globleModelMatrix);
	
	// torso的位置和长宽高设置
	globleModelMatrix = globleModelMatrix * glm::translate(glm::mat4(1.0), glm::vec3(0, 0.75 + 0.5, 0));
	globleModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.6, 0.8, 0.4)) * globleModelMatrix;

	robotPos = glm::vec3(globleModelMatrix * glm::vec4(0, 0, 0, 1));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	robot_shader.use();

	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	head();
	upperLeftArm();
	upperRightArm();
	upperLeftLeg();
	upperRightLeg();
	displayShadow();
	globleModelMatrix = mvstack.top();
	mvstack.pop();
}

void head() {

	mvstack.push(globleModelMatrix);
	globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(0, 0.7, 0));
	globleModelMatrix = glm::scale(globleModelMatrix, glm::vec3(0.4, 0.3, 0.3));
	if (cameraOnRobot) {
		camera.Position = glm::vec3(globleModelMatrix * glm::vec4(0, 0, 0, 1)) + glm::vec3(0, 0.5, 0);
		camera.Front = normalize(glm::vec3(globleModelMatrix*glm::vec4(0, 0, -1, 0)));
		camera.Up = normalize(glm::vec3(0, 1, 0));
	}
	//std::cout << front.x << " " << front.y << " " << front.z << "\n";
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	robot_shader.use();
	robot_shader.setInt("texture1", 0);
	robot_shader.setInt("texture2", 1);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture3);
	//robot_shader.setInt("flag", 1);
	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	//除了正面
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 6, 36);

	//正面贴图
	floor_shader.use();
	floor_shader.setInt("flag", 0);
	floor_shader.setMat4("projection", projection);
	floor_shader.setMat4("view", view);
	floor_shader.setMat4("model", globleModelMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	floor_shader.setInt("flag", 0);
	displayShadow();
	globleModelMatrix = mvstack.top();
	mvstack.pop();

}

void upperLeftArm() {
	mvstack.push(globleModelMatrix);
	globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(-0.7, 0.4, 0));
	globleModelMatrix = glm::scale(globleModelMatrix, glm::vec3(0.3, 0.3, 0.3));
	if (!flag)
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(45.0), glm::vec3(1, 0, 0));
	else
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(-45.0), glm::vec3(1, 0, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	robot_shader.use();
	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	robot_shader.setVec3("viewPos", camera.Position);
	//std::cout<<camera.Position<<"\n"
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	lowerLeftArm();
	displayShadow();
	globleModelMatrix = mvstack.top();
	mvstack.pop();
}

void upperRightArm() {
	mvstack.push(globleModelMatrix);
	globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(0.7, 0.4, 0));
	globleModelMatrix = glm::scale(globleModelMatrix, glm::vec3(0.3, 0.3, 0.3));
	if (flag)
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(45.0), glm::vec3(1, 0, 0));
	else
		globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(-45.0), glm::vec3(1, 0, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	robot_shader.use();
	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	robot_shader.setVec3("viewPos", camera.Position);
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	lowerRightArm();
	globleModelMatrix = mvstack.top();
	mvstack.pop();
}

void lowerRightArm() {
	lowerLeftArm();
}

void lowerLeftArm() {
	mvstack.push(globleModelMatrix);
	globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(0, -1.25, 0));
	globleModelMatrix = glm::scale(globleModelMatrix, glm::vec3(0.6, 1.5, 0.6));

	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	robot_shader.setInt("flag", 0);
	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	robot_shader.setVec3("viewPos", camera.Position);
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	displayShadow();
	globleModelMatrix = mvstack.top();
	mvstack.pop();
}

void upperLeftLeg() {
	mvstack.push(globleModelMatrix);
	globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(-0.3, -0.48, 0));
	globleModelMatrix = glm::scale(globleModelMatrix, glm::vec3(0.4, 0.5, 0.4));
	//if(flag)
	globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(theta), glm::vec3(1, 0, 0));

	/*displayShadow();*/
//else
	//globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(-45.0), glm::vec3(1, 0, 0));
// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	robot_shader.use();
	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	robot_shader.setVec3("viewPos", camera.Position);
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	lowerLeftLeg();
	globleModelMatrix = mvstack.top();
	mvstack.pop();
}

void upperRightLeg() {
	mvstack.push(globleModelMatrix);
	globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(0.3, -0.48, 0));
	globleModelMatrix = glm::scale(globleModelMatrix, glm::vec3(0.4, 0.5, 0.4));
	//if (!flag)
	globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(-theta), glm::vec3(1, 0, 0));
	//else
		//globleModelMatrix = glm::rotate(globleModelMatrix, (float)glm::radians(45.0), glm::vec3(1, 0, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	robot_shader.use();
	robot_shader.setMat4("projection", projection);
	robot_shader.setMat4("view", view);
	robot_shader.setMat4("model", globleModelMatrix);
	robot_shader.setVec3("viewPos", camera.Position);
	glBindVertexArray(robotVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	lowerRightLeg();
	globleModelMatrix = mvstack.top();
	mvstack.pop();
}

void lowerRightLeg() {
	lowerLeftArm();
}

void lowerLeftLeg() {
	lowerLeftArm();
}

void displayLight() {
	// activate shader
	lightShader.use();

	glm::mat4 model;
	model = glm::translate(model, lightPos);
	lightShader.setMat4("model", model);
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	lightShader.setMat4("projection", projection);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	lightShader.setMat4("view", view);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void displayShadow(glm::mat4 model) {
	//shadow

	// activate shader
	shadowShader.use();
	// pass projection matrix to shader 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	shadowProjMatrix = glm::mat4(1.0);
	shadowProjMatrix[1][3] = -1.0 / lightPos[1];
	shadowProjMatrix[3][3] = 0;

	glBindVertexArray(lightVAO);

	// 绘制阴影
	glm::mat4 matrix = projection * view * glm::translate(glm::mat4(1.0), lightPos)*
		shadowProjMatrix *glm::translate(glm::mat4(1.0), -lightPos)*model;

	shadowShader.setMat4("matrix", matrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void timerCallBack(void f()) {
	float currentCallTime = glfwGetTime();
	if (currentCallTime - lastCallTime >= 1.0 / speed) {
		f();
		lastCallTime = currentCallTime;
	}
}


void moveForward() {
	flag = !flag;
	theta = -theta;
	globleModelMatrix = glm::translate(globleModelMatrix, forwardDir);
	// 如果不跳，不在箱子上面且当前高度大于1，则会落到地面
	if (!canBound && !inBox() && robotPos.y > 1.0001) {
		canBound = true;
	}
}

void moveBack() {
	flag = !flag;
	theta = -theta;
	globleModelMatrix = glm::translate(globleModelMatrix, -forwardDir);
	if (!canBound && !inBox() && robotPos.y > 1.0001) {
		canBound = true;
	}
}

void bound() {

	if (canBound) {
		globleModelMatrix = glm::translate(globleModelMatrix, glm::vec3(0, 0.01*boundSpeed, 0));
		boundSpeed -= 0.01;
		//如果下落的过程踩到箱子
		if (inBox()) {
			if (robotPos.y < 2) {
				canBound = true;
				score += 1;
				hasBox[box_x][box_z] = false;
			}
		}
		else {
			if (robotPos.y < 1) {
				canBound = false;
			}
		}
	}
}

bool inBox(glm::vec3 p) {
	float rx = p.x;
	float rz = p.z;

	for (int i = 0; i < BOX_NUMBER; i++) {
		if (boxesPos[i].x - 0.5 < rx &&
			boxesPos[i].x + 0.5 > rx &&
			boxesPos[i].z - 0.5 < rz &&
			boxesPos[i].z + 0.5 > rz
			) {
			box_x = boxesPos[i].x;
			box_z = boxesPos[i].z;
			if (!hasBox[box_x][box_z])
				continue;
			return true;
		}
	}
	return false;
}

void printHelp() {
	std::cout << "========== 机器人版跳一跳 ==========\n\n";
	std::cout << "Q ---------- 加快机器人的移动速度\n";
	std::cout << "E ---------- 降低机器人的移动速度\n";
	std::cout << "W ---------- 让相机向前移动\n";
	std::cout << "S ---------- 相机向后移动\n";
	std::cout << "A ---------- 相机左转\n";
	std::cout << "D ----------- 相机右转\n";
	std::cout << "C ---------- 开启机器人视角模式，初始为自由相机模式（即可以移动相机）\n";
	std::cout << "B ---------- 开启自由相机模式\n";
	std::cout << "R ---------- 机器人回到游戏原点，但是分数不变\n";
	std::cout << "ESC ---------- 退出游戏，在控制台打印最终分数，跟踩的箱子个数和时间有关\n";
	std::cout << "方向键 -----------控制机器人的上下左右\n";
	std::cout << "SPACE(空格键) ---------- 让机器人跳一跳\n";
	std::cout << "鼠标 ---------- 控制相机的方向\n";
}