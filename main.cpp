#include <iostream>
//Including GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//INCLUDING GLFW
#include <GLFW/glfw3.h>

const GLuint WIDTH = 1024, HEIGHT = 768;

const GLchar* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"layout (location = 1) in vec3 color;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position, 1.0);\n"
	"ourColor = color;\n"
	//"vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
	"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
	"out vec4 color;\n"
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"color = vec4(ourColor, 1.0f);\n"
	"}\n\0";

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//When a user presses the escape key, we set the WindowShouldClose property to true,
	//closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
	//------------------------------------------------------//
	//Window setup											//
	//------------------------------------------------------//
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	//Initialize GLFW
	glfwInit();
	//Set required GLFW options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//Create a GLFW window object, GLFW's functions can be used with this
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ORANGE_FUCKING_TRIANGLE", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	//Set this to true so GLEW uses experimental extensions, etc.
	glewExperimental = GL_TRUE;
	glewInit();
	//Initialize GLEW to setup OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to Initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
	//define viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	//------------------------------------------------------//
	//End Window setup										//
	//------------------------------------------------------//

	GLint success;
	GLchar infoLog[512];
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] = {
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	//GLuint indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};
	//create unsigned GL integers 
	GLuint VBO, VAO; //EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events have been logged, call corresponding functions
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		//wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//swap screen buffers
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//Terminate GLFW, clearing up any resources used by GLFW
	glfwTerminate();
	return 0;
}