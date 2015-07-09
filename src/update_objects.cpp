#include <string>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include <fstream>
#include <iostream>
#include "character.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include "object_type.h"

#include "movable.h"
#include "movable_object.h"
#include "moving_block.h"
#include "update_objects.h"
#include "gameMap.h"
#include <vector>
void updateAllObjects(sf::RenderWindow & window, std::vector<gameMap> &allMaps, int currentMap,std::vector<Character> & playerSet )
{
    gameMap & map = allMaps[currentMap];
            for(int obNum = 0;obNum < map.getMovingBlocks().size();obNum++)
    {
         std::vector <int> alreadyCheckedx;
        std::vector <int> alreadyCheckedy;
            map.getMovingBlocks()[obNum].update(allMaps, playerSet, currentMap, alreadyCheckedx,alreadyCheckedy);
    }
    for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
    {
        std::vector <int> alreadyCheckedx;
        std::vector <int> alreadyCheckedy;
        map.getAllObjects()[obNum].setYBeforeMovement();
        map.getAllObjects()[obNum].setXBeforeMovement();
            map.getAllObjects()[obNum].update(allMaps, playerSet, currentMap, alreadyCheckedx,alreadyCheckedy, true, false ,false, false, false);
map.getAllObjects()[obNum].setYBeforeMovement();
        map.getAllObjects()[obNum].setXBeforeMovement();
    }
    for(int obNum = 0; obNum < map.getFallingBlocks().size();obNum++)
    {
        map.getFallingBlocks()[obNum].update(allMaps, currentMap, playerSet);
    }
}
void drawAllObjects(sf::RenderWindow & window, std::vector<gameMap> &allMaps, int currentMap,std::vector<Character> & playerSet )
{
    gameMap & map = allMaps[currentMap];
        for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
    {
        window.draw(map.getAllObjects()[obNum].getSprite());
    }
        for(int obNum = 0;obNum < map.getMovingBlocks().size();obNum++)
    {
            window.draw(map.getMovingBlocks()[obNum].getSprite());
    }
        for(int obNum = 0; obNum < map.getFallingBlocks().size();obNum++)
    {
        window.draw(map.getFallingBlocks()[obNum].getSprite());
    }

}
