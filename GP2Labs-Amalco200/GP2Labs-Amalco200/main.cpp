//Header files
#include <iostream>

//header file for glew
#include <GL/glew.h>

//header for sdl2 functionality
#include <SDL.h>

//includes for opengl headers
#include <SDL_opengl.h>
#include <gl\GLU.h>

//includes for custom headers made by me
#include "Vertex.h"

//Global variables here
//pointer to SDL windows
SDL_Window*window;

//constants to control window creation
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

//while running this is true
bool running = true;

// vertex buffer object variable
GLuint triangleVBO;

//triangle data now contains both rgba and xyz
Vertex triangleData[] = {
							{ 0.0f, 1.0f, 0.0f,				//xyz
							1.0f, 0.0f, 0.0f, 1.0f },		//rgba

						{ -1.0f, -1.0f, 0.0f,				//xyz
							0.0f, 1.0f, 0.0f, 1.0f },		//rgba

						{ 1.0f, -1.0f, 0.0f,				//xyz
							0.0f, 0.0f, 1.0f, 1.0f }		//rgba
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
	glDeleteBuffers(1, &triangleVBO);

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//initialise opengl
void initOpenGL()
{
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

	//change to project matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate perpective matrix using glu lib functions
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	//switch to model view
	glMatrixMode(GL_MODELVIEW);

	//reset using identity matrix
	glLoadIdentity();
}

void InitGeometry()
{
	//create buffer
	glGenBuffers(1, &triangleVBO);

	//make vbo active
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//copy vertex data into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);
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

	//the 3 parameter is now filled out, the pipeline needs to know the size of each vertex
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);

	//the last parameter basically says that the colours starts 3 floats into each element of the array
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void**)(3 * sizeof(float)));

	//establish array contains vertices and normals colours
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//switch to model view
	glMatrixMode(GL_MODELVIEW);

	//triangle 1
		//reset using identity matrix
		glLoadIdentity();

		//2dspace to 3dspace
		gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

		//translate
		glTranslatef(-1.25f, -1.25f, -6.0f);
		//look at this for 2D camera
		//glOrtho() or gluOrtho2D

		//Look at the 3D
		//gluLookAt

		//actually draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / sizeof(Vertex));

		//triangle 2
		//reset using identity matrix
		glLoadIdentity();

		//2dspace to 3dspace
		gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

		//translate
		glTranslatef(1.25f, 1.25f, -6.0f);

		//actually draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / sizeof(Vertex));


	//require to swap front and back buffers
	SDL_GL_SwapWindow(window);
}

//function to update game state
void Update()
{

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

				}
					break;

					//if right
				case SDLK_RIGHT:
				{
					
				}
					break;

					//if up
				case SDLK_UP:
				{
					
				}
					break;

					//if down
				case SDLK_DOWN:
				{
					
				}
					break;

					//not sure what the default is for? ask Brian.
				default:
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