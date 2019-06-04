#include "ServiceLocator.h"

std::unordered_map<std::string, Service*> ServiceLocator::services;

ServiceLocator::ServiceLocator()
{

}

ServiceLocator::~ServiceLocator()
{
}

void ServiceLocator::provideService(Service* pService) {
	assert(pService != nullptr, "Provided object is a null pointer!");

	const std::type_index* typeIndex = &std::type_index(typeid(*pService));
	services.insert(std::pair<std::string, Service*>(typeIndex->name(), pService));
};