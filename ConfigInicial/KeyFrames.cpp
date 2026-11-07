// Practica 12
// Valdez Hernandez Alan Josue
// Fecha de entrega: 8 de noviembre del 2026
// Numero de cuenta: 421122006
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;

// Variables para las patas
float FRightLeg = 0.0f;
float FLeftLeg = 0.0f;
float BRightLeg = 0.0f;
float BLeftLeg = 0.0f;

// Variables para acostar al perrito
float bodyTilt = 0.0f;
float bodyOffsetY = 0.0f;
float bodyRoll = 0.0f;


//KeyFrames
float dogPosX = 0.0f;
float dogPosY = 0.0f;
float dogPosZ = 0.0f;

#define MAX_FRAMES 16
int i_max_steps = 1200;
int i_curr_steps = 0;
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	// Para inclinar al perrito
	float bodyTilt;
	float bodyTiltInc;

	float bodyOffsetY;
	float bodyOffsetYInc;

	float bodyRoll;
	float bodyRollInc;
	//------------------
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;

	// Para las patas y cola:
	float FRightLeg;
	float FRightLegInc;

	float FLeftLeg;
	float FLeftLegInc;

	float BRightLeg;
	float BRightLegInc;

	float BLeftLeg;
	float BLeftLegInc;

	float tail;
	float tailInc;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;
bool lyingFrameAdded = false;

void saveFrame(void)
{
	if (FrameIndex >= MAX_FRAMES) {
		std::cout << "Se alcanzo el maximo de Keyframes.\n";
		return;
	}

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;

	//Para rotar al perrito:
	KeyFrame[FrameIndex].bodyTilt = bodyTilt;
	KeyFrame[FrameIndex].bodyOffsetY = bodyOffsetY;
	KeyFrame[FrameIndex].bodyRoll = bodyRoll;
	//---------------------

	KeyFrame[FrameIndex].head = head;

	//Patas y cola:
	KeyFrame[FrameIndex].FRightLeg = FRightLeg;
	KeyFrame[FrameIndex].FLeftLeg = FLeftLeg;

	KeyFrame[FrameIndex].BRightLeg = BRightLeg;
	KeyFrame[FrameIndex].BLeftLeg = BLeftLeg;

	KeyFrame[FrameIndex].tail = tail;

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;

	rotDog = KeyFrame[0].rotDog;

	//Para rotal al perrito:
	bodyTilt = KeyFrame[0].bodyTilt;
	bodyOffsetY = KeyFrame[0].bodyOffsetY;
	bodyRoll = KeyFrame[0].bodyRoll;
	//-------------------------

	// Patas y cola
	FRightLeg = KeyFrame[0].FRightLeg;
	FLeftLeg = KeyFrame[0].FLeftLeg;

	BRightLeg = KeyFrame[0].BRightLeg;
	BLeftLeg = KeyFrame[0].BLeftLeg;

	tail = KeyFrame[0].tail;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

	//Rotar al perrito
	KeyFrame[playIndex].bodyTiltInc =
		(KeyFrame[playIndex + 1].bodyTilt -
			KeyFrame[playIndex].bodyTilt) / i_max_steps;

	KeyFrame[playIndex].bodyOffsetYInc =
		(KeyFrame[playIndex + 1].bodyOffsetY -
			KeyFrame[playIndex].bodyOffsetY) / i_max_steps;

	KeyFrame[playIndex].bodyRollInc =
		(KeyFrame[playIndex + 1].bodyRoll -
			KeyFrame[playIndex].bodyRoll) / i_max_steps;

	//---------------------

	// Patas y cola:
	KeyFrame[playIndex].FRightLegInc =
		(KeyFrame[playIndex + 1].FRightLeg -
			KeyFrame[playIndex].FRightLeg) / i_max_steps;

	KeyFrame[playIndex].FLeftLegInc =
		(KeyFrame[playIndex + 1].FLeftLeg -
			KeyFrame[playIndex].FLeftLeg) / i_max_steps;

	KeyFrame[playIndex].BRightLegInc =
		(KeyFrame[playIndex + 1].BRightLeg -
			KeyFrame[playIndex].BRightLeg) / i_max_steps;

	KeyFrame[playIndex].BLeftLegInc =
		(KeyFrame[playIndex + 1].BLeftLeg -
			KeyFrame[playIndex].BLeftLeg) / i_max_steps;

	KeyFrame[playIndex].tailInc =
		(KeyFrame[playIndex + 1].tail -
			KeyFrame[playIndex].tail) / i_max_steps;

}

