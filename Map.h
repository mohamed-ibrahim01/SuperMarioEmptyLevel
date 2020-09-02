#ifndef MAP_H
#define MAP_H
#include <fstream>
#include <sstream>
#include <SFML/Graphics/Sprite.hpp>
#include "ecs/core/GameEngine.h"
#include "Utils.h"
#include "ecs/components/CollisionComponent.h"
#include "ecs/components/AnimationComponent.h"
#include "ecs/components/TileComponent.h"


class Map
{
public:
    static void Load(GameEngine& gameEngine, std::string mapInfoFile, std::string levelFile)
    {
        ReadMapInfo(mapInfoFile);
        LoadLevel(gameEngine, levelFile);
    }
    
    static void AddTile(GameEngine& gameEngine, int tileIndex, float posX, float posY)
    {   
        sf::Vector2u tileHalfSize;
        
        if (tileIndex == 3)
        {
            tileHalfSize.x = 104/2;
            tileHalfSize.y = 128/2;
        }
        else 
        {
            tileHalfSize.x = mapInfo[TILE_WIDTH]/2;
            tileHalfSize.y = mapInfo[TILE_HEIGHT]/2;
        }
        
        int tileEntity = gameEngine.CreateEntity();
        gameEngine.AddComponent<PositionComponent>(tileEntity, PositionComponent{posX + tileHalfSize.x, posY + tileHalfSize.y});
        
        
        std::shared_ptr<sf::Sprite> sprite(new sf::Sprite(tileTextures[tileIndex-1]));
        sprite->setOrigin(tileHalfSize.x, tileHalfSize.y);
        gameEngine.AddComponent<RenderComponent>(tileEntity, RenderComponent{sprite});
        
        gameEngine.AddComponent<CollisionComponent>(tileEntity, CollisionComponent{1, tileHalfSize});
        
        if (tileIndex == 1)
        {
            gameEngine.AddComponent<TileComponent>(tileEntity, TileComponent{WALL});
        }
        else if (tileIndex == 2)
        {
            gameEngine.AddComponent<TileComponent>(tileEntity, TileComponent{COIN});
            gameEngine.AddComponent<AnimationComponent>(tileEntity, AnimationComponent{mapInfo[TILE_WIDTH],
            mapInfo[TILE_HEIGHT], 1});
        }   
    }
    
    
    static void ReadMapInfo(std::string mapInfoFileName)
    {
        std::fstream mapInfoFile;
        mapInfoFile.open(mapInfoFileName);
        
        mapInfoFile.ignore(1000, '\n');
        
        std::string str1, str2;
        getline(mapInfoFile, str1);
        std::stringstream ss(str1);

        for(int i = 0; i < MAP_INFO_NUM; ++i)
        {
            getline(ss, str2, ',');
            mapInfo[i] = stoi(str2);
        }
        
        for (int i = 0; i < mapInfo[TEXTURE_NUM]; i++)
        {
            std::string texturePath;
            getline(mapInfoFile, texturePath);
            tileTextures.push_back(Utils::LoadTexture(texturePath));
        }
        
        mapInfoFile.close();
    }
    
    static void LoadLevel(GameEngine& gameEngine, std::string levelFileName)
    {
        std::fstream levelFile;
        levelFile.open(levelFileName);
        assert(levelFile.is_open());
        
//        // background
//        int backgroundEntity = gameEngine.CreateEntity();
//        gameEngine.AddComponent<PositionComponent>(backgroundEntity, PositionComponent{0,0});
//        std::shared_ptr<sf::Sprite> sprite(new sf::Sprite(tileTextures[0]));
//        gameEngine.AddComponent<RenderComponent>(backgroundEntity, RenderComponent{sprite});
        

        for (int i = 0; i < mapInfo[VERTICAL_TILE_NUM]; i++)
        {
            for (int j = 0; j < mapInfo[HORIZONTAL_TILE_NUM]; j++)
            {
                char c;
                levelFile.get(c);   // tile index
                levelFile.ignore(); // comma ','
                int tileIndex = atoi(&c);
                
                if (tileIndex == 0)
                    continue;


                AddTile(gameEngine, tileIndex, j*mapInfo[TILE_WIDTH], i*mapInfo[TILE_HEIGHT]);
            }
        }

        levelFile.close();
    }
    
private:
    enum mapInfoIndexes {HORIZONTAL_TILE_NUM, VERTICAL_TILE_NUM, TILE_WIDTH, TILE_HEIGHT, TEXTURE_NUM, MAP_INFO_NUM};
    static int mapInfo[MAP_INFO_NUM];
    static std::vector<sf::Texture> tileTextures;
};

int Map::mapInfo[MAP_INFO_NUM];
std::vector<sf::Texture> Map::tileTextures;

#endif

