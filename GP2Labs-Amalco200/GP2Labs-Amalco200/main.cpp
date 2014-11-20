//Header files
#include <iostream>

//header file for glew
#include <GL/glew.h>

//header for sdl2 functionality
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>

//includes for opengl headers
#include <SDL_opengl.h>
#include <gl\GLU.h>

//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "assets/";
const std::string SHADER_PATH = "/shaders/";
const std::string TEXTURE_PATH = "/textures/";
const std::string FONT_PATH = "/fonts/";
const std::string MODEL_PATH = "/models/";
#else
const std::string ASSET_PATH = "/assets";
const std::string SHADER_PATH = "/shaders";
const std::string TEXTURE_PATH = "/textures";
const std::string FONT_PATH = "/fonts";
const std::string MODEL_PATH = "/models";
#endif

//includes for custom headers made by me
#include "Vertex.h"
#include "shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "FBXLoader.h"

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

//vertex array object
GLuint VAO;

// texture int (ID???)
GLuint texture = 0;

GLuint fontTexture;


std::vector<GameObject*> displayList;

GameObject * mainCamera;

//sdl gl context
SDL_GLContext glcontext = NULL;

void CheckForErrors()
{
	GLenum error;
	do{
		error = glGetError();
	} while (error != GL_NO_ERROR);
}

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
	//// delete the texture
	//glDeleteTextures(1, &texture);
	//glDeleteTextures(1, &fontTexture);

	auto iter = displayList.begin();
	while (iter != displayList.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = displayList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	displayList.clear();

	//clean geometry stuff
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
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
	////screen ratio
	//GLfloat ratio;

	//make sure height is always above 0
	if (height == 0)
	{
		height = 1;
	}

	////calculate screen ratio
	//ratio = (GLfloat)width / (GLfloat)height;

	//setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
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

void Initialise()
{
	mainCamera = new GameObject();
	Transform * transform = new Transform();
	transform->setPosition(0.0f, 0.0f, 10.0f);
	mainCamera->setTransform(transform);

	Camera * camera = new Camera();
	camera->setAspectRatio((float)(WINDOW_WIDTH / WINDOW_HEIGHT));
	camera->setFOV(45.0f);
	camera->setNearClip(0.1f);
	camera->setFarClip(1000.0f);

	mainCamera->setCamera(camera);
	displayList.push_back(mainCamera);

	std::string modelPath = ASSET_PATH + MODEL_PATH + "armoredrecon.fbx";
	GameObject* go = loadFBXFromFile(modelPath);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material* material = new Material();
		material->init();
		std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
		std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
		material->loadShader(vsPath, fsPath);

		go->getChild(i)->setMaterial(material);
	}
	displayList.push_back(go);

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->init();
	}
}

void renderGameObject(GameObject *pObject)
{
	if (!pObject)
	{
		return;
	}

	(pObject)->render();
	Mesh * currentMesh = (pObject)->getMesh();
	Transform * currentTransform = (pObject)->getTransform();
	Material * currentMaterial = (pObject)->getMaterial();

	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMesh->bind();
		currentMaterial->bind();

		GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
		Camera * camera = mainCamera->getCamera();
		mat4 MVP = camera->getProjectionMatrix()*camera->getViewMatrix()*currentTransform->getModel();
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < pObject->getChildCount(); i++)
	{
		renderGameObject(pObject->getChild(i));
	}
}

//function to draw shizzle
void Render()
{
	//set the clear colourt which is the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//clear colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		renderGameObject((*iter));
	}

	//require to swap front and back buffers
	SDL_GL_SwapWindow(window);
}

//function to update game state
void Update()
{
	//old code, not sure if needed
	////this one for 3d
	//projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	////this one for 2d font stuff
	////projMatrix = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, 0.1f, 100.0f);

	//viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//worldMatrix = glm::translate(mat4(1.0f), vec3(-4.0f, -3.0f, 0.0f));

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->update();
	}
}

void createTexture()
{
	std::string texturePath = ASSET_PATH + TEXTURE_PATH + "/texImage.png";

	texture = loadTextureFromFile(texturePath);


}

void createFontTexture()
{
	std::string fontPath = ASSET_PATH + FONT_PATH + "/OratorStd.otf";

	fontTexture = loadTextureFromFont(fontPath, 20, "I'm Batman");


}

//void initGeometryFromTexture(GLuint textureID)
//{
//	int width, height;
//
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
//	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
//
//	Vertex spriteData[] = {
//
//			{ vec3(10.0, 10.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },//	Top	Left
//
//			{ vec3(10.0f, height + 10, 0.0f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },//	Bottom	Left
//
//			{ vec3(width + 10, height + 10, 0.0f), vec2(1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },	//Bottom	Right
//
//			{ vec3(width + 10, 10.0f, 0.0f), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }//	Top	Right
//	};
//
//	GLuint spriteIndices[] = {
//		0, 1, 2,
//		0, 3, 2
//	};
//
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glGenBuffers(1, &triangleVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
//	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), spriteData, GL_STATIC_DRAW);
//
//	glGenBuffers(1, &triangleEBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), spriteIndices, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) +sizeof(vec2)));
//
//}

//Main method - program entry point
int main(int argc, char*arg[])
{
	//init everything -SDL, if it is not zero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return-1;
	}

	//init sdl ttf for fonts
	if (TTF_Init() == -1)
	{
		std::cout << "ERROr in ttf init: " << TTF_GetError();
	}

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)&(imageInitFlags)) != imageInitFlags)
	{
		std::cout << "ERROR SDL_Image Init" << IMG_GetError() << std::endl;
		//handle error here
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	//call our init opengl function
	initOpenGL();
	CheckForErrors();
	//set the wee viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	Initialise();

	//removed function calls
	//InitGeometry();
	//createTexture();
	//createShader();
	//for 2d font thing
	//createFontTexture();
	//initGeometryFromTexture(fontTexture);

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

				}
					break;

					//if down
				case SDLK_DOWN:
				{

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