#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../core/GameEngine.h"
#include "../components/PositionComponent.h"
#include "../components/RenderComponent.h"


extern GameEngine gameEngine;


class RenderSystem : public System
{
public:
    void Update(float dt, sf::RenderWindow& window) override
    {
        for (int i = 0; i < MAX_ENTITIES; ++i)
        {
            Signature renderSignature = gameEngine.GetSystemSignature<RenderSystem>();
            if ((gameEngine.GetEntityMask(i) & renderSignature) == renderSignature)
            {
                RenderComponent& renderComponent = gameEngine.GetComponent<RenderComponent>(i);
                PositionComponent& positionComponent = gameEngine.GetComponent<PositionComponent>(i);
                
                renderComponent.sprite->setPosition(positionComponent.x, positionComponent.y);
            
                window.draw(*renderComponent.sprite);
            }
        }
    }
};

#endif

