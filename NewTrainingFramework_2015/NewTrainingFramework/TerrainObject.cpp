#include "stdafx.h"
#include <algorithm>
#include "Vertex_NFG.h"
#include "SceneManager.h"
#include "TerrainObject.h"

TerrainObject::TerrainObject(GLint id)
	:TerrainObject{ id, Type::TERRAIN } {}

TerrainObject::TerrainObject(GLint id, Type type)
	: SceneObject{ id, type }, cellSize{ 0 }, sideCells{ 0 }, center{}, offsetY{ 0 }, dx{ 0 }, dz{ 0 } {}

void TerrainObject::generateModel()
{
	model = std::make_shared<Model>();

	GLint N = sideCells + 1;
	GLfloat x = center.x - sideCells * cellSize / (GLfloat)2;
	GLfloat z = center.z - sideCells * cellSize / (GLfloat)2;

	for (GLint i = 0; i <= sideCells; ++i)
	{
		for (GLint j = 0; j <= sideCells; ++j)
		{
			Vertex_NFG vex;
			vex.pos.x = x + j * cellSize;
			vex.pos.y = offsetY;
			vex.pos.z = z + i * cellSize;

			vex.uv.x = GLfloat(j);
			vex.uv.y = GLfloat(i);

			vex.uv2.x = j / (GLfloat)sideCells;
			vex.uv2.y = i / (GLfloat)sideCells;

			vertices.push_back(vex);

			if (j > 0 && i < sideCells)
			{
				//   1
				// 3 2
				indexes.push_back(i * N + j);
				indexes.push_back((i + 1) * N + j);
				indexes.push_back((i + 1) * N + j - 1);
			}

			if (j < sideCells && i < sideCells)
			{
				// 1 2
				// 3
				indexes.push_back(i * N + j);
				indexes.push_back(i * N + j + 1);
				indexes.push_back((i + 1) * N + j);
			}
		}
	}

	model->load<Vertex_NFG>(vertices, indexes);
}

void TerrainObject::setSideCells(GLint sideCells)
{
	if (sideCells <= 0 || sideCells % 2 != 0)
	{
		throw std::runtime_error{ "Cell-count must be even and greater than 0." };
	}
	this->sideCells = sideCells;
}

