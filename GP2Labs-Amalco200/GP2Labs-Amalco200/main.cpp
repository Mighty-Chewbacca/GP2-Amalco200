//Header files
#include <iostream>

//header file for glew
#include <GL/glew.h>

//header for sdl2 functionality
#include <SDL.h>

//includes for opengl headers
#include <SDL_opengl.h>
#include <gl\GLU.h>

//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//includes for custom headers made by me
#include "Vertex.h"
#include "shader.h"

//Global variables here
//pointer to SDL windows
SDL_Window*window;

//constants to control window creation
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

//while running this is true
bool running = true;

//bools for moving on axis
//going to allow user to choose an axis and then move it on that axis
bool xAxis = false;
bool yAxis = false;
bool zAxis = false;

//variables for the cubes position in the world
float xPos = 0, yPos = 0, zPos = -5;

float xPos2 = 1.8f, yPos2 = 1.255f, zPos2 = -5;

//matrices
mat4 viewMatrix;
mat4 projMatrix;
mat4 worldMatrix;

//holds shader program
GLuint shaderProgram = 0;

// vertex buffer object variable
GLuint triangleVBO;

//element buffer object
GLuint triangleEBO;

//triangle data now contains both rgba and xyz
Vertex triangleData[] = {

#pragma region Front_face
	//Front face
		{ -0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Left

		{ -0.5f, -0.5f, 0.5f,
		1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left

		{ 0.5f, -0.5f, 0.5f,
		0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right

		{ 0.5f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f, 1.0f },// Top Right
#pragma endregion

#pragma region Back_face
		//back face
		{ -0.5f, 0.5f, -0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Left

		{ -0.5f, -0.5f, -0.5f,
		1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left

		{ 0.5f, -0.5f, -0.5f,
		0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right

		{ 0.5f, 0.5f, -0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Right
#pragma endregion

};

GLuint indices[] = {

	//personal notes, every 3 numbers relates to 1 triangle, they connect to each other, the number relates to the number of the vertice and not the amount of times it is touched

	//front
	0, 1, 2, 0, 3, 2,

	//left
	4, 5, 1, 4, 0, 1,

	//right
	3, 7, 2, 7, 6, 2,

	//bottom
	1, 5, 2, 6, 2, 5,

	//top
	4, 7, 0, 0, 3, 7,

	//back
	4, 5, 6, 4, 7, 6
};



//sdl gl context
SDL_GLContext glcontext = NULL;

//Global functions
void InitWindow(int width, int height, bool fullscreen)
{
	//create window
	window = SDL_CreateWindow(
		"Lab 1",					//Window title
		SDL_WINDOWPOS_CENTERED,		//x pos centered
		SDL_WINDOWPOS_CENTERED,		//y pos centered
		width,						//width in pixels
		height,						//height in pixels
		SDL_WINDOW_OPENGL			//flags
		);
}

//used to clean up once we exit app
void CleanUp()
{
	//clean geometry stuff
	glDeleteProgram(shaderProgram);

	glDeleteBuffers(1, &triangleEBO);
	glDeleteBuffers(1, &triangleVBO);

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//initialise opengl
void initOpenGL()
{
	//ask for version 3.2 of gl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glcontext = SDL_GL_CreateContext(window);
	//something went wrong creating the context, if it is still null
	if (!glcontext)
	{
		std::cout << "Error creating opengl context" << SDL_GetError() << std::endl;
	}

	//smooth shading
	glShadeModel(GL_SMOOTH);
	//clear background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//clear depth buffer to 1.0
	glClearDepth(1.0f);
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	//depth test to use
	glDepthFunc(GL_LEQUAL);
	//turn on prespective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/*Problem: glewInitfailed, something is seriously wrong*/
		std::cout << "Error:" << glewGetErrorString(err) << std::endl;
	}
}

//function to set/reset viewport
void setViewport(int width, int height)
{
	//screen ratio
	GLfloat ratio;

	//make sure height is always above 0
	if (height == 0)
	{
		height = 1;
	}

	//calculate screen ratio
	ratio = (GLfloat)width / (GLfloat)height;

	//setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void InitGeometry()
{
	//create buffer
	glGenBuffers(1, &triangleVBO);

	//make vbo active
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//copy vertex data into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

	//create element buffer
	glGenBuffers(1, &triangleEBO);

	//make active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);

	//copy index data to ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

//method to change the axis the user is affecting
void ChangeAxis(int newAxis)
{
	if (newAxis == 0)
	{
		xAxis = true;
		yAxis = false;
		zAxis = false;
	}

	if (newAxis == 1)
	{
		xAxis = false;
		yAxis = true;
		zAxis = false;
	}

	if (newAxis == 2)
	{
		xAxis = false;
		yAxis = false;
		zAxis = true;
	}
}


//function to draw shizzle
void Render()
{
	//set the clear colourt which is the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//clear colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//make the new vbo active, repeat here as a sanity check(may have changed since initialisation)
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);

	glUseProgram(shaderProgram);

	GLint MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	mat4 MVP = projMatrix * viewMatrix * worldMatrix;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	//tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//actually draw the triangle
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	//require to swap front and back buffers
	SDL_GL_SwapWindow(window);
}

//function to update game state
void Update()
{
	projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f 1.0f, 0.0f));

	worldMatrix = glm::translate(mat4(1.0f), vec3(0.0f 0.0f, 0.0f));
}

void createShader()
{
	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);
	glLinkProgram(shaderProgram);
	checkForLinkErrors(shaderProgram);

	glBindAttribLocation(shaderProgram, 0, "vertexPosition");

	//no we can delete them
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);
}

//Main method - program entry point
int main(int argc, char*arg[])
{
	//init everything -SDL, if it is not zero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return-1;
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	//call our init opengl function
	initOpenGL();
	InitGeometry();

	//set the wee viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	createShader();

	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			//get dat event type
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				//set boolean to false to exit game loop
				running = false;
			}

			//open case statement
			switch (event.type){
				//try to find a keypress :)
				//check if key is down
			case SDL_KEYDOWN:
				//check if its the right key that is pressed
				switch (event.key.keysym.sym)
				{
					//if left
				case SDLK_LEFT:
				{
					if (xAxis)
					{
						xPos = xPos - 0.1f;
					}
					if (yAxis)
					{
						yPos = yPos - 0.1f;
					}
					if (zAxis)
					{
						zPos = zPos - 0.1f;
					}
				}
					break;

					//if right
				case SDLK_RIGHT:
				{
					if (xAxis)
					{
						xPos = xPos + 0.1f;
					}
					if (yAxis)
					{
						yPos = yPos + 0.1f;
					}
					if (zAxis)
					{
						zPos = zPos + 0.1f;
					}
				}
					break;

					//if up
				case SDLK_UP:
				{
					if (xAxis)
					{

					}

					if (yAxis)
					{

					}

					if (zAxis)
					{

					}

				}
					break;

					//if down
				case SDLK_DOWN:
				{
					if (xAxis)
					{

					}

					if (yAxis)
					{

					}

					if (zAxis)
					{

					}
				}
					break;

					//if x is pressed
				case SDLK_x:
				{
					ChangeAxis(0);
				}
					break;

					//if y is pressed
				case SDLK_y:
				{
					ChangeAxis(1);
				}
					break;

					//if z is pressed
				case SDLK_z:
				{
					ChangeAxis(2);
				}
					break;
				}
				break;
			}
		}

		Update();
		Render();
	}

	CleanUp();
	return 0;
}