#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
class Texture {
public:
	Texture();
	virtual ~Texture();

	static Texture* load(const std::string& pTexturePath);
	unsigned int getId();

protected:
	static Texture* _loadFromFile(const std::string pTexturePath);

	unsigned int _id;
};

#endif // TEXTURE_HPP
