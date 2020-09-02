#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "ecs/core/GameEngine.h"
#include "ecs/systems/RenderSystem.h"
#include "ecs/systems/GravitySystem.h"
#include "Utils.h"
#include "Map.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/GravityComponent.h"
#include "ecs/components/InputComponent.h"
#include "ecs/components/InputComponent.h"
#include "ecs/systems/InputSystem.h"
#include "ecs/components/AnimationComponent.h"
#include "ecs/systems/MovementSystem.h"
#include "ecs/systems/AnimationSystem.h"
#include "ecs/components/CollisionComponent.h"
#include "ecs/systems/CollisionSystem.h"
#include "ecs/components/TileComponent.h"

using namespace std;

GameEngine gameEngine;

int main(int argc, char** argv)
{

    gameEngine.Init();
    
    gameEngine.RegisterComponent<PositionComponent>();
    gameEngine.RegisterComponent<RenderComponent>();
    gameEngine.RegisterComponent<RigidbodyComponent>();
    gameEngine.RegisterComponent<GravityComponent>();
    gameEngine.RegisterComponent<InputComponent>();
    gameEngine.RegisterComponent<AnimationComponent>();
    gameEngine.RegisterComponent<CollisionComponent>();
    gameEngine.RegisterComponent<TileComponent>();
    
    
    gameEngine.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(gameEngine.GetComponentType<PositionComponent>());
        signature.set(gameEngine.GetComponentType<RenderComponent>());
        gameEngine.SetSystemSignature<RenderSystem>(signature);
    }
    
    gameEngine.RegisterSystem<GravitySystem>();
    {
        Signature signature;
        signature.set(gameEngine.GetComponentType<PositionComponent>());
        signature.set(gameEngine.GetComponentType<RigidbodyComponent>());
        signature.set(gameEngine.GetComponentType<GravityComponent>());
        gameEngine.SetSystemSignature<GravitySystem>(signature);
    }
    
    gameEngine.RegisterSystem<InputSystem>();
    {
        Signature signature;
        signature.set(gameEngine.GetComponentType<InputComponent>());
        gameEngine.SetSystemSignature<InputSystem>(signature);
    }
    
    gameEngine.RegisterSystem<MovementSystem>();
    {
        Signature signature;
        signature.set(gameEngine.GetComponentType<InputComponent>());
        signature.set(gameEngine.GetComponentType<PositionComponent>());
        signature.set(gameEngine.GetComponentType<RigidbodyComponent>());
        signature.set(gameEngine.GetComponentType<AnimationComponent>());
        signature.set(gameEngine.GetComponentType<GravityComponent>());
        gameEngine.SetSystemSignature<MovementSystem>(signature);
    }
    
    gameEngine.RegisterSystem<AnimationSystem>();
    {
        Signature signature;
        signature.set(gameEngine.GetComponentType<RenderComponent>());
        signature.set(gameEngine.GetComponentType<AnimationComponent>());
        gameEngine.SetSystemSignature<AnimationSystem>(signature);
    }
    
    gameEngine.RegisterSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(gameEngine.GetComponentType<PositionComponent>());
        signature.set(gameEngine.GetComponentType<CollisionComponent>());
        gameEngine.SetSystemSignature<CollisionSystem>(signature);
    }
    
    Map::Load(gameEngine, "map_info/map_info.txt", "map_info/level2.txt");
    
    int marioEntity = gameEngine.CreateEntity();
    gameEngine.SetPlayerEntity(marioEntity);
    gameEngine.AddComponent<PositionComponent>(marioEntity, PositionComponent{100.0f,100.0f});
    std::shared_ptr<sf::Sprite> sprite(new sf::Sprite(Utils::LoadTexture("assets/mario_sheet.png")));
    sprite->setOrigin(16, 32);
    gameEngine.AddComponent<RenderComponent>(marioEntity, RenderComponent{sprite});
    gameEngine.AddComponent<RigidbodyComponent>(marioEntity, RigidbodyComponent{250, 200, false, sf::Vector2f(), sf::Vector2f()});
    gameEngine.AddComponent<InputComponent>(marioEntity, InputComponent{NONE});
    gameEngine.AddComponent<AnimationComponent>(marioEntity, AnimationComponent{32, 65, 3, 0.1f});
    gameEngine.AddComponent<CollisionComponent>(marioEntity, CollisionComponent{0, sf::Vector2u(16, 32)});
    gameEngine.AddComponent<GravityComponent>(marioEntity, GravityComponent{true, 980});
    
    
//    sf::RenderWindow window(sf::VideoMode(512, 480), "ECS Test");
    sf::RenderWindow window(sf::VideoMode(800, 640), "Mario .dev");
    sf::View view(sf::FloatRect(0, 0, 800, 640));
    
    
    float dt = 0;
    sf::Clock clock;
    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();
        sf::Event e{};
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        
        float playerPosX = gameEngine.GetComponent<PositionComponent>(gameEngine.GetPlayerEntity()).x;
        if (playerPosX + 200 > 400)
        {
            view.setCenter(playerPosX + 200, 320);
            window.setView(view);
        }
            
        window.clear(sf::Color(102, 178, 255));
        
        gameEngine.Tick(dt, window);
        
        window.display();
    }

    return 0;
}

