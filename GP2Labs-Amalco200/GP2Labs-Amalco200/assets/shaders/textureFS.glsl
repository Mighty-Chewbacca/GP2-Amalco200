#version 150

in vec2 vertexTexCoordsOut;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
	FragColor = texture(texture0, vertexTexCoordsOut);
}