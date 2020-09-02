#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <math.h>
#include <SFML/System/Vector2.hpp>

extern GameEngine gameEngine;

class CollisionSystem : public System
{
    enum CollisionDirections {NONE, RIGHT, LEFT, TOP, DOWN};
public:
    void Update(float dt, sf::RenderWindow& window) override
    {
        int playerEntity = gameEngine.GetPlayerEntity();
        PositionComponent& playerPositionComponent = gameEngine.GetComponent<PositionComponent>(playerEntity);
        CollisionComponent& playerCollisionComponent = gameEngine.GetComponent<CollisionComponent>(playerEntity);
        GravityComponent& playerGravityComponent = gameEngine.GetComponent<GravityComponent>(playerEntity);
        RigidbodyComponent& playerRigidbodyComponent = gameEngine.GetComponent<RigidbodyComponent>(playerEntity);
        
        
        for (int i = 0; i < MAX_ENTITIES && i != playerEntity; ++i)
        {
            Signature collisionSignature = gameEngine.GetSystemSignature<CollisionSystem>();
            if ((gameEngine.GetEntityMask(i) & collisionSignature) == collisionSignature)
            {
                PositionComponent& positionComponent = gameEngine.GetComponent<PositionComponent>(i);
                CollisionComponent& collisionComponent = gameEngine.GetComponent<CollisionComponent>(i);
             
                float deltaX = positionComponent.x - playerPositionComponent.x;
                float deltaY = positionComponent.y - playerPositionComponent.y;
                
                float intersectX = fabs(deltaX) - (playerCollisionComponent.halfSize.x + collisionComponent.halfSize.x);
                float intersectY = fabs(deltaY) - (playerCollisionComponent.halfSize.y + collisionComponent.halfSize.y);
                
                
                int collisionDir = NONE;
                if (intersectX < 0 && intersectY < 0)
                {
                    if (intersectX > intersectY)
                    {
                        if (deltaX > 0)
                        {
                            positionComponent.x += -intersectX * playerCollisionComponent.push;
                            playerPositionComponent.x += intersectX * (1 - playerCollisionComponent.push);
                            collisionDir = RIGHT;
                        }
                        else
                        {
                            positionComponent.x += intersectX * playerCollisionComponent.push; 
                            playerPositionComponent.x += -intersectX * (1 - playerCollisionComponent.push); 
                            collisionDir = LEFT;
                        }
                    }
                    else
                    {
                        if (deltaY > 0)
                        {
                            positionComponent.y += -intersectY * playerCollisionComponent.push;
                            playerPositionComponent.y += intersectY * (1 - playerCollisionComponent.push);
                            collisionDir = DOWN;
                        }
                        else
                        {
                            positionComponent.y += intersectY * playerCollisionComponent.push;
                            playerPositionComponent.y += -intersectY * (1 - playerCollisionComponent.push);
                            collisionDir = TOP;
                        }
                    }
                }
                
                if (collisionDir == DOWN)
                {
                    playerGravityComponent.enable = false;
                    playerRigidbodyComponent.velocity.y = 0;
                    playerRigidbodyComponent.canJump = true;
                }
                else if (collisionDir == TOP)
                {
                    playerRigidbodyComponent.velocity.y = 0;
                    if (gameEngine.HasComponent<TileComponent>(i))
                    {
                        TileComponent& tileComponent = gameEngine.GetComponent<TileComponent>(i);
                        if (tileComponent.type == WALL)
                        {
                            gameEngine.DisableEntity(i);   
                        }
                        else if (tileComponent.type == COIN)
                        {
                            AnimationComponent& animationComponent = gameEngine.GetComponent<AnimationComponent>(i);
                            animationComponent.row = 1;
                        }
                    }
                }
                else if (collisionDir == NONE)
                {
                    playerGravityComponent.enable = true;
                }
            }
        }
    }
};

#endif

