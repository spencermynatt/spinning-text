
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
const char* vertex_shader =

"#version 430 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 textures;\n"
"out vec2 evt;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"evt = textures;\n"
	//secondish step of the transformations
"gl_Position = transform * vec4(position, 1.0);\n" //multiply our transform which is rotating as the same position, then set it as a uniform to use later
"}\0";
const char* fragment_shader =
"#version 430 core\n"
"out vec4 new_textures;\n"
"in vec2 evt;\n"
"uniform sampler2D sampTexture;\n" //variable for empty_Texture to put data in
"void main()\n"
"{\n"
"new_textures = texture(sampTexture, evt);\n" // if you set it to zero, it won't run
"}\n\0";
int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "RECTANGLE", NULL, NULL);
	if (window == NULL)
	{
		cout << "error";

	};

	glfwMakeContextCurrent(window);
	glewInit();
	glViewport(0, 0, 800, 600);



	int vertex = glCreateShader(GL_VERTEX_SHADER);
	int fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const char* pointer_to_shader[1];
	pointer_to_shader[0] = vertex_shader;
	glShaderSource(vertex, 1, pointer_to_shader, 0);


	pointer_to_shader[0] = fragment_shader;
	glShaderSource(fragment, 1, pointer_to_shader, 0);
	glCompileShader(vertex);
	glCompileShader(fragment);
	int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	//store only the unique vertices and then specify the order at which
	//we want to draw these vertices in.

	float vertices[] = {
		// Positions        // Texture Coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 0
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 2
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // 3
	};

	int elements[] = {
		0, 1, 2, // first	
		2, 3, 0, // second	
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //we can't set zero to stride because our array is not tightly packed with one attribute
	// each coordinate in each vertex takes up 4 bytes. That means it takes 20 bytes each line
	//just setting it to zero will confused opengl into not knowing where to separate the data

	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//3 *size of float is just where it starts
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrchannels;
	unsigned char* data = SOIL_load_image("resources/bathroom.jpg", &width, &height, &nrchannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.7f, 0.5f, 2.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture);
		//THIRD STEP OF TRANSFORMING
		glm::mat4 transform = glm::mat4(1.0f); 
		//define a mat4 and explicitly initialize it to the
				// identity matrix by initializing the 
				//matrix's diagonals to 1.0, everything else is zero
				//[ 1, 0, 0, TX,         x       0
				//  0, 1, 0, TY,    *    y       0
				//  0, 0, 1, TZ,         z       0
                              //    0, 0, 0, 1 ]         1       1
				//this happens third
 
                  //glm::vec3 can be whatever location you want it to be
		//transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));  //this just moves it, you don't need it to rotate something lol
		// translation vector will mulitply to glm::vec3, its 3 because w is always 1
		//move by how many points, for now it'll just be zero
		//all three values will become 0.0f except  w
		
		
		                //transform matrix       rectangle  position        position we want
	                        //[ 1, 0, 0,  0,        0.5f,  0.5f, 0.0f,         0.5f    =     
				//  0, 1, 0,  0,    *   0.5f, -0.5f, 0.0f,     *   -0.5f   =                  our new position
				//  0, 0, 1,  0,        -0.5f, -0.5f, 0.0f,         0.0f    =     
                              //    0, 0, 0,  1 ]        -0.5f,  0.5f, 0.0f,          1                           
		//FINAL STEP ROTATE OR TRANSLATE
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
               
		
		//rotation math
		
		//first part just gets the position
		//rotation matrix on x axis
		// [1,  0,    0,  0            0.5f,  0.5f, 0.0f
		//  0, cos, -sin, 0   *        0.5f, -0.5f, 0.0f,
		//  0  sin,  cos, 0           -0.5f, -0.5f, 0.0f,
		//  0,   0,   0,  1]           -0.5f,  0.5f, 0.0f,
		
		//rotation matrix on y axis
		// [cos, 0, sin,  0            0.5f,  0.5f, 0.0f
		//  0,   1,  0,   0   *        0.5f, -0.5f, 0.0f,                          *     x
		//  -sin, 0,  cos, 0           -0.5f, -0.5f, 0.0f,                               y
		//  0,   0,   0,  1]           -0.5f,  0.5f, 0.0f,                               z
	        //                                                                               1	
		                                
		//rotation matrix on z axis
		// [cos, -sin, 0,  0            0.5f,  0.5f, 0.0f
		//  sin, cos,  0,  0   *        0.5f, -0.5f, 0.0f,
		//  0,    0,   1,  0           -0.5f, -0.5f, 0.0f,
		//  0,    0,   0,  1]           -0.5f,  0.5f, 0.0f,
		
		
		 
	
		//glfw get time is the amount of time since glfw was initilizied
		//it will be saved into a float
		//it will multiplied by a random number that increased or decreases
		//the last function of glm roate decides which axis to rotate on
		// 3 it will rototate around the z axis
		// get matrix's uniform location and set matrix
		glUseProgram(program);
		
		
		//1. THIS IS THE FIRST STEP OF THE PROCESS OF TRANSFORMING
		unsigned int transformLoc = glGetUniformLocation(program, "transform");
		//get location of transform and save it as a variable
		//SECOND STEP OF THE TRANSFORMING
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                 //transformloc is the variable
		//1 is how many matrixs
		//we're not transposing it so it's GL_FALSE
		//and we're sending all our matrix rotation data to the vertex shader in this function
		  //equation we need, the 1 is just the w
		// transform * vec4(position, 1.0);
		///[ 1, 0, 0,  0,        0.5f,  0.5f, 0.0f,
		//  0, 1, 0,  0,    *   0.5f, -0.5f, 0.0f, 
		//  0, 0, 1,  0,        -0.5f, -0.5f, 0.0f,
		 //    0, 0, 0,  1 ]    -0.5f,  0.5f, 0.0f,
		glBindVertexArray(VAO);

		

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
