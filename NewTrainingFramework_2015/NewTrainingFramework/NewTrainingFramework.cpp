// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ModelShader.h"
#include "Vertex_NFG.h"
#include "Shaders1.h"
#include <algorithm>
#include "Shaders.h"
#include "Camera.h"
#include "Vertex.h"
#include "Parser.h"
#include <conio.h>
#include <iomanip>
#include <numeric>
#include <vector>

std::pair<std::vector<Vertex_NFG>, std::vector<GLushort>> modelData;
GLuint vboId, vboId1, vboIdsModel[3];
std::vector<GLushort> wireframe;
ModelShader modelShader;
Shaders1 lineShaders;
bool isWired = false;
Shaders myShaders;
GLuint id_texture;
// Camera* camera;

int init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	ResourceManager::getInstance()->init();
	SceneManager::getInstance()->init();

	return 0;

	/*
	// Parse the document.
	modelData = Parser::parseFile("..\\Resources\\Packet\\Models\\Croco.nfg");

	glGenBuffers(3, vboIdsModel);

	glBindBuffer(GL_ARRAY_BUFFER, vboIdsModel[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_NFG) * modelData.first.size(), modelData.first.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdsModel[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * modelData.second.size(), modelData.second.data(), GL_STATIC_DRAW);

	wireframe = getWired(modelData.second);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdsModel[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * wireframe.size(), wireframe.data(), GL_STATIC_DRAW);

	// printContainer(modelData.first);
	// printContainer(modelData.second);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TEXTURE WORK
	int width, height, bpp;
	char* chr = LoadTGA("..\\Resources\\Packet\\Textures\\Croco.tga", &width, &height, &bpp);

	glGenTextures(1, &id_texture); // Reserve a buffer name called id_texture;
	glBindTexture(GL_TEXTURE_2D, id_texture); // Reserve buffer and bind it to that id;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Linear - blurr, Nearest - zoomed in pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Wrap ? GL_REPEAT it repeats.
	glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 24) ? GL_RGB : GL_RGBA, width, height, 0, (bpp == 24) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, chr);

	return modelShader.init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");
	*/

	/*
	// First triangle
	//triangle data (heap)
	Vertex verticesData[6];
	Vertex lineVerticesData[2];

	verticesData[0].pos.x = 0.5f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;

	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = 0.0f;
	verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;

	verticesData[2].pos.x = 0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;
	verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f;

	// Second triangle
	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = 0.5f;  verticesData[3].pos.z = 0.0f;
	verticesData[3].color.x = 1.0f;  verticesData[3].color.y = 0.0f;  verticesData[3].color.z = 0.0f;

	verticesData[4].pos.x = -0.5f;  verticesData[4].pos.y = -0.5f;  verticesData[4].pos.z = 0.0f;
	verticesData[4].color.x = 0.0f;  verticesData[4].color.y = 1.0f;  verticesData[4].color.z = 0.0f;

	verticesData[5].pos.x = -0.5f;  verticesData[5].pos.y = 0.5f;  verticesData[5].pos.z = 0.0f;
	verticesData[5].color.x = 1.0f;  verticesData[5].color.y = 1.0f;  verticesData[5].color.z = 0.0f;

	// Line
	lineVerticesData[0].pos.x = 0.0f; lineVerticesData[0].pos.y = -1.0f; lineVerticesData[0].pos.z = 0.0f;
	lineVerticesData[1].pos.x = 0.0f; lineVerticesData[1].pos.y = 1.0f; lineVerticesData[1].pos.z = 0.0f;

	//buffer object - for the two triangles
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//buffer1 object - for the white line
	glGenBuffers(1, &vboId1);
	glBindBuffer(GL_ARRAY_BUFFER, vboId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation of shaders and program
	return myShaders.init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs") |
		lineShaders.init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");

	*/
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SceneManager::getInstance()->draw();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
	return;
	/*
	// M1 S3 T4
	auto texture = ResourceManager::getInstance()->load<Texture>(4);
	auto shader = ResourceManager::getInstance()->load<Shader>(3);
	auto model = ResourceManager::getInstance()->load<Model>(1);
	glUseProgram(shader->getProgramId());

	glBindBuffer(GL_ARRAY_BUFFER, model->getVboId());

	if (!isWired)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIboId());
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredboId());

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), 0);
	}

	if (shader->unifMatrix != -1)
	{
		glUniformMatrix4fv(shader->unifMatrix, 1, GL_FALSE, (float*)(camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	// TEXTURE WORK
	glActiveTexture(GL_TEXTURE0); // Setat implicit pentru o textura.
	glBindTexture(texture->getTextureResource()->type, texture->getTextureId());

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), (void*)(5 * sizeof(Vector3)));
	}

	if (shader->textureUniform != -1)
	{
		glUniform1i(shader->textureUniform, 0); // Id = 0 deoarece este implicit o singura textura.
	}

	if (!isWired)
	{
		glDrawElements(GL_TRIANGLES, model->getNoInd(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_LINES, model->getNoIndWired(), GL_UNSIGNED_SHORT, 0);
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
	return;

	/*
	glUseProgram(modelShader.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboIdsModel[0]);
	if (!isWired)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdsModel[1]);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdsModel[2]);

	if (modelShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.positionAttribute);
		glVertexAttribPointer(modelShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), 0);
	}

	if (modelShader.unifMatrix != -1)
	{
		glUniformMatrix4fv(modelShader.unifMatrix, 1, GL_FALSE, (float*)(camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	// TEXTURE WORK
	glActiveTexture(GL_TEXTURE0); // Setat implicit pentru o textura.
	glBindTexture(GL_TEXTURE_2D, id_texture);

	if (modelShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.uvAttribute);
		glVertexAttribPointer(modelShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), (void*)(5 * sizeof(Vector3)));
	}

	if (modelShader.textureUniform != -1)
	{
		glUniform1i(modelShader.textureUniform, 0); // Id = 0 deoarece este implicit o singura textura.
	}

	if (!isWired)
	{
		glDrawElements(GL_TRIANGLES, modelData.second.size(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_LINES, wireframe.size(), GL_UNSIGNED_SHORT, 0);
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	return;
	*/

	/*glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if (myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	if (myShaders.uniformMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.uniformMatrix, 1, GL_FALSE, (float*)(camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// For the white line.
	glUseProgram(lineShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId1);

	if (lineShaders.positionAttribute != -1)
	{
		glVertexAttribPointer(lineShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(lineShaders.positionAttribute);
	}

	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
}

void Update(ESContext* esContext, float deltaTime)
{
	auto camera = SceneManager::getInstance()->getActiveCamera();
	camera->setDeltaTime(deltaTime);
	// Alegem o limita - constanta - globals
	// Timp global care este zero initial
	// Cand timpul global depaseste limita executam calculele. Scad limita cand = reset.
	// Camera trebuie sa stie timpul real, atunci nu vrem ca miscarea sa fie sacadata si punem in afara ifului.

	Globals::TIME += (Globals::TIME > Globals::FRAME_LIMIT) ? -Globals::FRAME_LIMIT : deltaTime;
	if (Globals::TIME <= Globals::FRAME_LIMIT)
	{
		Globals::alpha = (Globals::alpha > 2 * PI) ? 2 * PI - Globals::alpha : Globals::alpha + Globals::pas;
		// DO CALCULUS;
		// Load tga

		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			// 0x100 sau 0x80 pentru MOUSE
			POINT pct;
			GetCursorPos(&pct); //coord pe ecran
			ScreenToClient(esContext->hWnd, &pct); // pentru a obtine coord in fereastra
			if (pct.x < esContext->width / 2) {
				camera->rotateOz(1);
			}
			else {
				camera->rotateOz(-1);
			}
		}
	}
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	auto controls = SceneManager::getInstance()->getControls();

	if (bIsPressed && controls.find(key) != controls.end()) {
		auto camera = SceneManager::getInstance()->getActiveCamera();

		switch (controls[key])
		{
		case Controls::MOVE_CAMERA_POSITIVE_X:
			camera->moveOx(1);
			break;
		case Controls::MOVE_CAMERA_NEGATIVE_X:
			camera->moveOx(-1);
			break;
		case Controls::MOVE_CAMERA_POSITIVE_Y:
			camera->moveOy(-1);
			break;
		case Controls::MOVE_CAMERA_NEGATIVE_Y:
			camera->moveOy(1);
			break;
		case Controls::MOVE_CAMERA_POSITIVE_Z:
			camera->moveOz(-1);
			break;
		case Controls::MOVE_CAMERA_NEGATIVE_Z:
			camera->moveOz(1);
			break;
		case Controls::ROTATE_CAMERA_POSITIVE_X:
			camera->rotateOx(1);
			break;
		case Controls::ROTATE_CAMERA_NEGATIVE_X:
			camera->rotateOx(-1);
			break;
		case Controls::ROTATE_CAMERA_POSITIVE_Y:
			camera->rotateOy(-1);
			break;
		case Controls::ROTATE_CAMERA_NEGATIVE_Y:
			camera->rotateOy(1);
			break;
		case Controls::ROTATE_CAMERA_POSITIVE_Z:
			camera->rotateOz(-1);
			break;
		case Controls::ROTATE_CAMERA_NEGATIVE_Z:
			camera->rotateOz(1);
			break;
		default:
			break;
		}
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	/*
	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	*/
	// if (init(&esContext) != 0)
		// return 0;

	ResourceManager::getInstance()->init();
	SceneManager::getInstance()->init();

	glEnable(GL_DEPTH_TEST);

	esRegisterDrawFunc(&SceneManager::getInstance()->getESContext(), Draw);
	esRegisterUpdateFunc(&SceneManager::getInstance()->getESContext(), Update);
	esRegisterKeyFunc(&SceneManager::getInstance()->getESContext(), Key);

	esMainLoop(&SceneManager::getInstance()->getESContext());

	// Releasing OpenGL resources
	CleanUp();

	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();
	printf("Press any key...\n");
	_getch();
	return 0;
}