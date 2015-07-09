
#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.h"
#include "tile.h"
#include "tileType.h"
#include <math.h>
#include "ladderType.h"
#include "door.h"
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "moving_block.h"

#include "falling_block.h"


Falling_Block::Falling_Block() : collision(false), falling(false)
{
setLiftable(false);
}
void Falling_Block::update(std::vector<gameMap> &allMaps, int currentMap,
                           std::vector<Character> & playerSet)
{
gameMap & map = allMaps[currentMap];
   if(!collision && !falling)
    {
        currentFrame = 0;


    }else if(collision && !falling)
    {
        if(currentFrame == framesTilFall)
        {
            falling = true;
        }else
        {
            currentFrame++;
        }
    }else
    {
        sf::Vector2f posc1, posc2;
        setPosition(sf::Vector2f(getPosition().x,getPosition().y+getVelocity().y));
        setVelocityY(getVelocity().y + map.getGravity());
        if(getVelocity().y > map.getTile(0).getTileDims().y/2+ playerSet[0].getDimensions().y/2-1)
           {
               setVelocityY(map.getTile(0).getTileDims().y/2+ playerSet[0].getDimensions().y/2-1);
           }
        sf::Vector2i corners = getCornerType(CORNER_DOWNRIGHT,
                                                     CORNER_DOWNLEFT,
                                                     map, posc1,posc2);

                if(!map.getTileType(corners.x).getWalkable()||
               !map.getTileType(corners.y).getWalkable())
               {
                   map.setKill(id);
               }
                for(int obNum = 0;obNum < map.getFallingBlocks().size();obNum++)
                {
                    if(checkForCollision(map.getFallingBlocks()[obNum]))
                    {
                        map.setKill(id);
                    }
                }

    for(int movNum =0 ; movNum < map.getMovingBlocks().size();movNum++)
    {
     if(checkForCollision(map.getMovingBlocks()[movNum]))
        {
             map.setKill(id);
        }
    }

        for(int charNum =0 ; charNum < playerSet.size();charNum++)
    {
     if(checkForCollision(playerSet[charNum]))
        {
            map.setKill(id);
        }
    }

        for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
    {
        if(checkForCollision(map.getAllObjects()[obNum]))
        {
            map.setKill(id);
        }
    }
    }
    setCollision(false);
    sprite.setPosition(sf::Vector2f(round(getPosition().x),round(getPosition().y)));

}


bool Falling_Block::getCollision()
{
    return collision;
}
void Falling_Block::setCollision(bool p_col)
{
    collision = p_col;
}
bool Falling_Block::getCloud()
{
    return cloud;
}

void Falling_Block::setCloud(bool p_cloud)
{
    cloud = p_cloud;
}



void Falling_Block::setFramesTilFall(int p_framesTilFall)
{
    framesTilFall = p_framesTilFall;
}
