//A gameMap stores all vital information abou the map
//map - 2d vector with the actual map
//tileSet - set of all tiles
//tileTypeSet - all the different tile types
//ladderTypeSet - all the different types of ladders
//doorSet - all the doors in the map - mapped for easy access
//fullDoorSet - easier to go through than the mapped doorset
//loadedFrom - the file this map was loaded from
//gravity - the gravity in pixels/frame
//sideScroll - is the map a sidescroller?

#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include "character.h"
#include <fstream>
#include <iostream>
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include "object_type.h"

#include "movable.h"
#include "movable_object.h"
#include "moving_block.h"
#include "falling_block.h"
#include "gameMap.h"
#include <vector>
gameMap:: gameMap(std::vector <std::vector<int> > p_map,
                   std::vector<Tile> p_tileSet,std::vector<TileType> p_tileTypeSet,
                   std::vector<LadderType> p_ladderTypeSet,
                   std::map< int ,std::map < int,Door > > p_doorSet,
                   std::vector<Door> p_fullDoorSet,
                   std::string p_loadedFrom,
                   std::vector<Movable_Object> p_allObjects,
                   std::vector<Object_Type> p_objectTypeSet,
                   std::vector<Moving_Block> p_allMovingBlocks,
                   std::vector<Falling_Block> p_allFallingBlocks,
                   float p_gravity, bool p_setSideScroll)
{
    map = p_map;
    tileSet = p_tileSet;
    tileTypeSet = p_tileTypeSet;
    gravity = p_gravity;
    sideScroll = p_setSideScroll;
    ladderTypeSet = p_ladderTypeSet;
    doorSet = p_doorSet;
    loadedFrom = p_loadedFrom;
    fullDoorSet = p_fullDoorSet;
    allObjects = p_allObjects;
    objectTypeSet = p_objectTypeSet;
    allMovingBlocks = p_allMovingBlocks;
    allFallingBlocks = p_allFallingBlocks;
}

std::vector<Falling_Block> & gameMap::getFallingBlocks()
{
    return allFallingBlocks;
}

//Gets the height of the map
int gameMap::getHeight()
{
    return map.size();
}

//Gets the width o fthe map
int gameMap::getWidth()
{
    return map[0].size();
}

//Returns the tile for a specific number
Tile gameMap::getTile(int num)
{
    return tileSet[num];
}

//Returned the number of the tile at x,y
int gameMap::tileNum(int x, int y)
{
    return map[y][x];
}

//Returns the tileType for the num
TileType gameMap::getTileType(int num)
{
    return tileTypeSet[num];
}

//return the gravity
float gameMap::getGravity()
{
    return gravity;
}

//Set the map;s gravity
void gameMap::setGravity(float p_gravity)
{
    gravity = p_gravity;
}

//Get whether of not it is a side scroller
bool gameMap::getSideScroll()
{
    return sideScroll;
}

//Set whether of not it is a side scroller
void gameMap::setSideScroll(bool p_setSideScroll)
{
    sideScroll = p_setSideScroll;
}

//Get the ladder type for the num
LadderType gameMap::getLadderType(int num)
{
    return ladderTypeSet[num];
}

//Gets the door at position x,y
Door& gameMap::getDoor(int x, int y)
{
    return doorSet[x][y];
}

//Find out where the map was loaded from
std::string gameMap::getLoadedFrom()
{
    return loadedFrom;
}

//returns all the doors in the current Map
std::vector<Door> & gameMap::getFullDoorSet()
{
    return fullDoorSet;
}
std::vector<Movable_Object> & gameMap::getAllObjects()
{
    return allObjects;
}
std::vector<Object_Type> gameMap::getObjectTypeSet()
{
    return objectTypeSet;
}
std::vector<Moving_Block> & gameMap::getMovingBlocks()
{
    return allMovingBlocks;
}
std::vector<int> & gameMap::getKillList()
{
    return killList;
}
void gameMap::setKill(int p_kill)
{
    bool newKill = true;
    for(int i=0;i<killList.size();i++)
    {
        if(killList[i]==p_kill)
        {
            newKill = false;
        }
    }
    if(newKill)
    {
        killList.push_back(p_kill);
    }
}
