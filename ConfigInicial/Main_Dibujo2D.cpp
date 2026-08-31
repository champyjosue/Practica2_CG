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
		0.0f,   0.82f,  0.0f,  0.0f,0.0f,0.0f, // 0: Top frente
		0.0f,   0.00f,  0.0f,  0.0f,0.0f,0.0f, // 1: Centro nariz
		0.0f,  -0.24f,  0.0f,  0.0f,0.0f,0.0f, // 2: Boca superior
		0.0f,  -0.60f,  0.0f,  0.0f,0.0f,0.0f, // 3: Centro pecho base

		// --- CONTORNO EXTERNO (Mitad Izquierda) ---
		-0.25f,  0.70f,  0.0f,  0.0f,0.0f,0.0f, // 4: Esquina cabeza 
		-0.45f,  0.45f,  0.0f,  0.0f,0.0f,0.0f, // 5: Sien izq
		-0.62f, -0.30f,  0.0f,  0.0f,0.0f,0.0f, // 6: Mejilla ancha izq
		-0.50f, -0.70f,  0.0f,  0.0f,0.0f,0.0f, // 7: Base inferior izq
		-0.30f, -0.70f,  0.0f,  0.0f,0.0f,0.0f, // 8: Pata izq

		// --- CONTORNO EXTERNO (Mitad Derecha) ---
		0.25f,  0.70f,  0.0f,  0.0f,0.0f,0.0f, // 9: Esquina cabeza der
		0.45f,  0.45f,  0.0f,  0.0f,0.0f,0.0f, // 10: Sien der
		0.62f, -0.30f,  0.0f,  0.0f,0.0f,0.0f, // 11: Mejilla ancha der
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
		
		 // Para los ojos
		0.25f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 25: centro ojo der
		0.33f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 26
		0.315f,0.297f, 0.0f,  0.0f,0.0f,0.0f, // 27
		0.275f,0.326f, 0.0f,  0.0f,0.0f,0.0f, // 28
		0.225f,0.326f, 0.0f,  0.0f,0.0f,0.0f, // 29
		0.185f,0.297f, 0.0f,  0.0f,0.0f,0.0f, // 30
		0.17f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 31
		0.185f,0.203f, 0.0f,  0.0f,0.0f,0.0f, // 32
		0.225f,0.174f, 0.0f,  0.0f,0.0f,0.0f, // 33
		0.275f,0.174f, 0.0f,  0.0f,0.0f,0.0f, // 34
		0.315f,0.203f, 0.0f,  0.0f,0.0f,0.0f, // 35
		0.33f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 36:

		-0.25f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 37: centro ojo izq
		-0.33f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 38
		-0.315f,0.297f, 0.0f,  0.0f,0.0f,0.0f, // 39
		-0.275f,0.326f, 0.0f,  0.0f,0.0f,0.0f, // 40
		-0.225f,0.326f, 0.0f,  0.0f,0.0f,0.0f, // 41
		-0.185f,0.297f, 0.0f,  0.0f,0.0f,0.0f, // 42
		-0.17f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 43
		-0.185f,0.203f, 0.0f,  0.0f,0.0f,0.0f, // 44
		-0.225f,0.174f, 0.0f,  0.0f,0.0f,0.0f, // 45
		-0.275f,0.174f, 0.0f,  0.0f,0.0f,0.0f, // 46
		-0.315f,0.203f, 0.0f,  0.0f,0.0f,0.0f, // 47
		-0.33f, 0.25f,  0.0f,  0.0f,0.0f,0.0f, // 48

		// --- BIGOTES ---
		0.10f, -0.12f,  0.0f,  0.0f,0.0f,0.0f, // 49: origen bigotes der
		0.65f, -0.02f,  0.0f,  0.0f,0.0f,0.0f, // 50: punta bigote der 1
		0.70f, -0.12f,  0.0f,  0.0f,0.0f,0.0f, // 51: punta bigote der 2
		0.65f, -0.22f,  0.0f,  0.0f,0.0f,0.0f, // 52: punta bigote der 3

		-0.10f, -0.12f,  0.0f,  0.0f,0.0f,0.0f, // 53: origen bigotes izq
		-0.65f, -0.02f,  0.0f,  0.0f,0.0f,0.0f, // 54: punta bigote izq 1
		-0.70f, -0.12f,  0.0f,  0.0f,0.0f,0.0f, // 55: punta bigote izq 2
		-0.65f, -0.22f,  0.0f,  0.0f,0.0f,0.0f, // 56: punta bigote izq 3

		// --- DEDOS PATA IZQUIERDA ---
		-0.44f, -0.80f,  0.0f,  0.0f,0.0f,0.0f, // 57: punta dedo 1 izq
		-0.43f, -0.72f,  0.0f,  0.0f,0.0f,0.0f, // 58: valle 1 izq
		-0.31f, -0.80f,  0.0f,  0.0f,0.0f,0.0f, // 59: punta dedo 3 izq

		// --- DEDOS PATA DERECHA ---
		0.44f, -0.80f,  0.0f,  0.0f,0.0f,0.0f, // 60: punta dedo 1 der
		0.43f, -0.72f,  0.0f,  0.0f,0.0f,0.0f, // 61: valle 1 der
		0.31f, -0.80f,  0.0f,  0.0f,0.0f,0.0f, // 62: punta dedo 3 der

		// --- BOQUITA (pequeño rombo bajo la nariz) ---
		-0.05f, -0.21f,  0.0f,  0.0f,0.0f,0.0f, // 63: boca izq
		0.00f, -0.25f,  0.0f,  0.0f,0.0f,0.0f, // 64: boca abajo
		0.05f, -0.21f,  0.0f,  0.0f,0.0f,0.0f, // 65: boca der

		// --- BRILLO DE LOS OJOS (blanco) ---
		0.29f,  0.28f,  0.0f,  1.0f,1.0f,1.0f, // 66: brillo ojo der
		-0.23f,  0.28f,  0.0f,  1.0f,1.0f,1.0f, // 67: brillo ojo izq

		// --- ARCO SUPERIOR PATA IZQUIERDA  ---
		-0.47f, -0.66f,  0.0f,  0.0f,0.0f,0.0f, // 68
		-0.40f, -0.63f,  0.0f,  0.0f,0.0f,0.0f, // 69
		-0.33f, -0.66f,  0.0f,  0.0f,0.0f,0.0f, // 70

		// --- ARCO SUPERIOR PATA DERECHA ---
		0.47f, -0.66f,  0.0f,  0.0f,0.0f,0.0f, // 71
		0.40f, -0.63f,  0.0f,  0.0f,0.0f,0.0f, // 72
		0.33f, -0.66f,  0.0f,  0.0f,0.0f,0.0f, // 73

		  // --- DEDO DE EN MEDIO (pata izquierda) ---
		-0.39f, -0.80f, 0.0f, 0.0f, 0.0f, 0.0f, // 74: punta dedo medio izq
		-0.35f, -0.72f, 0.0f, 0.0f, 0.0f, 0.0f, // 75: valle 2 izq

		// --- DEDO DE EN MEDIO (pata derecha) ---
		0.39f, -0.80f, 0.0f, 0.0f, 0.0f, 0.0f, // 76: punta dedo medio der
		0.35f, -0.72f, 0.0f, 0.0f, 0.0f, 0.0f, // 77: valle 2 der

		// --- RELLENO DE COLOR (cuerpo y cabeza) ---
		0.00f, -0.05f, 0.0f, 0.94f, 0.76f, 0.45f, // 78: centro del relleno
		0.00f, 0.82f, 0.0f, 0.94f, 0.76f, 0.45f, // 79: (dup de 0) top
		-0.25f, 0.70f, 0.0f, 0.94f, 0.76f, 0.45f, // 80: (dup de 4)
		-0.45f, 0.45f, 0.0f, 0.94f, 0.76f, 0.45f, // 81: (dup de 5)
		-0.62f, -0.30f, 0.0f, 0.94f, 0.76f, 0.45f, // 82: (dup de 6)
		-0.50f, -0.70f, 0.0f, 0.94f, 0.76f, 0.45f, // 83: (dup de 7)
		-0.30f, -0.70f, 0.0f, 0.94f, 0.76f, 0.45f, // 84: (dup de 8)
		0.00f, -0.60f, 0.0f, 0.94f, 0.76f, 0.45f, // 85: (dup de 3)
		0.30f, -0.70f, 0.0f, 0.94f, 0.76f, 0.45f, // 86: (dup de 13)
		0.50f, -0.70f, 0.0f, 0.94f, 0.76f, 0.45f, // 87: (dup de 12)
		0.62f, -0.30f, 0.0f, 0.94f, 0.76f, 0.45f, // 88: (dup de 11)
		0.45f, 0.45f, 0.0f, 0.94f, 0.76f, 0.45f, // 89: (dup de 10)
		0.25f, 0.70f, 0.0f, 0.94f, 0.76f, 0.45f, // 90: (dup de 9)
		0.00f, 0.82f, 0.0f, 0.94f, 0.76f, 0.45f, // 91: (dup de 0,

		// --- RELLENO OREJAS (rosa/durazno) ---
		0.42f, 0.88f, 0.0f, 0.95f, 0.72f, 0.62f, // 92: tip oreja der (dup de 19)
		0.16f, 0.71f, 0.0f, 0.95f, 0.72f, 0.62f, // 93: base oreja der (dup de 20)
		0.25f, 0.70f, 0.0f, 0.95f, 0.72f, 0.62f, // 94: esquina cabeza der (dup de 9)

		-0.42f, 0.88f, 0.0f, 0.95f, 0.72f, 0.62f, // 95: tip oreja izq (dup de 22)
		-0.19f, 0.72f, 0.0f, 0.95f, 0.72f, 0.62f, // 96: base oreja izq (dup de 23)
		-0.25f, 0.70f, 0.0f, 0.95f, 0.72f, 0.62f, // 97: esquina cabeza izq (dup de 4)
			
		// --- CACHETITOS ROSAS ---
		0.42f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 98: centro cachete der
		0.485f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 99
		0.466f, 0.066f, 0.0f, 0.98f, 0.75f, 0.75f, // 100
		0.42f, 0.085f, 0.0f, 0.98f, 0.75f, 0.75f, // 101
		0.374f, 0.066f, 0.0f, 0.98f, 0.75f, 0.75f, // 102
		0.355f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 103
		0.374f, -0.026f, 0.0f, 0.98f, 0.75f, 0.75f, // 104
		0.42f, -0.045f, 0.0f, 0.98f, 0.75f, 0.75f, // 105
		0.466f, -0.026f, 0.0f, 0.98f, 0.75f, 0.75f, // 106
		0.485f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 107: repite el 99 para cerrar

		-0.42f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 108: centro cachete izq
		-0.485f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 109
		-0.466f, 0.066f, 0.0f, 0.98f, 0.75f, 0.75f, // 110
		-0.42f, 0.085f, 0.0f, 0.98f, 0.75f, 0.75f, // 111
		-0.374f, 0.066f, 0.0f, 0.98f, 0.75f, 0.75f, // 112
		-0.355f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 113
		-0.374f, -0.026f, 0.0f, 0.98f, 0.75f, 0.75f, // 114
		-0.42f, -0.045f, 0.0f, 0.98f, 0.75f, 0.75f, // 115
		-0.466f, -0.026f, 0.0f, 0.98f, 0.75f, 0.75f, // 116
		-0.485f, 0.02f, 0.0f, 0.98f, 0.75f, 0.75f, // 117:

		// --- RELLENO NARIZ ---
		0.00f, 0.00f, 0.0f, 0.95f, 0.55f, 0.58f, // 118: hub (dup de 1)
		-0.08f, -0.10f, 0.0f, 0.95f, 0.55f, 0.58f, // 119: (dup de 14)
		0.00f, -0.18f, 0.0f, 0.95f, 0.55f, 0.58f, // 120: (dup de 16)
		0.08f, -0.10f, 0.0f, 0.95f, 0.55f, 0.58f, // 121: (dup de 15)

		// --- RELLENO BOCA (rosa oscurito) ---
			0.00f, -0.18f, 0.0f, 0.55f, 0.20f, 0.22f, // 122: (dup de 16, pivote del fan)
			-0.05f, -0.21f, 0.0f, 0.55f, 0.20f, 0.22f, // 123: (dup de 63)
			0.00f, -0.25f, 0.0f, 0.55f, 0.20f, 0.22f, // 124: (dup de 64)
			0.05f, -0.21f, 0.0f, 0.55f, 0.20f, 0.22f, // 125: (dup de 65)
	};

	// Arreglo de índices para dibujar todas las aristas internas y externas con GL_LINES
	unsigned int indices[] = {
		// Línea central
		0, 1,   2, 3,

		// Contorno principal
		0, 4,   4, 5,   5, 6,   6, 7,   7, 57,  57, 58,  58, 74,  74, 75,  75, 59,  59, 8,   8, 3,
		0, 9,   9, 10,  10, 11, 11, 12, 12, 60,  60, 61,  61, 76,  76, 77,  77, 62,  62, 13,  13, 3,

		// Rombo de la nariz
		0, 1,   1, 14,  14, 16, 16, 15, 15, 1,

		// Conexiones faciales internas (Triangulación)
		4, 17,  5, 17,  1, 17,
		9, 18,  10, 18, 1, 18,
		6, 14,  6, 3,   11, 15, 11, 3,

		// Orejas (abanico de 3 líneas cada una)
		19, 20,   19, 21,   19, 9,     // oreja derecha:
		22, 23,   22, 24,   22, 4,      // oreja izquierda

		// Bigotes 
		49, 50,   49, 51,   49, 52,
		53, 54,   53, 55,   53, 56,

		// Boquita
		16, 63,  63, 64,  64, 65,  65, 16,

		// Arco que cierra la patita izquierda (junto con el zigzag ya crea una forma cerrada/redondeada)
		7, 68,   68, 69,   69, 70,   70, 8,

		// Arco que cierra la patita derecha
		12, 71,  71, 72,   72, 73,   73, 13
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

		// Relleno de color (va primero, para que las líneas queden encima)
		glDrawArrays(GL_TRIANGLE_FAN, 78, 14);

		// Relleno de las orejas (2 triángulos = 6 vértices)
		glDrawArrays(GL_TRIANGLES, 92, 6);

		// Cachetitos rosas
		glDrawArrays(GL_TRIANGLE_FAN, 98, 10);  // cachete der
		glDrawArrays(GL_TRIANGLE_FAN, 108, 10); // cachete izq
		glDrawArrays(GL_TRIANGLE_FAN, 118, 4);  // nariz
		glDrawArrays(GL_TRIANGLE_FAN, 122, 4);  // boca

		glLineWidth(2.5f);
		// Dibuja las líneas usando todos los índices guardados en el EBO
		glDrawElements(GL_LINES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_TRIANGLE_FAN, 25, 12); // ojo derecho
		glDrawArrays(GL_TRIANGLE_FAN, 37, 12); // ojo izquierdo
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 66, 1); // brillo ojo derecho
		glDrawArrays(GL_POINTS, 67, 1); // brillo ojo izquierdo
		
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