#ifndef ECS_H
#define ECS_H
#include <iostream>
#include <vector>
#include <memory>
#include <bitset>
#include <array>
#include <algorithm>
#include <iostream>
#include "../StarQuad.h"
#include "../StarUtils.h"
#include "../StariskExport.h"
#include <unordered_map>
#include "imgui/imgui.h"


class STARISK_API Component;
class STARISK_API Entity;
class STARISK_API StarECSManager;

using ComponentId = std::size_t;

// Defined once in ecs.cpp (compiled only into starisk_core.dll) so every
// DLL that calls this -- editor, core, and any loaded project plugin --
// shares the SAME typeMap/counter instead of each getting its own private
// copy. That sharing is what the getComponentTypeID<T>() comment below
// already assumed was happening; previously this was `inline` in the
// header, which meant it silently was NOT shared.
STARISK_API ComponentId getComponentTypeIDForName(const char* typeName);

template <typename T>
inline ComponentId getComponentTypeID()
{
    // static still caches per-DLL for performance,
    // but the value comes from the shared exported function
    static ComponentId typeId = getComponentTypeIDForName(typeid(T).name());
    return typeId;
}

constexpr std::size_t maxComponents = 64;

using ComponentArray = std::array<Component*, maxComponents>;

using ComponentBitSet = std::bitset<maxComponents>;


class STARISK_API Component
{

public:
    Entity* entity;


    Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(Component&&) = default;
    Component& operator=(Component&&) = default;

    virtual void init(){};
    virtual void update(){};
    virtual void draw(){};
    
    virtual ~Component(){};
};


class STARISK_API Entity
{
private:
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    std::string id;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    
public:
    StarQuad quad;
    

  
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    
    ///Entity(Entity&&) = default;
    //Entity& operator=(Entity&&) = default;


    Entity(){ id = generateUUID(); componentArray.fill(nullptr); }
    
    ~Entity(){ destroy(); }

    std::string getId()
    {
        return id;
    }

    void destroy(){ active = false;}

    bool isActive() const 
    {
        return active;
    }

    void update()
    {
        for(auto& c : components) c->update();
    }

    void draw()
    {
        for(auto& c:components) c->draw();
    }


    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T& getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        assert(ptr != nullptr && "getComponent<T>() called on entity without that component");
        return  *static_cast<T*>(ptr);
    }


};

class STARISK_API System
{
public:
    


    System(){}
    
    virtual ~System(){}

    virtual void init(){}
    virtual void update(StarECSManager* sem, StarBatchManager* sbm){}



};


class STARISK_API StarECSManager
{
public:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<System>> systems;
    StarBatchManager* sbm;
    size_t entityCount = 0;

    StarECSManager()
    {

    }

    ~StarECSManager()
    {

    }

    StarECSManager(const StarECSManager&) = delete;
    StarECSManager& operator=(const StarECSManager&) = delete;

    StarECSManager(StarECSManager&&) noexcept = default;
    StarECSManager& operator=(StarECSManager&&) noexcept = default;


    
    void update(StarBatchManager* sbm)
    {
        for(auto& e : entities) e->update();
        for(auto& s : systems) s->update(this, sbm);
    }
    void draw()
    {
        for(auto& e : entities) e->draw();
    }

    void refresh()
    {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities), [this](const std::unique_ptr<Entity> &mEntity)
    {
        return !mEntity->isActive();
    }), std::end(entities));
        entityCount--;

        //if(entityCount != entities.size()) entityCount = entities.size();
    }

    template <typename T, typename... Targs>
    T& addSystem(Targs&&... mArgs)
    {
        T* s(new T(std::forward<Targs>(mArgs)...));
        std::unique_ptr<System> uPtr{s};
        systems.emplace_back(std::move(uPtr));
        return *s;
    }
    template <typename T>
    T& addSystem(std::unique_ptr<System>& s)
    {
        systems.emplace_back(std::move(s));
        auto& ret = systems.back();
        return ret;
    }

    Entity& addEntity()
    {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        entityCount++;
        return *e;
    }

};

#endif