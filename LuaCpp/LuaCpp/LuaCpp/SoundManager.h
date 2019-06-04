#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "SFML\Audio.hpp"
#include <unordered_map>
#include <vector>
#include <array>
#include "ServiceLocator.h"
#include "lua.hpp"

class SoundPair {
public:

	SoundPair();
	SoundPair(sf::SoundBuffer& pBuffer, sf::Sound* pSound);

	sf::SoundBuffer buffer;
	sf::Sound sound;
};


class SoundManager : public Service
{
public:
	SoundManager();
	virtual ~SoundManager();

	sf::SoundBuffer& loadSound(const std::string& pFilename);
	sf::Sound& playSound(const std::string& pFilename);
	sf::Music& playMusic(const std::string& pFilename);
	//sf::Music& getCurrentMusic() const;

	void setMusicVolume(int volume);
	void stopMusic();
	void setMusicLoop(bool value);

	void deleteSound(std::string& pFilename);
	void clearBufferCache();

	//static SoundManager& instance();
	static SoundManager* instance;

	static void registerLib(lua_State* lua);
private:

	sf::Music _music;
	sf::SoundBuffer _buffer;
	std::unordered_map<std::string, SoundPair*> _sounds;
};

#endif // !MUSIC_MANAGER_H

