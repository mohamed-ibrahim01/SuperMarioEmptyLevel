#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <iostream>
#include <array>
#include <queue>
#include <memory>
#include <bitset>
#include <unordered_map>
#include "Types.h"
#include "System.h"
#include "ComponentArray.h"


class GameEngine
{
public:
    GameEngine() = default;

    void Init()
    {
        // populate available entities
        for (int i = 0; i < MAX_ENTITIES; ++i)
        {
            mAvialableEntities.push(i);
        }
    }
    
    int CreateEntity()
    {
        // get entity from available entities
        int entity = mAvialableEntities.front();
        mAvialableEntities.pop();
        
        std::cout << "entity created : " << entity << "\n";
        
        return entity;
    }
    
    void DisableEntity(int entity)
    {
        mEntityMasks[entity].reset();
    }
    
    template<typename T>
    void RegisterComponent()
    {
        // register new component type by store an id tied to its typeName in map
        std::string typeName = typeid(T).name();
        mComponentTypes.insert({typeName, mNextComponentType});
        ++mNextComponentType;
        
        // create new componentArray of this new component
        mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
    }
    

    template<typename T>
    void AddComponent(int entity, T component)
    {
        std::string typeName = typeid(T).name();
        
        int componentType = GetComponentType<T>();
        mEntityMasks[entity].set(componentType);
        
        GetComponentArray<T>(typeName)->Add(entity, component);
    }
    
    
    template<typename T>
    T& GetComponent(int entity)
    {
        std::string typeName = typeid(T).name();
        
        return GetComponentArray<T>(typeName)->Get(entity);
    }
    template<typename T>
    bool HasComponent(int entity)
    {
        std::string typeName = typeid(T).name();
        
        int componentType = GetComponentType<T>();
        return mEntityMasks[entity].test(componentType);
    }
    
    template<typename T>
    int GetComponentType()
    {
        std::string typeName = typeid(T).name();
        
        return mComponentTypes[typeName];
    }
    
    
    template<typename T>
    void RegisterSystem()
    {
        std::string typeName = typeid(T).name();
        
        mSystems.insert({typeName, std::make_shared<T>()});
    }
    
    
    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        std::string typeName = typeid(T).name();
        
        mSystemSignatures.insert({typeName, signature});
    }
    
    template<typename T>
    Signature GetSystemSignature()
    {
        std::string typeName = typeid(T).name();
        
        return mSystemSignatures[typeName];
    }
    
    std::bitset<MAX_COMPONENTS> GetEntityMask(int index)
    {
        return mEntityMasks[index];
    }
    
    void SetPlayerEntity(int entity)
    {
        playerEntity = entity;
    }
    
    int GetPlayerEntity()
    {
        return playerEntity;
    }
    
    void Tick(float dt, sf::RenderWindow& window)
    {
        for (auto& pair : mSystems)
        {
            auto& system = pair.second;
            system->Update(dt, window);
        }
    }
    

private:
    int playerEntity;
    
    std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITIES> mEntityMasks;
    
    std::queue<int> mAvialableEntities;
    std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrays;

    std::unordered_map<std::string, int> mComponentTypes;
    std::unordered_map<std::string, Signature> mSystemSignatures;

    std::unordered_map<std::string, std::shared_ptr<System>> mSystems;
    int mNextComponentType = 0;
    
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray(std::string typeName)
    {
        return std::static_pointer_cast<ComponentArray<T>> (mComponentArrays[typeName]);
    }
};


#endif

