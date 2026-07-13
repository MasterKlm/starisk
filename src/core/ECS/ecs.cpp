#include "ecs.h"
#include <string>
#include <unordered_map>

ComponentId getComponentTypeIDForName(const char* typeName)
{
    static std::unordered_map<std::string, ComponentId> typeMap;
    static ComponentId counter = 0;

    auto key = std::string(typeName);
    auto it = typeMap.find(key);
    if (it != typeMap.end())
        return it->second;

    ComponentId newId = counter++;
    typeMap[key] = newId;
    return newId;
}