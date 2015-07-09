    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include "character.h"
    #include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include "object_type.h"
#include "gameMap.h"
    #include "movable.h"
    #include "moving_block.h"
    #include "falling_block.h"
    #include <math.h>
    Moving_Block::Moving_Block() : currentPath(0), frameBuffer(0),
    nextOne(1), numFrames(0)
    {
        dir = "left";
    }
    void Moving_Block::setCloud(bool p_cloud)
    {
        cloud = p_cloud;
    }
    bool Moving_Block::getCloud()
    {
        return cloud;
    }
    void Moving_Block::setCurrentPath(int p_path)
    {
        currentPath = p_path;
    }
    int Moving_Block::getCurrentPath()
    {
        return currentPath;
    }
    std::vector<sf::Vector2f> Moving_Block::getPath()
    {
        return path;
    }
    void Moving_Block::setPath(std::vector<sf::Vector2f> p_path)
    {
        path = p_path;
    }
    sf::Vector2f Moving_Block::getSpeedAndBuffer()
    {
        return speedAndBuffer;
    }
    void Moving_Block::setSpeedAndBuffer(sf::Vector2f p_SpeedAndBuffer)
    {
        speedAndBuffer = p_SpeedAndBuffer;
            unAccel = powf(speedAndBuffer.x,2)/(2*speedAndBuffer.y);
    maxFrames = round(sqrt(speedAndBuffer.y*2/unAccel));
    }
void Moving_Block::update(std::vector<gameMap> &allMaps,
                            std::vector<Character> & playerSet,
                             int &currentMap,
                             std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy)
{
    gameMap & map = allMaps[currentMap];
bool used = false;
bool check = false;
    //find distance from beginning to current
    float xdist = path[currentPath].x-getPosition().x;
    float ydist = path[currentPath].y-getPosition().y;
    float distance = sqrtf(powf(xdist,2)+powf(ydist,2));
    float xPath = -(path[currentPath].x - path[nextOne].x);
    float yPath = -(path[currentPath].y - path[nextOne].y);
    float distancePath = sqrtf(powf(xPath,2)+powf(yPath,2));
    setYBeforeMovement();
    if(distance <= speedAndBuffer.y)
    {
        setVelocityX(getVelocity().x+(xPath/distancePath)*unAccel);
        setVelocityY(getVelocity().y+(yPath/distancePath)*unAccel);
        used = true;

    }

    xdist = path[nextOne].x-getPosition().x;
    ydist = path[nextOne].y-getPosition().y;
    distance = sqrtf(powf(xdist,2)+powf(ydist,2));

    if(distance <= speedAndBuffer.y)
    {
        setVelocityX(getVelocity().x-(xPath/distancePath)*unAccel);
        setVelocityY(getVelocity().y-(yPath/distancePath)*unAccel);
        numFrames++;
    }

    if(numFrames == maxFrames)
    {
        check = true;
        numFrames = 0;
        setVelocityX(0);
            setVelocityY(0);
        if(currentPath == getPath().size()-2 && dir=="left")
        {

        nextOne = getPath().size()-2;
        currentPath = getPath().size()-1;
        dir = "right";
        }else if(currentPath == 1 && dir == "right")
        {
          nextOne = 1;
        currentPath = 0;
        dir = "left";
        }else if(dir == "left"){
        nextOne++;
        currentPath++;
        }else{
            nextOne--;
        currentPath--;
        }
        setPosition(sf::Vector2f(path[currentPath].x,getPosition().y));
    }
    setPosition(sf::Vector2f(getPosition().x + getVelocity().x, getPosition().y));

    for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
    {


        if(!getCloud() &&checkForCollision(map.getAllObjects()[obNum]))
        {
    Movable_Object & obj = map.getAllObjects()[obNum];
            int dir = copysign(1,obj.getPosition().x - getPosition().x);
            alreadyCheckedx.push_back(id);
                                    float newX;
                newX = getPosition().x + (getDimensions().x/2+obj.getDimensions().x/2)*dir;

            obj.setPosition(sf::Vector2f(newX,obj.getPosition().y));

            if(dir ==1)
            {
                obj.update(allMaps, playerSet, currentMap,
                           alreadyCheckedx, alreadyCheckedy,
                            false, false, true, false, false);
            }else{
                obj.update(allMaps, playerSet, currentMap,
                           alreadyCheckedx,  alreadyCheckedy,
                           false, true, false, false ,false);

            }
        }
    }
for(int charNum =0 ; charNum < playerSet.size();charNum++)
    {


    if(!getCloud()&& checkForCollision(playerSet[charNum]))
        {
            Character & obj = playerSet[charNum];
            alreadyCheckedx.push_back(id);
            int dir = copysign(1,obj.getPosition().x - getPosition().x);
            float newX;
            newX = getPosition().x + (getDimensions().x/2+obj.getDimensions().x/2)*dir;
            obj.setPosition(sf::Vector2f(newX,obj.getPosition().y));
            obj.setClimbing(false);
            if(dir ==1)
            {

                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                            false, false, true, false, false);
            }else{

                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, true, false, false ,false);
            }
    }
    }
