// Practica 9
// Valdez Hernandez Alan Josue
// Fecha de entrega: 18 de octubre del 2026
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
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 950, HEIGHT = 700;
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

// ---------------------------------------------------------------------------
// MODELOS LUMINOSOS DEL PANTEON
// Cada modelo tiene su luz puntual pegada a la flama / foco.
// Para mover un modelo, cambia SOLO su posicion aqui (la luz lo sigue sola).
// El piso esta aproximadamente en y = -1.0; si algun modelo flota o se hunde,
// cambia la "y" de su posicion.
// ---------------------------------------------------------------------------
glm::vec3 posFogata(1.5f, -1.0f, 4.6f);
glm::vec3 posVela(2.25f, -1.0f, 0.0f);
glm::vec3 posLuciernaga(0.2f, -0.45f, 0.9f);   // centro del recorrido de la luciernaga
glm::vec3 posFarol(-3.6f, -1.0f, 5.2f);
glm::vec3 posVeladora(-6.9f, -1.0f, 1.7f);

// Escala de cada modelo (1.0 = tamano original del .obj)
const float ESC_FOGATA = 1.0f;
const float ESC_VELA = 1.0f;
const float ESC_LUCIERNAGA = 3.0f;   // la luciernaga mide ~0.1, por eso se agranda
const float ESC_FAROL = 1.0f;
const float ESC_VELADORA = 1.0f;
//------------------------------------------------

