#include <SFML/Graphics.hpp>
#include "Texture.h"

Texture::Texture(): _id() { }
Texture::~Texture() {}

unsigned int Texture::getId() {
	return _id;
}

// Importer for textures
Texture* Texture::load(const std::string& pFilename) 
{
	// Load from file and store in cache
	sf::Image image;
	if (image.loadFromFile(pFilename)) {
		Texture* texture = new Texture();
		return texture;
	}
	else return nullptr;
}

// Importer for textures
//sf::Image* Texture::loadImage(const std::string& pFilename)
//{
//	// Load from file and store in cache
//	sf::Image image;
//	if (image.loadFromFile(pFilename)) {
//		return &image;
//	}
//	else return nullptr;
//}