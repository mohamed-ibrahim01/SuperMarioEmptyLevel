#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SFML/Graphics/Rect.hpp>


extern GameEngine gameEngine;

class AnimationSystem : public System
{
public:
    void Update(float dt, sf::RenderWindow& window) override
    {
        for (int i = 0; i < MAX_ENTITIES; ++i)
        {
            Signature animationSignature = gameEngine.GetSystemSignature<AnimationSystem>();
            if ((gameEngine.GetEntityMask(i) & animationSignature) == animationSignature)
            {
                RenderComponent& renderComponent = gameEngine.GetComponent<RenderComponent>(i);
                AnimationComponent& animationComponent = gameEngine.GetComponent<AnimationComponent>(i);
             
                animationComponent.totalTime += dt;
                
                if (animationComponent.totalTime >= animationComponent.animationTime)
                {
                    ++animationComponent.currentAnimation;
                    animationComponent.totalTime = 0;
                }
                
                if (animationComponent.currentAnimation == animationComponent.animationsNum)
                {
                    animationComponent.currentAnimation = 0;
                }
                
                sf::IntRect currentRect;
                currentRect.height = animationComponent.height;
                currentRect.top = animationComponent.row * animationComponent.height;
                
                if (animationComponent.faceRight)
                {
                    currentRect.left = animationComponent.currentAnimation * animationComponent.width;
                    currentRect.width = animationComponent.width;
                }
                else
                {
                    currentRect.left = (animationComponent.currentAnimation + 1) * animationComponent.width;
                    currentRect.width = -animationComponent.width;
                }
                
                renderComponent.sprite->setTextureRect(currentRect);
            }
        }
    }
};

#endif