// Positions of the point lights (se calculan en cada frame,)
// [0] fogata   [1] vela   [2] luciernaga   [3] farol   [4] veladora
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 - Alan Valdez", nullptr, nullptr);

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
	
	Model Dog((char*)"Models/RedDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model TumbaTransparente((char*)"Models/TumbaTransparente.obj");
	Model bosque((char*)"Models/Bosque.obj");
	Model nube((char*)"Models/Nubes.obj");
	Model sol((char*)"Models/SunCute.obj");
	Model flor((char*)"Models/cartoon_flower.obj");
	Model hongo((char*)"Models/Blue+Mushroom.obj");

	//--------------------
	// Modelos luminosos del panteon (carpeta Models/)
	Model fogata((char*)"Models/fogata.obj");
	Model vela((char*)"Models/vela.obj");
	Model luciernaga((char*)"Models/luciernaga.obj");
	Model farol((char*)"Models/farol.obj");
	Model veladora((char*)"Models/veladora.obj");
	Model luna((char*)"Models/Luna.obj");
	//---------------------------------


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
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

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

                  glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		GLfloat t = glfwGetTime();

		// ---------------------------------------------------------------------
		// Movimiento de la luciernaga: vuela en un recorrido suave y mira hacia
		// donde avanza. Su luz va pegada a ella.
		// ---------------------------------------------------------------------
		glm::vec3 posLuc = posLuciernaga + glm::vec3(0.35f * sin(0.6f * t), 0.12f * sin(1.7f * t), 0.35f * cos(0.45f * t));
		float avanceX = 0.35f * 0.6f * cos(0.6f * t);
		float avanceZ = -0.35f * 0.45f * sin(0.45f * t);
		float rumboLuciernaga = atan2(avanceX, avanceZ);   // el modelo mira hacia +Z

		// Cada luz puntual queda en la flama / foco de su modelo
		pointLightPositions[0] = posFogata + glm::vec3(0.0f, 0.25f * ESC_FOGATA, 0.0f);
		pointLightPositions[1] = posVela + glm::vec3(0.0f, 0.27f * ESC_VELA, 0.0f);
		pointLightPositions[2] = posLuc;
		pointLightPositions[3] = posFarol + glm::vec3(0.0f, 0.875f * ESC_FAROL, 0.0f);
		pointLightPositions[4] = posVeladora + glm::vec3(0.0f, 0.25f * ESC_VELADORA, 0.0f);

		// ---------- DIRECCIONAL: luz de luna (azul frio, muy tenue) ----------
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);//Hacia donde esta apuntando la funet de luz direccional!
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.06f, 0.12f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.10f, 0.13f, 0.28f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.10f, 0.10f, 0.20f);

		// ---------- PUNTUAL 1: FOGATA (naranja intenso, parpadeo irregular) ----------
		float parpadeoFogata = 0.85f + 0.10f * sin(t * 11.0f) * sin(t * 5.3f) + 0.05f * sin(t * 27.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f * parpadeoFogata, 0.10f * parpadeoFogata, 0.02f * parpadeoFogata);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f * parpadeoFogata, 0.65f * parpadeoFogata, 0.20f * parpadeoFogata);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.6f * parpadeoFogata, 0.3f * parpadeoFogata, 0.1f * parpadeoFogata);
		// 3 Valores (constantes) para le calculo basico del factor de atenuacion:
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0007f);

		// ---------- PUNTUAL 2: VELA (amarillo calido, alcance corto, parpadeo suave) ----------
		float parpadeoVela = 0.90f + 0.06f * sin(t * 9.0f) + 0.04f * sin(t * 23.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.12f * parpadeoVela, 0.10f * parpadeoVela, 0.04f * parpadeoVela);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f * parpadeoVela, 0.85f * parpadeoVela, 0.45f * parpadeoVela);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.3f * parpadeoVela, 0.2f * parpadeoVela, 0.1f * parpadeoVela);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.18f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.06f);

		// ---------- PUNTUAL 3: LUCIERNAGA (verde amarillento, destello intermitente, alcance muy corto) ----------
		float destelloLuc = 0.25f + 0.75f * pow(0.5f + 0.5f * sin(t * 3.5f), 2.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.08f * destelloLuc, 0.45f * destelloLuc, 0.05f * destelloLuc);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.25f * destelloLuc, 1.50f * destelloLuc, 0.15f * destelloLuc);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.40f * destelloLuc, 1.50f * destelloLuc, 0.30f * destelloLuc);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.44f);

		// ---------- PUNTUAL 4: FAROL (ambar fijo, con un leve titileo, alcance medio) ----------
		float titileoFarol = 0.97f + 0.03f * sin(t * 7.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.12f * titileoFarol, 0.08f * titileoFarol, 0.03f * titileoFarol);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f * titileoFarol, 0.90f * titileoFarol, 0.55f * titileoFarol);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.4f * titileoFarol, 0.3f * titileoFarol, 0.15f * titileoFarol);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.10f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.04f);

		// ---------- PUNTUAL 5: VELADORA (Flama azul con la misma intensidad de la Vela 1) ----------
		float parpadeoVeladora = 0.90f + 0.07f * sin(t * 8.0f + 1.3f) + 0.03f * sin(t * 21.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		// Espejo exacto de la potencia de pointLights[1], invirtiendo Rojo y Azul:
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.04f * parpadeoVeladora, 0.10f * parpadeoVeladora, 0.12f * parpadeoVeladora);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 0.40f * parpadeoVeladora, 0.85f * parpadeoVeladora, 1.00f * parpadeoVeladora);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.10f * parpadeoVeladora, 0.20f * parpadeoVeladora, 0.30f * parpadeoVeladora);
		// Misma atenuación que la primera vela (0.18 y 0.06) para que abarque el mismo radio:
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.18f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.06f);
		//---------------------------------

		// ---------- SPOTLIGHT: Linterna normal (blanca suave, menor intensidad, mismo radio) ----------
		// Donde se posiciona la fuente de luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Hacia donde se observa la luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		// Luz blanca cálida tenue (valores iguales en R, G, B dan luz blanca sin teñir de color):
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.03f, 0.03f, 0.03f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.35f, 0.35f, 0.32f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.20f, 0.20f, 0.20f);
		// Atenuación suave para que alumbre como linterna real sin quemar el fondo:
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0075f);
		// Mismo radio de abertura que tenías (12 grados interno, 18 grados externo):
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

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
		glm::mat4 modelPiso(1.0f);
		modelPiso = glm::translate(	modelPiso,glm::vec3(0.0f, -1.2f, -1.0f));
		modelPiso = glm::scale(modelPiso,glm::vec3(5.0f, 0.2f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,glm::value_ptr(modelPiso)		);
		Piso.Draw(lightingShader);

		//Modelos del bosque
		glm::mat4 modelBosque(1);
		modelBosque = glm::translate(modelBosque, glm::vec3(-0.5f, -1.33f, 0.0f));
		modelBosque = glm::rotate(modelBosque, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBosque));
		bosque.Draw(lightingShader);

		glm::mat4 modelNube(1);
		modelNube = glm::translate(modelNube, glm::vec3(0.0f, 4.0f, 0.0f));
		modelNube = glm::rotate(modelNube, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelNube));
		nube.Draw(lightingShader);

		glm::mat4 modelSol(1);
		modelSol = glm::translate(modelSol, glm::vec3(0.0f, 4.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSol));
		//sol.Draw(lightingShader);

		glm::mat4 modelFlor(1);
		modelFlor = glm::translate(modelFlor, glm::vec3(-1.5f, -1.0f, 0.98f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFlor));
		flor.Draw(lightingShader);

		glm::mat4 modelFlor2(1);
		modelFlor2 = glm::translate(modelFlor2, glm::vec3(1.5f, -1.0f, 0.98f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFlor2));
		flor.Draw(lightingShader);

		glm::mat4 modelHongo(1);
		modelHongo = glm::translate(modelHongo, glm::vec3(0.2f, -0.89f, 1.28f));
		modelHongo = glm::scale(modelHongo, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHongo));
		hongo.Draw(lightingShader);

		// ---------- Modelos luminosos del panteon (opacos) ----------
		// Fogata
		glm::mat4 modelFogata(1);
		modelFogata = glm::translate(modelFogata, posFogata);
		modelFogata = glm::scale(modelFogata, glm::vec3(ESC_FOGATA));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFogata));
		fogata.Draw(lightingShader);

		// Vela
		glm::mat4 modelVela(1);
		modelVela = glm::translate(modelVela, posVela);
		modelVela = glm::scale(modelVela, glm::vec3(ESC_VELA));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVela));
		vela.Draw(lightingShader);

		// Luciernaga (vuela y gira hacia donde avanza)
		glm::mat4 modelLuciernaga(1);
		modelLuciernaga = glm::translate(modelLuciernaga, posLuc);
		modelLuciernaga = glm::rotate(modelLuciernaga, rumboLuciernaga, glm::vec3(0.0f, 1.0f, 0.0f));
		modelLuciernaga = glm::scale(modelLuciernaga, glm::vec3(ESC_LUCIERNAGA));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLuciernaga));
		luciernaga.Draw(lightingShader);

		// Farol
		glm::mat4 modelFarol(1);
		modelFarol = glm::translate(modelFarol, posFarol);
		modelFarol = glm::scale(modelFarol, glm::vec3(ESC_FAROL));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFarol));
		farol.Draw(lightingShader);

		// Veladora
		glm::mat4 modelVeladora(1);
		modelVeladora = glm::translate(modelVeladora, posVeladora);
		modelVeladora = glm::scale(modelVeladora, glm::vec3(ESC_VELADORA));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVeladora));
		veladora.Draw(lightingShader);

		glm::mat4 modelLuna(1);
		modelLuna = glm::translate(modelLuna, glm::vec3(1.2f, 6.0f, 1.8f));
		modelLuna = glm::scale(modelLuna, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLuna));
		luna.Draw(lightingShader);

		// Modelo Tumba transparente ------------------------------
		// Crear la matriz del modelo
		glm::mat4 modelT = glm::mat4(1.0f);
		modelT = glm::translate(modelT, glm::vec3(-0.8f, -1.2f, 0.0f));
		modelT = glm::rotate(modelT, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelT = glm::scale(modelT, glm::vec3(0.5f, 0.5f, 0.5f));
		// Activar transparencia
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelT));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		TumbaTransparente.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		//lampShader.Use();
		//// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//modelLoc = glGetUniformLocation(lampShader.Program, "model");
		//viewLoc = glGetUniformLocation(lampShader.Program, "view");
		//projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//// Set matrices
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//// Draw the light object (using light's vertex attributes)
		//for (GLuint i = 0; i < 5; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);



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
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;//
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
/*Integrar 5 fuentes de luz a nuestro escenario: direccion, puntual y de reflexion

Miercoles 11 de noviembre entrega de proyecto*/