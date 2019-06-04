#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <typeindex>
#include <memory>
#include <unordered_map>
#include "SFML/Graphics.hpp"

class World;
class AbstractBehaviour;

class GameObject
{
public:
	//--------------------------------------------------------General
	const std::string & name();
	const std::string & type();

	GameObject(const std::string& pName, const std::string& pTextureFilename, const sf::Vector2f& pPosition = sf::Vector2f(0.0f, 0.0f));
	GameObject(const std::string& pName, const sf::Vector2f& pPosition = sf::Vector2f(0.0f, 0.0f));
	virtual ~GameObject();
	void setName(const std::string& pName);
	std::string getName() const;

	virtual void fixedUpdate(float pStep);
	virtual void variableUpdate(float pStep);
	virtual void draw(sf::RenderTarget& pTarget, const sf::Transform& pParentTransform) const;

	//--------------------------------------------------------Transform
	//contains local rotation, scale, position
	const sf::Transform& getTransform() const;

	//access just the local position
	void setLocalPosition(sf::Vector2f pPosition);
	sf::Vector2f getLocalPosition() const;

	//access the local rotation in degrees
	void setLocalRotation (float pRotation);
	float getLocalRotation() const;

	//access the local scale
	void setLocalScale(sf::Vector2f pScale);
	sf::Vector2f getLocalScale() const;

	//get the objects world position by combining transforms, SLOW use with care
	//sf::Vector2f getWorldPosition() const;
	//sf::Transform getWorldTransform() const;

	//change LOCAL position, rotation, scaling
	void translate(sf::Vector2f pTranslation);
	void rotate(float pAngle);
	void scale(float pFactor);

	//---------------------------------------------------------Elements
	void setSprite(sf::Sprite pSprite);
	sf::Sprite getSprite() const;

	void setTexture(sf::Texture pTexture);
	sf::Texture getTexture() const;

	void loadnSetSprite(std::string pFileName);

	void setSpriteOrigin(sf::Vector2f pNewOrigin);
	void setSpriteColor(sf::Color pColor);
	sf::Vector2f getSpriteCenter() const;

	//Collider* createCollider(ColliderTypee pType, glm::vec3 pSize = glm::vec3(0, 0, 0));
	//Collider* getCollider();

	//----------------------------------------------------------Behaviours

	//behaviour is expected to be unique per game object, in general do NOT share them between objects
	void addBehaviour(AbstractBehaviour* pBehaviour);

	template<class T>
	T * getBehaviour() {
		return static_cast<T *>(_behaviours.at(typeid(T).name()));	// Consider using weak or shared pointers, or & instead of *
	}

	int getBehaviourCount();

	//---------------------------------------------------------Child Management

	//child management, note that add/remove and setParent are closely coupled.
	//a.add(b) has the same effect as b.setParent(a)
	//Adding a gameobject or resetting the parent, recursively passes on the world pointer to all child objects

	//shortcut to set the parent of pChild to ourselves
	void add(GameObject* pChild);
	//shortcut to set the parent of pChild to nullptr
	void remove(GameObject* pChild);

	void setParent(GameObject* pGameObject);
	GameObject* getParent() const;
	void setAsFirstChild(GameObject* pChild);

	GameObject* getWorldObject() const;
	World* getWorld() const;

	int getChildCount() const;
	GameObject* getChildAt(int pIndex) const;

	GameObject(const GameObject&);

protected:
	std::string _name;
	sf::Transformable _transformable;
	sf::Sprite _sprite;
	sf::Texture _texture;
	World* _world;

	GameObject* _parent;
	std::vector<GameObject*> _children;

	//Mesh* _mesh;
	//AbstractMaterial* _material;
	std::unordered_map<std::string, AbstractBehaviour*> _behaviours;

	//Collider* _collider = nullptr;
	void updateTransform();

	//update children list administration
	void _innerAdd(GameObject* pChild);
	void _innerRemove(GameObject* pChild);

	//used to pass on pointer to the world to a gameobject
	virtual void _setWorldRecursively(World* pWorld);
private:
	GameObject& operator= (const GameObject&);
	std::string _type;
	virtual void onDraw(sf::RenderTarget& pTarget, const sf::Transform& pTransform) const;
};



#endif // !GAMEOBJECT_HPP