if(check)
{
    setPosition(sf::Vector2f(getPosition().x,path[currentPath].y));
}
    setPosition(sf::Vector2f(getPosition().x, getPosition().y + getVelocity().y));

       for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
    {
        if(checkForCollision(map.getAllObjects()[obNum]) && !getCloud() )
        {

            Movable_Object & obj = map.getAllObjects()[obNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            alreadyCheckedy.push_back(id);
                                    float newY;
                newY = getPosition().y + (getDimensions().y/2+obj.getDimensions().y/2)*dir;
            obj.setPosition(sf::Vector2f(obj.getPosition().x,newY));

            if(dir ==1)
            {


                obj.setVelocityY(getVelocity().y);
                obj.update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, false, false, false ,true);

            }else{
                obj.setVelocityY(0);
                obj.update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, false, false, true ,false);
            }


        }else if(getCloud() && checkForCollision(map.getAllObjects()[obNum])  && yBeforeMovement > getPosition().y)
        {
            float tempy = getPosition().y;
            setPosition(sf::Vector2f(getPosition().x,yBeforeMovement));
            if(!checkForCollision(map.getAllObjects()[obNum]))
            {
                setPosition(sf::Vector2f(getPosition().x,tempy));
                Movable_Object & obj = map.getAllObjects()[obNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            alreadyCheckedy.push_back(id);
                                    float newY;
                newY = getPosition().y + (getDimensions().y/2+obj.getDimensions().y/2)*dir;
            obj.setPosition(sf::Vector2f(obj.getPosition().x,newY));
                obj.setVelocityY(0);
                obj.update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, false, false, true ,false);
            }else
            {
                setPosition(sf::Vector2f(getPosition().x,tempy));
            }
        }
        }

for(int charNum =0 ; charNum < playerSet.size();charNum++)
    {
                bool checked = false;
        for(int i =0; i<alreadyCheckedy.size();i++)
        {
            if(alreadyCheckedy[i] == playerSet[charNum].getId())
            {
                checked = true;
            }
        }

    if(!checked && checkForCollision(playerSet[charNum])&& !getCloud())
        {
            Character & obj = playerSet[charNum];
            obj.setClimbing(false);
            alreadyCheckedy.push_back(id);
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            float newY;
            newY = getPosition().y + (getDimensions().y/2+obj.getDimensions().y/2)*dir;

            if(dir ==1)
            {


                    obj.setPosition(sf::Vector2f(obj.getPosition().x,newY));
                obj.setVelocityY(getVelocity().y);
                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, false, false, false ,true);

            }else{

                obj.setPosition(sf::Vector2f(obj.getPosition().x,newY));
                obj.setVelocityY(0);
                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, false, false, true ,false);
            }
    }else if(getCloud() && checkForCollision(playerSet[charNum])  && yBeforeMovement > getPosition().y)
        {
            float tempy = getPosition().y;
            setPosition(sf::Vector2f(getPosition().x,yBeforeMovement));
            if(!checkForCollision(playerSet[charNum]))
            {
                setPosition(sf::Vector2f(getPosition().x,tempy));
                Movable_Object & obj = playerSet[charNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            alreadyCheckedy.push_back(id);
                                    float newY;
                newY = getPosition().y + (getDimensions().y/2+obj.getDimensions().y/2)*dir;
            obj.setPosition(sf::Vector2f(obj.getPosition().x,newY));
                obj.setVelocityY(0);
                obj.update(allMaps, playerSet, currentMap,
                                           alreadyCheckedx, alreadyCheckedy,
                                           false, false, false, true ,false);
            }else
            {
                setPosition(sf::Vector2f(getPosition().x,tempy));
            }
        }
    }


    sprite.setPosition(sf::Vector2f(round(getPosition().x),round(getPosition().y)));
}

void Moving_Block::update(std::vector<gameMap> &allMaps, std::vector<Character> & playerSet,
             int &currentMap,
             std::vector<int> & alreadyCheckedx,
             std::vector<int> & alreadyCheckedy, bool withGrav,
            bool checkLeft, bool checkRight,
            bool checkUp, bool checkDown)
            {

            }


