#pragma once
#include "TextureResource.h"
#include "Loadable.h"
#include <memory>

class Texture : public Loadable
{
	// Texture Resource Config
	std::shared_ptr<TextureResource> tr;

	// TextureId generated by glGenTextures()
	GLuint textureId;

	// Flag for resources
	GLboolean holdsResources;

public:
	// Constructor
	Texture(std::shared_ptr<TextureResource> tr = nullptr);

	// Initializer
	Texture& init(std::shared_ptr<TextureResource> tr);

	// Destructor
	virtual ~Texture();

	// Getters and setters
	GLuint getTextureId() const;
	void setTextureId(GLuint id);

	std::shared_ptr<TextureResource> getTextureResource();
	void setTextureResource(std::shared_ptr<TextureResource> textureResource);

	// Inherited via Loadable
	virtual void load() override;

	// Inherited via Loadable
	virtual void freeResources() override;
};