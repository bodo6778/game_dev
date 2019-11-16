#include "stdafx.h"
#include "Shaders.h"
#include <iostream>

int Shaders::init(char* fileVertexShader, char* fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if (vertexShader == 0)
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	if (program == 0)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return -3;
	}

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	uniformMatrix = glGetUniformLocation(program, "u_matrix");
	colorAttribute = glGetAttribLocation(program, "a_color");

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}