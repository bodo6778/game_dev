#include "stdafx.h"
#include "Shader.h"
#include <iostream>

Shader::Shader(std::shared_ptr<ShaderResource> sr)
	:programId{}, sr{ sr }, fields{} {}

Shader& Shader::init(std::shared_ptr<ShaderResource> sr)
{
	freeResources();
	this->sr = sr;
	return *this;
}

void Shader::load()
{
	GLuint vertexShader = esLoadShader(GL_VERTEX_SHADER, const_cast<char*>(sr->vsShader.c_str()));

	if (vertexShader == 0) {
		throw std::runtime_error{ "Failed to compile vertex shader." };
	}

	GLuint fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, const_cast<char*>(sr->fsShader.c_str()));

	if (fragmentShader == 0) {
		throw std::runtime_error{ "Failed to compile fragment shader." };
	}

	programId = esLoadProgram(vertexShader, fragmentShader);

	if (programId == 0) {
		throw std::runtime_error{ "Failed to link program." };
	}

	// Attributes
	fields.binormAttribute = glGetAttribLocation(programId, "a_binormL");
	fields.positionAttribute = glGetAttribLocation(programId, "a_posL");
	fields.colorAttribute = glGetAttribLocation(programId, "a_colorL");
	fields.normAttribute = glGetAttribLocation(programId, "a_normL");
	fields.tgtAttribute = glGetAttribLocation(programId, "a_tgtL");
	fields.uv2Attribute = glGetAttribLocation(programId, "a_uv2L");
	fields.uvAttribute = glGetAttribLocation(programId, "a_uvL");

	// Uniforms
	fields.isReflectedUniform = glGetUniformLocation(programId, "u_is_reflected");
	fields.skyboxUniform = glGetUniformLocation(programId, "u_texture_skybox");
	fields.projectionUniform = glGetUniformLocation(programId, "u_projection");
	fields.fogTransitionUniform = glGetUniformLocation(programId, "u_fog_R");
	fields.fogColorUniform = glGetUniformLocation(programId, "u_fog_color");
	fields.fogClarityUniform = glGetUniformLocation(programId, "u_fog_r");
	fields.dispMaxUniform = glGetUniformLocation(programId, "u_disp_max");
	fields.heightUniform = glGetUniformLocation(programId, "u_height");
	fields.cameraUniform = glGetUniformLocation(programId, "u_camera");
	fields.modelUniform = glGetUniformLocation(programId, "u_model");
	fields.viewUniform = glGetUniformLocation(programId, "u_view");
	fields.timeUniform = glGetUniformLocation(programId, "u_time");

	for (GLuint i = 0; i < Fields::MAX_TEXTURES; ++i)
	{
		fields.textureUniform[i] = glGetUniformLocation(programId, ("u_texture_" + std::to_string(i)).c_str());
	}

	holdsResources = true;
}

void Shader::freeResources()
{
	if (holdsResources) {
		// Current
		GLint maxShaders;
		glGetProgramiv(programId, GL_ATTACHED_SHADERS, &maxShaders);

		// Fetch current shaders
		GLsizei curentShaders; GLuint* shaders = new GLuint[maxShaders];
		glGetAttachedShaders(programId, maxShaders, &curentShaders, shaders);

		// Delete current shaders
		for (GLsizei i{ 0 }; i < curentShaders; ++i) {
			glDeleteShader(shaders[i]);
		}

		// Free array of indexes.
		delete[] shaders;

		// Delete current program
		glDeleteProgram(programId);

		holdsResources = false;
	}
}

Shader::~Shader()
{
	freeResources();
}

GLuint Shader::getProgramId() const
{
	return programId;
}

void Shader::setProgramId(GLuint programId)
{
	this->programId = programId;
}

Fields Shader::getFields() const
{
	return fields;
}

std::shared_ptr<ShaderResource> Shader::getShaderResource()
{
	return sr;
}

void Shader::setShaderResource(std::shared_ptr<ShaderResource> shaderResource)
{
	sr = shaderResource;
}