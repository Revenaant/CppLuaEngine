#ifndef SERVICELOCATOR_HPP
#define SERVICELOCATOR_HPP

#include <unordered_map>
#include <typeindex>
#include <assert.h>

class Service {
protected:
	virtual ~Service() {};
};

class ServiceLocator
{
public:
	ServiceLocator();
	~ServiceLocator();

	static std::unordered_map<std::string, Service*> services;

	static void provideService(Service* pService);

	template<class T> 
	static T* getService() {
		static_assert(std::is_convertible_v<T, Service>, "Provided object is not derived from 'Service'!");
		return static_cast<T *>(services.at(typeid(T).name()));
	}

	template<class T>
	static void removeService() {
		static_assert(std::is_convertible_v<T, Service>, "Provided object is not derived from 'Service'!");
		services.erase(typeid(T).name());
	};

};

#endif // !SERVICELOCATOR_HPP
