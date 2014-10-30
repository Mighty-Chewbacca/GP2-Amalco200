#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

GLuint loadTextureFromFile(const std::string& filename);

#endif