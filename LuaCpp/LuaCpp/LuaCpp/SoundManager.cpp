#include "SoundManager.h"
#include <iostream>
#include "Config.h"

SoundManager* SoundManager::instance;
SoundManager::SoundManager()
{
	instance = this;
}

SoundManager::~SoundManager()
{
}

SoundPair::SoundPair() {

}

SoundPair::SoundPair(sf::SoundBuffer& pBuffer, sf::Sound* pSound) {
	buffer = pBuffer;
	sound = *pSound;
}

sf::SoundBuffer& SoundManager::loadSound(const std::string& pFilename) {
	if (_buffer.loadFromFile(config::SOUND_PATH + pFilename)) {

		sf::SoundBuffer* buff = new sf::SoundBuffer(_buffer);
		SoundPair* soundPair = new SoundPair(*buff, new sf::Sound(*buff));
		_sounds.emplace(config::SOUND_PATH + pFilename, soundPair);

		return *buff;
	}

	return _buffer;
}

sf::Sound& SoundManager::playSound(const std::string& pFilename) {

	if (_sounds[config::SOUND_PATH + pFilename]->sound.getStatus() == sf::Sound::Status::Stopped)
		_sounds[config::SOUND_PATH + pFilename]->sound.play();

	return _sounds[config::SOUND_PATH + pFilename]->sound;
}

sf::Music& SoundManager::playMusic(const std::string& pFilename) {
	if (_music.openFromFile(config::SOUND_PATH + pFilename)) {
		_music.play();
		_music.setLoop(true);
		return _music;
	}
}

void SoundManager::setMusicVolume(int volume) {
	_music.setVolume(volume);
}

void SoundManager::stopMusic() {
	_music.stop();
}

void SoundManager::setMusicLoop(bool value) {
	_music.setLoop(value);
}

void SoundManager::deleteSound(std::string& pFilename) {
	_sounds[pFilename];
}

void SoundManager::clearBufferCache() {
	//std::unordered_map< std::string&, sf::SoundBuffer&>::iterator it = _sounds.begin();

	//while (it != _sounds.end())
	//{
	//	delete (*it).second;

	//	it = _sounds.erase(it);
	//}
}

int loadSound(lua_State* lua) {
	std::string sound = luaL_checkstring(lua, -1);
	SoundManager::instance->loadSound(sound);
	lua_pushstring(lua, sound.c_str());
	return 1;
}

int playSound(lua_State* lua) {
	SoundManager::instance->playSound(luaL_checkstring(lua, -1));
	return 0;
}

int playMusic(lua_State* lua) {
	SoundManager::instance->playMusic(luaL_checkstring(lua, -1));
	return 0;
}

int stopMusic(lua_State* lua) {
	SoundManager::instance->stopMusic();
	return 0;
}

int setMusicLoop(lua_State* lua) {
	SoundManager::instance->setMusicLoop(lua_toboolean(lua, -1));
	return 0;
}

int setMusicVolume(lua_State* lua) {
	SoundManager::instance->setMusicVolume(luaL_checkinteger(lua, -1));
	return 0;
}

static const struct luaL_Reg soundLib[] = {
	{"loadSound", loadSound},
	{"playSound", playSound},
	{"playMusic", playMusic},
	{"stopMusic", stopMusic},
	{"setMusicVolume", setMusicVolume},
	{"setMusicLoop", setMusicLoop},
	{NULL, NULL} // Signals the end of the registry
};

void SoundManager::registerLib(lua_State* lua) {
	luaL_newlib(lua, soundLib);
	lua_setglobal(lua, "Sound");
}