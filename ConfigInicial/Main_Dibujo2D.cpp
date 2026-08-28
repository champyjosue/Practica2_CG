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


int main() {
	glfwInit();
	//Verificaciï¿½n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 - Alan Valdez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificaciï¿½n de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaciï¿½n de errores de inicializaciï¿½n de glew

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

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// X,      Y,      Z,     R,   G,   B
		   // --- LÍNEA CENTRAL ---
		   0.0f,   0.75f,  0.0f,  0.0f,0.0f,0.0f, // 0: Top frente
		   0.0f,   0.00f,  0.0f,  0.0f,0.0f,0.0f, // 1: Centro nariz
		   0.0f,  -0.20f,  0.0f,  0.0f,0.0f,0.0f, // 2: Boca superior
		   0.0f,  -0.60f,  0.0f,  0.0f,0.0f,0.0f, // 3: Centro pecho base

		   // --- CONTORNO EXTERNO (Mitad Izquierda) ---
		  -0.25f,  0.70f,  0.0f,  0.0f,0.0f,0.0f, // 4: Esquina cabeza izq
		  -0.45f,  0.45f,  0.0f,  0.0f,0.0f,0.0f, // 5: Sien izq
		  -0.70f, -0.30f,  0.0f,  0.0f,0.0f,0.0f, // 6: Mejilla ancha izq
		  -0.50f, -0.70f,  0.0f,  0.0f,0.0f,0.0f, // 7: Base inferior izq
		  -0.30f, -0.70f,  0.0f,  0.0f,0.0f,0.0f, // 8: Pata izq

		  // --- CONTORNO EXTERNO (Mitad Derecha - Espejo) ---
		  0.25f,  0.70f,  0.0f,  0.0f,0.0f,0.0f, // 9: Esquina cabeza der
		  0.45f,  0.45f,  0.0f,  0.0f,0.0f,0.0f, // 10: Sien der
		  0.70f, -0.30f,  0.0f,  0.0f,0.0f,0.0f, // 11: Mejilla ancha der
		  0.50f, -0.70f,  0.0f,  0.0f,0.0f,0.0f, // 12: Base inferior der
		  0.30f, -0.70f,  0.0f,  0.0f,0.0f,0.0f, // 13: Pata der

		  // --- ROMBO NARIZ ---
		 -0.08f, -0.10f,  0.0f,  0.0f,0.0f,0.0f, // 14: Nariz izq
		  0.08f, -0.10f,  0.0f,  0.0f,0.0f,0.0f, // 15: Nariz der
		  0.00f, -0.18f,  0.0f,  0.0f,0.0f,0.0f, // 16: Nariz abajo

		  // --- OJOS Y TRIÁNGULOS FACIALES ---
		 -0.25f,  0.25f,  0.0f,  0.0f,0.0f,0.0f, // 17: Ojo izq centro
		  0.25f,  0.25f,  0.0f,  0.0f,0.0f,0.0f, // 18: Ojo der centro

		  // --- OREJAS ---
		  0.42f,  0.88f,  0.0f,  0.0f,0.0f,0.0f, // 19: Punta oreja derecha
	  0.16f,  0.71f,  0.0f,  0.0f,0.0f,0.0f, // 20: Base superior oreja der
	  0.38f,  0.55f,  0.0f,  0.0f,0.0f,0.0f, // 21: Base media oreja der
	 -0.42f,  0.88f,  0.0f,  0.0f,0.0f,0.0f, // 22: Punta oreja izquierda
	 -0.19f,  0.72f,  0.0f,  0.0f,0.0f,0.0f, // 23: Base superior oreja izq
	 -0.38f,  0.55f,  0.0f,  0.0f,0.0f,0.0f, // 24: Base media oreja izq
	};

	// Arreglo de índices para dibujar todas las aristas internas y externas con GL_LINES
	unsigned int indices[] = {
		// Línea central
		0, 1,   1, 2,   2, 3,

		// Contorno principal
		0, 4,   4, 5,   5, 6,   6, 7,   7, 8,   8, 3,
		0, 9,   9, 10,  10, 11, 11, 12, 12, 13, 13, 3,

		// Rombo de la nariz
		0, 1,   1, 14,  14, 16, 16, 15, 15, 1,

		// Conexiones faciales internas (Triangulación)
		4, 17,  5, 17,  1, 17,
		9, 18,  10, 18, 1, 18,
		6, 14,  6, 3,   11, 15, 11, 3,

		// Orejas (abanico de 3 líneas cada una)
		19, 20,   19, 21,   19, 9,     // oreja derecha: punta -> base sup, base media, y esquina cabeza (vértice 9)
		22, 23,   22, 24,   22, 4      // oreja izquierda: punta -> base sup, base media, y esquina cabeza (vértice 4)

	};



	GLuint VBO, VAO, EBO;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)



	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);

		glLineWidth(2.5f);
		// Renderiza todas las aristas combinadas mediante el EBO
		glDrawElements(GL_LINES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

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