// Crea la secuencia para sentarse, saludar y acostarse
void CreateSitWaveAnimation()
{
	FrameIndex = 0;

	// Pose 1: Perrito de pie
	dogPosX = 0.0f;
	dogPosY = 0.0f;
	dogPosZ = 0.0f;
	rotDog = 0.0f;

	bodyTilt = 0.0f;
	bodyOffsetY = 0.0f;
	bodyRoll = 0.0f;

	head = 0.0f;
	tail = 0.0f;

	FLeftLeg = 0.0f;
	FRightLeg = 0.0f;
	BLeftLeg = 0.0f;
	BRightLeg = 0.0f;

	saveFrame();

	// Pose 2: Comienza a sentarse
	bodyOffsetY = -0.025f;
	BLeftLeg = 28.0f;
	BRightLeg = 28.0f;
	FLeftLeg = -8.0f;
	FRightLeg = -8.0f;
	tail = 8.0f;
	saveFrame();

	// Pose 3: Perrito sentado
	bodyOffsetY = -0.055f;
	bodyTilt = -5.0f;
	BLeftLeg = 62.0f;
	BRightLeg = 62.0f;
	FLeftLeg = 0.0f;
	FRightLeg = 0.0f;
	head = -5.0f;
	tail = -8.0f;
	saveFrame();

	// Pose 4: Levanta la pata delantera derecha
	FRightLeg = -58.0f;
	head = 4.0f;
	tail = 12.0f;
	saveFrame();

	// Pose 5: Primer movimiento del saludo
	FRightLeg = -78.0f;
	tail = -12.0f;
	saveFrame();

	// Pose 6: Segundo movimiento del saludo
	FRightLeg = -48.0f;
	tail = 12.0f;
	saveFrame();

	// Pose 7: Regresa a la posicion de saludo
	FRightLeg = -72.0f;
	tail = -8.0f;
	saveFrame();

	// Pose 8: Mantiene la pata levantada
	FRightLeg = -58.0f;
	tail = 0.0f;
	saveFrame();

	// Pose 9: Comienza a recostarse
	bodyOffsetY = -0.085f;
	bodyTilt = -8.0f;
	bodyRoll = 20.0f;

	FLeftLeg = 15.0f;
	FRightLeg = -15.0f;
	BLeftLeg = 35.0f;
	BRightLeg = -35.0f;

	head = -12.0f;
	tail = 15.0f;
	saveFrame();

	// Pose 10: Se inclina hacia un costado
	bodyOffsetY = -0.12f;
	bodyTilt = 0.0f;
	bodyRoll = 55.0f;

	FLeftLeg = 45.0f;
	FRightLeg = -45.0f;
	BLeftLeg = 50.0f;
	BRightLeg = -50.0f;

	head = -20.0f;
	tail = 20.0f;
	saveFrame();

	// Pose 11: Queda recostado de lado
	bodyOffsetY = -0.13f;
	bodyTilt = 0.0f;
	bodyRoll = 90.0f;

	FLeftLeg = 75.0f;
	FRightLeg = -75.0f;
	BLeftLeg = 65.0f;
	BRightLeg = -65.0f;

	head = -30.0f;
	tail = 25.0f;
	saveFrame();

	// Pose 12: Mantiene la posicion final
	bodyOffsetY = -0.13f;
	bodyTilt = 0.0f;
	bodyRoll = 90.0f;

	FLeftLeg = 85.0f;
	FRightLeg = -85.0f;
	BLeftLeg = 70.0f;
	BRightLeg = -70.0f;

	head = -35.0f;
	tail = 15.0f;
	saveFrame();

	resetElements();

	std::cout << "Secuencia de animacion creada con "
		<< FrameIndex << " Keyframes." << std::endl;
}

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 12 - Alan Valdez", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		// Inicializar movimientos del cuerpo
		KeyFrame[i].bodyTilt = 0;
		KeyFrame[i].bodyTiltInc = 0;

		KeyFrame[i].bodyOffsetY = 0;
		KeyFrame[i].bodyOffsetYInc = 0;

		KeyFrame[i].bodyRoll = 0;
		KeyFrame[i].bodyRollInc = 0;
		//-----------------------------


		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;

		//Patas y cola:
		KeyFrame[i].FRightLeg = 0;
		KeyFrame[i].FRightLegInc = 0;

		KeyFrame[i].FLeftLeg = 0;
		KeyFrame[i].FLeftLegInc = 0;

		KeyFrame[i].BRightLeg = 0;
		KeyFrame[i].BRightLegInc = 0;

		KeyFrame[i].BLeftLeg = 0;
		KeyFrame[i].BLeftLegInc = 0;

		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
	}

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		//Body
		modelTemp = glm::mat4(1.0f);

		modelTemp = glm::translate(modelTemp,
			glm::vec3(dogPosX, dogPosY + bodyOffsetY, dogPosZ));

		modelTemp = glm::rotate(modelTemp,
			glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));

		// Rotacion lateral para acostar al perrito
		modelTemp = glm::rotate(modelTemp,
			glm::radians(bodyRoll), glm::vec3(0.0f, 0.0f, 1.0f));

		// Inclinacion del cuerpo
		model = modelTemp;

		model = glm::rotate(model,
			glm::radians(bodyTilt), glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLeftLeg),
			glm::vec3(-1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FRightLeg),
			glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(BLeftLeg),
			glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(BRightLeg),
			glm::vec3(-1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls

	if (keys[GLFW_KEY_4])
	{

		head += 0.2f;

	}

	if (keys[GLFW_KEY_5])
	{

		head -= 0.2f;

	}

	if (keys[GLFW_KEY_2])
	{

		rotDog += 0.2f;

	}

	if (keys[GLFW_KEY_3])
	{

		rotDog -= 0.2f;

	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.005;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.005;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.005;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.005;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

	// patas y cola:
	// Pata delantera derecha
	if (keys[GLFW_KEY_6])
	{
		FRightLeg += 0.2f;
	}

	if (keys[GLFW_KEY_7])
	{
		FRightLeg -= 0.2f;
	}

	// Pata delantera izquierda
	if (keys[GLFW_KEY_8])
	{
		FLeftLeg += 0.2f;
	}

	if (keys[GLFW_KEY_9])
	{
		FLeftLeg -= 0.2f;
	}

	// Pata trasera derecha
	if (keys[GLFW_KEY_Z])
	{
		BRightLeg += 0.2f;
	}

	if (keys[GLFW_KEY_X])
	{
		BRightLeg -= 0.2f;
	}

	// Pata trasera izquierda
	if (keys[GLFW_KEY_C])
	{
		BLeftLeg += 0.2f;
	}

	if (keys[GLFW_KEY_V])
	{
		BLeftLeg -= 0.2f;
	}

	// Movimiento de la cola
	if (keys[GLFW_KEY_MINUS])
	{
		tail += 0.2f;
	}

	if (keys[GLFW_KEY_EQUAL])
	{
		tail -= 0.2f;
	}

	// Bajar y subir el cuerpo
	if (keys[GLFW_KEY_O])
	{
		dogPosY -= 0.001f;
	}

	if (keys[GLFW_KEY_P])
	{
		dogPosY += 0.001f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;

		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	// Guardar una postura con K
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		if (!play && !lyingFrameAdded && FrameIndex < MAX_FRAMES - 1)
			saveFrame();
	}

	// Reproducir movimientos y finalmente acostarse con L
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (!play)
		{
			// Si no se guardaron movimientos, guardar la postura actual
			if (FrameIndex == 0)
			{
				saveFrame();
			}
			else
			{
				// Recuperar la ultima postura guardada
				dogPosX = KeyFrame[FrameIndex - 1].dogPosX;
				dogPosY = KeyFrame[FrameIndex - 1].dogPosY;
				dogPosZ = KeyFrame[FrameIndex - 1].dogPosZ;

				rotDog = KeyFrame[FrameIndex - 1].rotDog;
				bodyTilt = KeyFrame[FrameIndex - 1].bodyTilt;
				bodyOffsetY = KeyFrame[FrameIndex - 1].bodyOffsetY;
				bodyRoll = KeyFrame[FrameIndex - 1].bodyRoll;

				head = KeyFrame[FrameIndex - 1].head;

				FRightLeg = KeyFrame[FrameIndex - 1].FRightLeg;
				FLeftLeg = KeyFrame[FrameIndex - 1].FLeftLeg;
				BRightLeg = KeyFrame[FrameIndex - 1].BRightLeg;
				BLeftLeg = KeyFrame[FrameIndex - 1].BLeftLeg;

				tail = KeyFrame[FrameIndex - 1].tail;
			}

			// Agregar el movimiento final para acostarse
			if (!lyingFrameAdded)
			{
				bodyOffsetY = -0.13f;
				bodyTilt = 0.0f;
				bodyRoll = 90.0f;

				FLeftLeg = 85.0f;
				FRightLeg = -85.0f;
				BLeftLeg = 70.0f;
				BRightLeg = -70.0f;

				head = -35.0f;
				tail = 15.0f;

				saveFrame();
				lyingFrameAdded = true;
			}

			// Iniciar reproducción desde el primer KeyFrame
			resetElements();

			playIndex = 0;
			i_curr_steps = 0;

			interpolation();
			play = true;
		}
		else
		{
			play = false;
		}
	}

	// Reiniciar la secuencia con R
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		play = false;
		FrameIndex = 0;
		playIndex = 0;
		i_curr_steps = 0;
		lyingFrameAdded = false;

		dogPosX = 0.0f;
		dogPosY = 0.0f;
		dogPosZ = 0.0f;

		rotDog = 0.0f;
		bodyTilt = 0.0f;
		bodyOffsetY = 0.0f;
		bodyRoll = 0.0f;

		head = 0.0f;
		tail = 0.0f;

		FRightLeg = 0.0f;
		FLeftLeg = 0.0f;
		BRightLeg = 0.0f;
		BLeftLeg = 0.0f;

		std::cout << "Secuencia reiniciada." << std::endl;
	}

	// Activar o desactivar iluminacion con ESPACIO
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;

		if (active)
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		else
			Light1 = glm::vec3(0.0f);
	}
}

void Animation()
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps)
		{
			playIndex++;

			if (playIndex >= FrameIndex - 1)
			{
				printf("termina animacion\n");

				playIndex = 0;
				play = false;
			}
			else
			{
				i_curr_steps = 0;
				interpolation();
			}
		}
		else
		{
			// Movimiento del perrito
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;

			rotDog += KeyFrame[playIndex].rotDogInc;

			// Movimiento del cuerpo
			bodyTilt += KeyFrame[playIndex].bodyTiltInc;
			bodyOffsetY += KeyFrame[playIndex].bodyOffsetYInc;
			bodyRoll += KeyFrame[playIndex].bodyRollInc;

			// Movimiento de cabeza y cola
			head += KeyFrame[playIndex].headInc;
			tail += KeyFrame[playIndex].tailInc;

			// Movimiento de las patas
			FRightLeg += KeyFrame[playIndex].FRightLegInc;
			FLeftLeg += KeyFrame[playIndex].FLeftLegInc;

			BRightLeg += KeyFrame[playIndex].BRightLegInc;
			BLeftLeg += KeyFrame[playIndex].BLeftLegInc;

			i_curr_steps++;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