void TerrainObject::draw()
{
	glUseProgram(shader->getProgramId());

	glBindBuffer(GL_ARRAY_BUFFER, model->getVboId());

	if (!wiredFormat)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIboId());
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredboId());

	// Get field attributes and uniforms
	Fields fields = shader->getFields();

	if (fields.positionAttribute != -1)
	{
		glEnableVertexAttribArray(fields.positionAttribute);
		glVertexAttribPointer(fields.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), 0);
	}

	if (fields.unifMatrix != -1)
	{
		auto camera = SceneManager::getInstance()->getActiveCamera();
		glUniformMatrix4fv(fields.unifMatrix, 1, GL_FALSE, (float*)(getModelMatrix() * camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	// Atentie ! colorbind are dimensiune 4 iar MAX_TEXTURES 5, daca se mai adauga o textura arunca exceptie.
	for (GLuint index = 0; index < std::min<GLuint>(Fields::MAX_TEXTURES, textures.size()); ++index)
	{
		glActiveTexture(index + GL_TEXTURE0);

		glBindTexture(textures[index]->getTextureResource()->type, textures[colorBind[index]]->getTextureId());

		if (fields.textureUniform[index] != -1)
		{
			glUniform1i(fields.textureUniform[index], index);
		}
	}

	if (fields.uvAttribute != -1)
	{
		glEnableVertexAttribArray(fields.uvAttribute);
		glVertexAttribPointer(fields.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), (void*)(5 * sizeof(Vector3)));
	}

	if (fields.uv2Attribute != -1)
	{
		glEnableVertexAttribArray(fields.uv2Attribute);
		glVertexAttribPointer(fields.uv2Attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), (void*)(5 * sizeof(Vector3) + sizeof(Vector2)));
	}

	if (fields.heightUniform != -1)
	{
		glUniform3f(fields.heightUniform, height.x, height.y, height.z);
	}

	if (fields.colorAttribute != -1)
	{
		glVertexAttrib3f(fields.colorAttribute, color.x, color.y, color.z);
	}

	if (!wiredFormat)
	{
		glDrawElements(GL_TRIANGLES, model->getNoInd(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_LINES, model->getNoIndWired(), GL_UNSIGNED_SHORT, 0);
	}
}

template<typename Fun>
inline void TerrainObject::move(GLfloat& diff, const GLfloat& cpos, GLfloat& pos, Fun fun)
{
	diff = cpos - pos;

	if (std::abs(diff) > cellSize)
	{
		if (diff > 0)
		{
			pos += cellSize;
			diff -= cellSize;
			std::for_each(vertices.begin(), vertices.end(), 
				[&](auto& vertex) { fun(vertex, 1); });
		}
		else
		{
			pos -= cellSize;
			diff += cellSize;
			std::for_each(vertices.begin(), vertices.end(),
				[&](auto& vertex) { fun(vertex, -1); });
		}

		modified = true;
	}
}

void TerrainObject::update()
{
	auto cameraPos = SceneManager::getInstance()->getActiveCamera()->getPosition();

	move(dx, cameraPos.x, position.x,
		[&](Vertex_NFG& vertex, GLint sign) { vertex.uv2.x += sign / (GLfloat) sideCells; });

	move(dz, cameraPos.z, position.z,
		[&](Vertex_NFG& vertex, GLint sign) { vertex.uv2.y += sign / (GLfloat) sideCells; });

	// TODO; model->freeResources();
	model->load<Vertex_NFG>(vertices, indexes);
}

Matrix& TerrainObject::getModelMatrix()
{
	// Lazy updates
	if (modified) {
		auto tran = modelMatrix.SetScale(scale);
		tran = tran * modelMatrix.SetRotationX(rotation.x);
		tran = tran * modelMatrix.SetRotationY(rotation.y);
		tran = tran * modelMatrix.SetRotationZ(rotation.z);
		auto heightOffset = std::max<GLfloat>(height.x, std::max<GLfloat>(height.y, height.z));
		tran = tran * modelMatrix.SetTranslation(position.x, position.y - heightOffset - 1, position.z);
		modelMatrix = tran;
		modified = false;
	}
	return modelMatrix;
}

TerrainObject::~TerrainObject()
{
	// TODO;
}

GLint TerrainObject::getSideCells() const
{
	return sideCells;
}

void TerrainObject::setCellSize(GLfloat cellSize)
{
	if (cellSize <= 0)
	{
		throw std::runtime_error{ "Cell size must be greater than 0." };
	}
	this->cellSize = cellSize;
}

GLfloat TerrainObject::getCellSize() const
{
	return cellSize;
}

void TerrainObject::setOffsetY(GLfloat offsetY)
{
	this->offsetY = offsetY;
}

GLfloat TerrainObject::getOffsetY() const
{
	return offsetY;
}

void TerrainObject::setCenter(Vector3& center)
{
	this->center = center;
}

Vector3& TerrainObject::getCenter()
{
	return center;
}

void TerrainObject::setHeight(Vector3& height)
{
	this->height = height;
}

Vector3& TerrainObject::getHeight()
{
	return height;
}

void TerrainObject::setColorBind(GLuint r, GLuint g, GLuint b, GLuint blend)
{
	bool hasR{ false }, hasG{ hasR }, hasB{ hasG }, hasBlend{ hasB };
	for (GLuint i = 0; i < textures.size(); ++i)
	{
		if (!hasR && r == textures[i]->getTextureResource()->id)
		{
			hasR = true;
			this->colorBind[0] = i;
		}

		if (!hasG && g == textures[i]->getTextureResource()->id)
		{
			hasG = true;
			this->colorBind[1] = i;
		}

		if (!hasB && b == textures[i]->getTextureResource()->id)
		{
			hasB = true;
			this->colorBind[2] = i;
		}

		if (!hasBlend && blend == textures[i]->getTextureResource()->id)
		{
			hasBlend = true;
			this->colorBind[3] = i;
		}
	}

	if (!hasR || !hasG || !hasB || !hasBlend)
	{
		throw std::runtime_error{ "Terrain Object doesn't have the colorBind ids in textures vector." };
	}
}

const GLuint* TerrainObject::getColorBind() const
{
	return colorBind;
}
