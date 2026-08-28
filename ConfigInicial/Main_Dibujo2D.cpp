//Practica 2
//Valdez Hernandez Alan Josue
//Fecha de entrega: 26 de agosto de 2026
//Numero de cuenta: 421122006

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

#include <vector>
#include <cmath>

// Genera vértices (x,y,z, r,g,b) de un círculo/elipse en forma de abanico (TRIANGLE_FAN)
std::vector<float> generarCirculo(float cx, float cy, float radioX, float radioY,
	float r, float g, float b, int segmentos = 40)
{
	std::vector<float> vertices;

	// Centro (primer vértice del fan)
	vertices.insert(vertices.end(), { cx, cy, 0.0f, r, g, b });

	for (int i = 0; i <= segmentos; i++)
	{
		float angulo = 2.0f * 3.14159265f * i / segmentos;
		float x = cx + radioX * cos(angulo);
		float y = cy + radioY * sin(angulo);
		vertices.insert(vertices.end(), { x, y, 0.0f, r, g, b });
	}
	return vertices;
}

struct Pieza {
	GLuint VAO, VBO;
	int numVertices;
};

Pieza crearPieza(const std::vector<float>& datos)
{
	Pieza p;
	p.numVertices = datos.size() / 6; // 6 floats por vértice (x,y,z,r,g,b)

	glGenVertexArrays(1, &p.VAO);
	glGenBuffers(1, &p.VBO);

	glBindVertexArray(p.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, p.VBO);
	glBufferData(GL_ARRAY_BUFFER, datos.size() * sizeof(float), datos.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	return p;
}



int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 - Alan Valdez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);
	//Cargar informacion de los archivos que estan por separado
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		//X     Y     Z       R     G     B
		0.5f,  0.5f, 0.0f,    1.0f,0.0f,0.0f,  // top right
		0.5f, -0.5f, 0.0f,    1.0f,1.0f,0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f,0.0f,1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f,1.0f,0.0f, // top left 
	};

	//indices de cada uno de los vertices de 2 triangulos
	// (OpenGL dibuja primitivas como triángulos, no un cuadrado directamente.): 
	unsigned int indices[] = {  // note that we start from 0!
		3,2,1,// second Triangle
		0,1,3,
		
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	std::vector<Pieza> hamster;

	// Colores aproximados: cuerpo café claro, orejas/mejillas más oscuras, ojos/nariz negro
	hamster.push_back(crearPieza(generarCirculo(0.0f, -0.2f, 0.5f, 0.35f, 0.85f, 0.65f, 0.35f))); // cuerpo (elipse)
	hamster.push_back(crearPieza(generarCirculo(0.0f, 0.25f, 0.32f, 0.28f, 0.85f, 0.65f, 0.35f))); // cabeza
	hamster.push_back(crearPieza(generarCirculo(-0.22f, 0.5f, 0.1f, 0.1f, 0.7f, 0.5f, 0.3f)));   // oreja izq
	hamster.push_back(crearPieza(generarCirculo(0.22f, 0.5f, 0.1f, 0.1f, 0.7f, 0.5f, 0.3f)));   // oreja der
	hamster.push_back(crearPieza(generarCirculo(-0.12f, 0.22f, 0.04f, 0.04f, 0, 0, 0)));         // ojo izq
	hamster.push_back(crearPieza(generarCirculo(0.12f, 0.22f, 0.04f, 0.04f, 0, 0, 0)));         // ojo der
	hamster.push_back(crearPieza(generarCirculo(0.0f, 0.13f, 0.03f, 0.025f, 0, 0, 0)));        // nariz
	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		for (auto& pieza : hamster)
		{
			glBindVertexArray(pieza.VAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, pieza.numVertices);
			glBindVertexArray(0);
		}
        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
