#include "Material.h"
#include "Shader.h"

Material::Material()
{
	m_ShaderProgram = -1;
	m_Type = "Material";
}

Material::~Material()
{

}

void Material::destroy()
{
	glDeleteProgram(m_ShaderProgram);
}

bool Material::loadShader(const std::string& vsFilename, const std::string& fsFilename)
{
	//had to look at brians code for this bit, not sure why this is the code needed
	//ask asap
	GLuint vertexShaderProgram = 0;
	//std::string vsFilename = ASSET_PATH + SHADER_PATH + "/textureVS.glsl";
	vertexShaderProgram = loadShaderFromFile(const_cast<std::string&>(vsFilename), VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	//std::string fsFilename = ASSET_PATH + SHADER_PATH + "/textureFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(const_cast<std::string&>(fsFilename), FRAGMENT_SHADER);

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShaderProgram);
	glAttachShader(m_ShaderProgram, fragmentShaderProgram);
	glLinkProgram(m_ShaderProgram);
	checkForLinkErrors(m_ShaderProgram);


	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

	glBindAttribLocation(m_ShaderProgram, 0, "vertexPosition");
	glBindAttribLocation(m_ShaderProgram, 1, "vertexTexCoords");
	glBindAttribLocation(m_ShaderProgram, 2, "vertexColour");

	return true;
}

void Material::bind()
{
	glUseProgram(m_ShaderProgram);
}

GLint Material::getUniformLocation(const std::string& name)
{
	return glGetUniformLocation(m_ShaderProgram, name.c_str());
}