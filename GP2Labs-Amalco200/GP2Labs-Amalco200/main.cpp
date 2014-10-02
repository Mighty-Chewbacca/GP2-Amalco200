//Header files
#include <iostream>

//header file for glew
#include <GL/glew.h>

//header for sdl2 functionality
#include <SDL.h>

//includes for opengl headers
#include <SDL_opengl.h>
#include <gl\GLU.h>

//Global variables here
//pointer to SDL windows
SDL_Window*window;

//constants to control window creation
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

//while running this is true
bool running = true;

//floats for 1 triangle, v is vor vertice, number is number of vertice and final letter is for the part of the coord it relates to.
//if doing this for multiple i will be adding in T and a number as well, to represent triangle
//need a more efficient way of doing this
float v1X = 0.0f, v2X = -1.0f, v3X = 1.0f, v1Y = 1.0f, v2Y = -1.0f, v3Y = -1.0f, v1Z = 0.0f, v2Z = 0.0f, v3Z = 0.0f;

// vertex buffer object variable
GLuint triangleVBO;

//triangle data
float triangleData[] = { 0.0f, 1.0f, 0.0f			//top
						- 1.0f, -1.0f, 0.0f,		//bottom left
						1.0f, -1.0f, 0.0f };		//bottom right

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

//used to create one of the points of the triangle, takes 3 floats which each become part of the coord of the point
void CreatePoint(float x, float y, float z)
{
	glVertex3f(x, y, z);
}

//used to create the triangle, takes in 9 floats which will become the 9 coords for the vertices
void CreateTriangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3)
{
	glColor3f(1.0f, 0.0f, 0.0f);			//vertice colours
	CreatePoint(x1, y1, z1);
	CreatePoint(x2, y2, z2);
	CreatePoint(x3, y3, z3);
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

	//establish 3 coords per vertex with zero stride (space between elements)
	//in array and contain floating point numbers
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//establish array contains vertices and not normals colours or tex coords
	glEnableClientState(GL_VERTEX_ARRAY);

	//switch to model view
	glMatrixMode(GL_MODELVIEW);

	//triangle 1
		//reset using identity matrix
		glLoadIdentity();

		//translate
		glTranslatef(0.0f, 0.0f, -6.0f);

		//actually draw the triangle
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);


	////triangle 2
	//	//reset using identity matrix
	//	glLoadIdentity();

	//	//translate
	//	glTranslatef(1.0f, 1.0f, -6.0f);

	//	//actually draw the triangle
	//	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//	glDrawArrays(GL_TRIANGLES, 0, 3);

	//require to swap front and back buffers
	SDL_GL_SwapWindow(window);
}

//function to update game state
void Update()
{
	////move triangle test!!!
	//v1X = v1X + 0.01f;
	//v2X = v2X + 0.01f;
	//v3X = v3X + 0.01f;


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
					v1X = v1X - 0.01f;
					v2X = v2X - 0.01f;
					v3X = v3X - 0.01f;
				}
					break;

					//if right
				case SDLK_RIGHT:
				{
					v1X = v1X + 0.01f;
					v2X = v2X + 0.01f;
					v3X = v3X + 0.01f;
				}
					break;

					//if up
				case SDLK_UP:
				{
					v1Y = v1Y + 0.01f;
					v2Y = v2Y + 0.01f;
					v3Y = v3Y + 0.01f;
				}
					break;

					//if down
				case SDLK_DOWN:
				{
					v1Y = v1Y - 0.01f;
					v2Y = v2Y - 0.01f;
					v3Y = v3Y - 0.01f;
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