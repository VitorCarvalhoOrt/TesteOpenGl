#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


//Vertices coordinates
GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 0.5f,	0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

// Order of indices to build tri's
GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};


int main()
{
	// Initialize GLFW
	glfwInit();

	// Specifify to GlFW that 
	// we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
	// Error if the window doesnt create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Specify the window thats being rendered to the current context
	glfwMakeContextCurrent(window);

	// Load GLAD
	// Ideally, we error check if it loaded properly
	gladLoadGL();
	// Scales the viewport to the window.
	glViewport(0, 0, 800, 800);


	// Use these as the shaders
	Shader shaderProgram("default.vert", "default.frag");
	// Cria e seleciona um VAO 
	VAO VAO1;
	VAO1.Bind();
	// Linka o VBO ao VAO
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	// Adiciona os atributos ao VAO, depois deslinka cada um pra evitar edita ele sem querer
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Reconhece o ID do uniform Scale
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture

	Texture gato("out.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	gato.texUnit(shaderProgram, "tex0", 0);


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// limpa o buffer pra usar a nova cor
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		gato.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	gato.Delete();
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}