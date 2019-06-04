#include <iostream>
#include "GameObject.h"
#include "AbstractBehaviour.h"


GameObject::GameObject(const std::string& pName, const sf::Vector2f& pPosition)
	: _name(pName), _parent(nullptr), _children(), _world(nullptr)
{
	_transformable.setPosition(pPosition);
}

GameObject::GameObject(const std::string& pName, const std::string& pTextureFilename, const sf::Vector2f& pPosition)
	: _name(pName), _parent(nullptr), _children(), _world(nullptr)
{
	_transformable.setPosition(pPosition);
	if(!pTextureFilename.empty()) loadnSetSprite(pTextureFilename);
}

GameObject::~GameObject()
{
	//detach all children
	std::cout << "GC running on:" << _name << std::endl;

	while (_children.size() > 0) {
		GameObject* child = _children[0];
		remove(child);
		delete child;
	}

	for (auto &mapValue : _behaviours) {
		delete mapValue.second;
	}

	setParent(nullptr);

	//do not forget to delete behaviour, sprite, texture, collider manually if required!
}

void GameObject::setName(const std::string& pName)
{
	_name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

//-----------------------------------------------------Transform

const sf::Transform& GameObject::getTransform() const
{
	return _transformable.getTransform();
}

void GameObject::setLocalPosition(sf::Vector2f pPosition)
{
	_transformable.setPosition(pPosition);
}

sf::Vector2f GameObject::getLocalPosition() const
{
	return _transformable.getPosition();
}

void GameObject::setLocalRotation(float pAngle) 
{
	_transformable.setRotation(pAngle);
}

float GameObject::getLocalRotation() const 
{
	return _transformable.getRotation();
}

void GameObject::setLocalScale(sf::Vector2f pScale) 
{
	_transformable.setScale(pScale);
}

sf::Vector2f GameObject::getLocalScale() const 
{
	return _transformable.getScale();
}

//-----------------------------------------------------Object manipulation

void GameObject::translate(sf::Vector2f pTranslation)
{
	_transformable.move(pTranslation);
}

void GameObject::scale(float pFactor)
{
	_transformable.scale(pFactor, pFactor);
}

void GameObject::rotate(float pAngle)
{
	_transformable.rotate(pAngle);
}

//-----------------------------------------------------Sprite and Texture

void GameObject::loadnSetSprite(std::string pFileName)
{
	if (_texture.loadFromFile(pFileName)) {
		if (_sprite.getTexture() != nullptr) {
			_sprite.setTexture(_texture, true);
		}
		else _sprite = sf::Sprite(_texture);
	}
}

void GameObject::setSprite(sf::Sprite pSprite) 
{
	_sprite = pSprite;
}

sf::Sprite GameObject::getSprite() const
{
	return _sprite;
}

void GameObject::setTexture(sf::Texture pTexture) 
{
	_texture = pTexture;
}

sf::Texture GameObject::getTexture() const 
{
	return _texture;
}

void GameObject::setSpriteOrigin(sf::Vector2f pNewOrigin) 
{
	_transformable.setOrigin(pNewOrigin);
	_sprite.setOrigin(pNewOrigin);
}

void GameObject::setSpriteColor(sf::Color pColor) 
{
	_sprite.setColor(pColor);
}

sf::Vector2f GameObject::getSpriteCenter() const 
{
	if(_sprite.getTexture() != nullptr)
		return sf::Vector2f(_sprite.getTextureRect().width / 2, _sprite.getTextureRect().height / 2);
	else return sf::Vector2f(0, 0);
}

//-----------------------------------------------------Behaviour

void GameObject::addBehaviour(AbstractBehaviour* pBehaviour)
{
	const std::type_index* typeIndex = &std::type_index(typeid(*pBehaviour));
	_behaviours.insert(std::pair<std::string, AbstractBehaviour*>(typeIndex->name(), pBehaviour));
	pBehaviour->setOwner(this);
}

int GameObject::getBehaviourCount()
{
	return _behaviours.size();
}

//--------------------------------------------------------Collider
//
//Collider* GameObject::createCollider(ColliderTypee pType, glm::vec3 pSize) {
//	if (pSize == glm::vec3(0, 0, 0)) pSize = getMesh()->size(getMesh());
//
//	_collider = new Collider(this, pType, getLocalPosition(), pSize);
//	return _collider;
//}
//
//Collider* GameObject::getCollider() {
//	return _collider;
//}

//--------------------------------------------------------Parenting

void GameObject::setParent(GameObject* pParent) {
	//remove from previous parent
	if (_parent != nullptr) {
		_parent->_innerRemove(this);
	}

	//set new parent
	if (pParent != nullptr) {
		pParent->_innerAdd(this);
	}

	//if we have been detached from our parent, make sure
	//the world reference for us and all our children is set to null
	//if we have been attached to a parent, make sure
	//the world reference for us and all our children is set to our parent world reference
	//(this could still be null if the parent or parent's parent is not attached to the world)
	if (_parent == nullptr) {
		_setWorldRecursively(nullptr);
	}
	else {
		//might still not be available if our parent is not part of the world
		_setWorldRecursively(_parent->_world);
	}
}

void GameObject::_innerRemove(GameObject* pChild) {
	for (auto i = _children.begin(); i != _children.end(); ++i) {
		if (*i == pChild) {
			_children.erase(i);
			pChild->_parent = nullptr;
			return;
		}
	}
}

void GameObject::_innerAdd(GameObject* pChild)
{
	_children.push_back(pChild);
	pChild->_parent = this;
}

void GameObject::add(GameObject* pChild) {
	pChild->setParent(this);
}

void GameObject::remove(GameObject* pChild) {
	pChild->setParent(nullptr);
}

GameObject* GameObject::getParent() const {
	return _parent;
}

void GameObject::setAsFirstChild(GameObject* pChild) {
	
	auto pivot = std::find(_children.begin(), _children.end(), pChild);
	if (pivot != _children.end()) {
		std::rotate(_children.begin(), pivot, pivot + 1);
	}
}

GameObject* GameObject::getWorldObject() const {
	return (GameObject*)_world;
}

World* GameObject::getWorld() const {
	return _world;
}

//--------------------------------------------------------Update

void GameObject::fixedUpdate(float pStep)
{
	for (int i = _children.size() - 1; i >= 0; --i) {
		_children[i]->fixedUpdate(pStep);
	}
}

void GameObject::variableUpdate(float pStep)
{
	updateTransform();

	//make sure behaviours are updated after worldtransform is set
	for (auto &mapValue : _behaviours) {
		if (mapValue.second) {
			mapValue.second->variableUpdate(pStep);
		}
	}

	for (int i = _children.size() - 1; i >= 0; --i) {
		_children[i]->variableUpdate(pStep);
	}
}

void GameObject::updateTransform() {
	if (this->getParent() != NULL)
	{
		// I be damned if I can make the child follow the parent correctly, so for now they move independent
		this->setLocalPosition(this->getLocalPosition());
		this->_sprite.setPosition(this->getLocalPosition());

		this->setLocalRotation(this->getLocalRotation());
		this->_sprite.setRotation(this->getLocalRotation());

		this->setLocalScale(this->getLocalScale());
		this->_sprite.setScale(this->getLocalScale());
		//std::cout << "BITCONEEEEEEEEEEEEEEEEECT" << _parent->getWorldPosition() << std::endl;
	} 
}
//----------------------------------------------------------Draw

void GameObject::draw(sf::RenderTarget& pTarget, const sf::Transform& pParentTransform) const 
{
	// Combine the parent transform with the node's one
	sf::Transform combinedTransform = pParentTransform * _transformable.getTransform();

	// Let the node draw itself
	if(_sprite.getTexture() != nullptr) onDraw(pTarget, combinedTransform);

	// Draw the children
	for (std::size_t i = 0; i < _children.size(); ++i) {
		_children[i]->draw(pTarget, combinedTransform);
	}
}

void GameObject::onDraw(sf::RenderTarget & pTarget, const sf::Transform & pTransform) const
{
	pTarget.draw(_sprite);
}


void GameObject::_setWorldRecursively(World* pWorld) {
	_world = pWorld;

	for (int i = _children.size() - 1; i >= 0; --i) {
		_children[i]->_setWorldRecursively(pWorld);
	}
}

int GameObject::getChildCount() const {
	return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) const {
	return _children[pIndex];
}

GameObject::GameObject(const GameObject &)
{
}

const std::string & GameObject::name()
{
	return _name;
}

const std::string & GameObject::type()
{
	return _type;
}