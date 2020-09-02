#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include <SFML/System/Vector2.hpp>

#include "../core/GameEngine.h"
#include "../components/PositionComponent.h"
#include "../components/RigidbodyComponent.h"
#include "../components/GravityComponent.h"


extern GameEngine gameEngine;

class GravitySystem : public System
{
public:
    void Update(float dt, sf::RenderWindow& window) override
    {
        for (int i = 0; i < MAX_ENTITIES; ++i)
        {
            Signature physicsSignature = gameEngine.GetSystemSignature<GravitySystem>();
            if ((gameEngine.GetEntityMask(i) & physicsSignature) == physicsSignature)
            {
                PositionComponent& positionComponent = gameEngine.GetComponent<PositionComponent>(i);
                RigidbodyComponent& rigidbodyComponent = gameEngine.GetComponent<RigidbodyComponent>(i);
                GravityComponent& gravityComponent = gameEngine.GetComponent<GravityComponent>(i);
                
                if (!gravityComponent.enable)
                    continue;
             
                positionComponent.y += rigidbodyComponent.velocity.y * dt;
                rigidbodyComponent.velocity.y += gravityComponent.force * dt;
            }
        }
    }
};

#endif

