#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include "object_type.h"
#include "gameMap.h"
#include "character.h"
#include "movable.h"
#include "movable_object.h"
#include "moving_block.h"
#include "falling_block.h"


    Movable_Object::Movable_Object(sf::Vector2f p_position,
                                   sf::Vector2f p_dimensions,
                                   int p_ssNum,
                                   sf::Vector2u p_spriteLoc)
                                   {
                                       setPosition(p_position);
    setDimensions(p_dimensions);
    setSSNum(p_ssNum);
    setSpriteLoc(p_spriteLoc);
}
float Movable_Object::getPushingSpeed()
{
    return pushingSpeed;
}
void Movable_Object::setPushingSpeed(float p_pushingSpeed)
{
    pushingSpeed = p_pushingSpeed;
}
bool Movable_Object::getLiftable()
{
    return liftable;
}
void Movable_Object::setLiftable(bool p_liftable)
{
    liftable = p_liftable;
}
bool Movable_Object::getCharMovable()
{
    return charMovable;
}
void Movable_Object::setCharMovable(bool p_charMovable)
{
    charMovable = p_charMovable;
}

Movable_Object::Movable_Object(){
}


//Takes in two corners and returns the tile types of the corners



void Movable_Object::update(std::vector<gameMap> &allMaps,
                            std::vector<Character> & playerSet,
                             int &currentMap,
                             std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                            bool withGrav, bool checkLeft, bool checkRight,
                            bool checkUp, bool checkDown)
{

        gameMap & map = allMaps[currentMap];

        float yAfterSloped = getPosition().y;

        //check to see if we've been pushed by a moving block
            bool pushedByMovingBlockx = false;
            bool pushedByMovingBlocky = false;
checkIfPushedByMovingBlock(map,alreadyCheckedx,alreadyCheckedy,
                             pushedByMovingBlockx,
                             pushedByMovingBlocky);

    //A temporary holder
    sf::Vector2f posc1, posc2,posForCloud;


      if( fabs(getVelocity().x) > 0.01 || checkLeft || checkRight)
    {


        if(getVelocity().x<0 || checkLeft)
        {
            checkLR(map,-1, checkLeft, withGrav,
                               alreadyCheckedx, pushedByMovingBlockx,
                               yAfterSloped, alreadyCheckedy, pushedByMovingBlocky
                               );
        }else if(getVelocity().x>0 || checkRight)
        {
            checkLR(map,1, checkRight, withGrav,
                               alreadyCheckedx, pushedByMovingBlockx,
                               yAfterSloped, alreadyCheckedy, pushedByMovingBlocky);
        }

        if(checkRight || checkLeft|| withGrav)
        {
checkXCollisionsWithMovableObject(map, allMaps, currentMap,
                                  playerSet, alreadyCheckedx,
                                  alreadyCheckedy,
                                  pushedByMovingBlockx,
                                  map.getAllObjects(), true);

checkXCollisionsWithMovableObject(map, allMaps, currentMap,
                                  playerSet, alreadyCheckedx,
                                  alreadyCheckedy,
                                  pushedByMovingBlockx,
                                  playerSet, true);

checkXCollisionsWithMovableObject(map, allMaps, currentMap,
                                  playerSet, alreadyCheckedx,
                                  alreadyCheckedy,
                                  pushedByMovingBlockx,
                                  map.getFallingBlocks(), false);

    }
    }
    else
    {
        //If there's very low velocity and neither key is down, set it to zero
        setVelocityX(0);
    }



    if(checkLeft || checkRight || withGrav)
    {
            checkXCollisionsWithMovableObject(map, allMaps, currentMap,
                                  playerSet, alreadyCheckedx,
                                  alreadyCheckedy,
                                  pushedByMovingBlockx,
                                  map.getMovingBlocks(), false);
    }

setPosition(sf::Vector2f(getPosition().x, yAfterSloped));
    float gravEffects = copysign(1,map.getGravity());
    float gravForce= map.getGravity();
    if(map.getSideScroll() && withGrav)
    {
        gravityEffect(map, playerSet, gravEffects,  gravForce);
    }
    if((fabs(getVelocity().y) > 0.01 && withGrav) || checkDown || checkUp)
    {

        //Same as before but moving up
        if(getVelocity().y<0 || checkUp)
        {
            checkUD(map, -1, checkUp, withGrav,
             alreadyCheckedy, pushedByMovingBlocky, gravEffects);
        }
        //Same as up put for down
        if(getVelocity().y>0 || checkDown)
        {

            checkUD(map, 1, checkDown, withGrav,
             alreadyCheckedy, pushedByMovingBlocky, gravEffects);
        }


            if(checkUp || checkDown || withGrav)
            {
                 checkSlopedCorners(map,false, yAfterSloped, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
 setPosition(sf::Vector2f(getPosition().x, yAfterSloped));
            checkSlopedCorners(map,false, yAfterSloped, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
            setPosition(sf::Vector2f(getPosition().x, yAfterSloped));
checkYCollisionsWithMovableObject(map, allMaps, currentMap, playerSet, alreadyCheckedx,
                                alreadyCheckedy, map.getAllObjects());
checkYCollisionsWithMovableObject(map, allMaps, currentMap, playerSet, alreadyCheckedx,
                                alreadyCheckedy, playerSet);

checkYCollisionsWithFallingBlocks(map);

            }
checkYCollisionsWithMovingBlocks(map, allMaps, currentMap,
                                      playerSet, alreadyCheckedx, alreadyCheckedy,
                                                       withGrav);
    }else{
    }


checkIfInASlope(map);
    setVelocityX(0);
    sprite.setPosition(sf::Vector2f(round(getPosition().x),round(getPosition().y)));
}

void Movable_Object::checkForDeath(gameMap &map, std::vector<int> & alreadyChecked, bool pushedByBlock)
{

    if(pushedByBlock)
    {
        map.setKill(id);
        for(int i =0; i<alreadyChecked.size();i++)
        {
            bool MB = false;
        for(int movNum =0 ; movNum < map.getMovingBlocks().size();movNum++)
    {
            if(alreadyChecked[i] == map.getMovingBlocks()[movNum].getId())
            {
                MB = true;
            }
    }
    if(!MB)
    {
            map.setKill(alreadyChecked[i]);
    }
        }
    }
}

bool Movable_Object::getClimbing()
{
    return false;
}
bool Movable_Object::getCloud()
{
    return false;
}

void Movable_Object::checkIfPushedByMovingBlock(gameMap &map,
                                 std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
for(int movNum =0 ; movNum < map.getMovingBlocks().size();movNum++)
    {

            for(int i =0; i<alreadyCheckedx.size();i++)
        {
            if(alreadyCheckedx[i] == map.getMovingBlocks()[movNum].getId())
            {
                pushedByMovingBlockx = true;
            }
        }
                    for(int i =0; i<alreadyCheckedy.size();i++)
        {
            if(alreadyCheckedy[i] == map.getMovingBlocks()[movNum].getId())
            {
                pushedByMovingBlocky = true;
            }
        }
    }
}

void Movable_Object::checkLR(gameMap &map,const int dir, const bool &check, const bool &withGrav,
                               std::vector<int> & alreadyCheckedx, bool pushedByMovingBlockx,
                               float & yAfter,
                             std::vector<int> & alreadyCheckedy,
                             bool & pushedByMovingBlocky)
{
    sf::Vector2f newPos = getPosition();
        sf::Vector2f posc1, posc2;
                //Move the character
                        if(!check && withGrav)
            {
            newPos.x = getPosition().x+getVelocity().x;
            }
            setPosition(newPos);
            checkSlopedCorners(map, true, yAfter, alreadyCheckedx, alreadyCheckedy,
                               pushedByMovingBlockx, pushedByMovingBlocky);
            float tempY = getPosition().y;
           setPosition(sf::Vector2f(getPosition().x, yAfter));
           newPos = getPosition();
Corner C1;
Corner C2;
if(dir == -1)
{
    C1 = CORNER_UPLEFT;
    C2 = CORNER_DOWNLEFT;
}else
{
    C1 = CORNER_UPRIGHT;
    C2 = CORNER_DOWNRIGHT;
}

            //check top left/bottom left corner and get their type
            sf::Vector2i corners = getCornerType(C1,
                                                 C2,
                                                 map, posc1,posc2);

            //Check to set if either of the new corners are not walkable
            //If they are not, we reposition
            if(!map.getTileType(corners.x).getWalkable()||
               !map.getTileType(corners.y).getWalkable())
            {

                //Reposition the character
                newPos.x = (((1-dir)/2+posc1.x)*(map.getTile(0).getTileDims().x)-
                            dir*(getDimensions().x)/2);
                            checkForDeath(map, alreadyCheckedx,pushedByMovingBlockx);
            }
            setPosition(sf::Vector2f(newPos.x,yAfter));
}
template <typename T>
void Movable_Object::checkXCollisionsWithMovableObject(gameMap &map, std::vector<gameMap> &allMaps,
                                                       int currentMap,
                                                        std::vector<Character> & playerSet,
                                                       std::vector<int> & alreadyCheckedx,
                                                       std::vector<int> & alreadyCheckedy,
                                                       bool & pushedByMovingBlockx,
                                                       std::vector<T> & ObjectSet,
                                                       bool check)
{
    sf::Vector2f newPos = getPosition();
for(int obNum = 0;obNum < ObjectSet.size();obNum++)
    {
        bool checked = false;

        for(int i =0; i<alreadyCheckedx.size();i++)
        {
            if(alreadyCheckedx[i] == ObjectSet[obNum].getId())
            {
                checked = true;
            }
        }


        if(!checked &&checkForCollision(ObjectSet[obNum]))
        {

            setPosition(newPos);

            T & obj = ObjectSet[obNum];
            int dir = copysign(1,obj.getPosition().x - getPosition().x);
            alreadyCheckedx.push_back(id);
            float newX;
            if(fabs(getPosition().x - obj.getPosition().x)<obj.getPushingSpeed() || pushedByMovingBlockx)
            {
                newX = getPosition().x + (getDimensions().x/2+obj.getDimensions().x/2)*dir;
            }else
            {
                newX = obj.getPosition().x + obj.getPushingSpeed()*dir;
            }


            if(dir ==1)
            {
                 if(!obj.getClimbing() && check)
                {
                    obj.setPosition(sf::Vector2f(newX,obj.getPosition().y));
                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                           alreadyCheckedx, alreadyCheckedy,
                            false, false, true, false, false);
                }
               newPos.x = obj.getPosition().x-obj.getDimensions().x/2 - getDimensions().x/2;

            }else{
                 if(!obj.getClimbing() && check)
                {
                    obj.setPosition(sf::Vector2f(newX,obj.getPosition().y));
                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                           alreadyCheckedx,  alreadyCheckedy,
                           false, true, false, false ,false);
                }
               newPos.x = obj.getPosition().x+obj.getDimensions().x/2 +getDimensions().x/2;


            }

        }
    }
    setPosition(newPos);
}

void Movable_Object::gravityEffect(gameMap &map,std::vector<Character> & playerSet,
                   float gravEffects, float gravForce)
{
    //Add gravity to the velocity
        setVelocityY(gravForce+getVelocity().y);

        //Limit the velocity to 1 tile/frame
        if(fabs(getVelocity().y) >(map.getTile(0).getTileDims().y/2 +playerSet[0].getDimensions().y/2)-1)
        {
            setVelocityY((gravEffects*
                          map.getTile(0).getTileDims().y/2 +playerSet[0].getDimensions().y/2)-
                         gravEffects);
        }
}

void Movable_Object::checkUD(gameMap &map, const int dir, const bool &check, const bool &withGrav,
             std::vector<int> & alreadyCheckedy, bool pushedByMovingBlocky, float gravEffects)
{

    sf::Vector2f newPos = getPosition();
    sf::Vector2f posForCloud, posc1, posc2;
    Corner C1;
    Corner C2;
    if(dir == -1)
    {
        C1 = CORNER_UPLEFT;
        C2 = CORNER_UPRIGHT;
    }else
    {
        C1 = CORNER_DOWNLEFT;
        C2 = CORNER_DOWNRIGHT;
    }

    float temp = getPosition().y;
            setPosition(sf::Vector2f(getPosition().x, yBeforeMovement));
            bool cloudHit = false;
            sf::Vector2i cornersBeforeMovement = getCornerType(C1,
                                                 C2, map, posForCloud,posc2);
setPosition(sf::Vector2f(getPosition().x, temp));

            if(!check && withGrav)
            {
            newPos.y = getPosition().y+getVelocity().y;
            }
            setPosition(newPos);
            //Check all the corners if we are climbing and if they are
            //all not ladders, we can fall
            sf::Vector2i corners = getCornerType(C1, C2, map, posc1,posc2);
sf::Vector2i cornerToCheck;

if(dir == -1)
{
    cornerToCheck = cornersBeforeMovement;
}else
{
    cornerToCheck = corners;
}

                if(((map.getTileType(cornerToCheck.x).getCloud() &&
                     map.getTileType(cornerToCheck.x).getSlanted() == "No") ||
                    (map.getTileType(cornerToCheck.y).getCloud()) &&
                    map.getTileType(cornerToCheck.y).getSlanted() == "No") &&
                   posForCloud.y != posc1.y && (-dir)*gravEffects<0)
                {
                    cloudHit = true;
                }


            if(((!map.getTileType(corners.x).getWalkable()||
                !map.getTileType(corners.y).getWalkable())||cloudHit))
            {

                checkForDeath(map, alreadyCheckedy,pushedByMovingBlocky);
                setVelocityY(0);
                //if gravity is up and you aren't pressing space, you can
                //jump again
                newPos.y = (((1-dir)/2+posc1.y)*(map.getTile(0).getTileDims().y)-dir*(getDimensions().y)/2);
            }

            setPosition(newPos);

}

template <typename T>
void Movable_Object::checkYCollisionsWithMovableObject(gameMap &map, std::vector<gameMap> &allMaps,
                                                       int currentMap,
                                                        std::vector<Character> & playerSet,
                                                       std::vector<int> & alreadyCheckedx,
                                                       std::vector<int> & alreadyCheckedy,
                                                       std::vector<T> & ObjectSet)
{
    sf::Vector2f newPos = getPosition();
      for(int obNum = 0;obNum < ObjectSet.size();obNum++)
    {
                bool checked = false;
        for(int i =0; i<alreadyCheckedy.size();i++)
        {
            if(alreadyCheckedy[i] == ObjectSet[obNum].getId())
            {
                checked = true;
            }
        }
        if(!checked && checkForCollision(ObjectSet[obNum]))
        {
            setPosition(newPos);
            T & obj = ObjectSet[obNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            alreadyCheckedy.push_back(id);
            float newY = getPosition().y + (getDimensions().y/2+obj.getDimensions().y/2)*dir;
            if(dir ==1)
            {
               setVelocityY(obj.getVelocity().y);
               if(!obj.getClimbing())
                {
                      obj.setPosition(sf::Vector2f(obj.getPosition().x, newY));
                    obj.Movable_Object::update(allMaps, playerSet,currentMap,
                               alreadyCheckedx, alreadyCheckedy,
                                false, false, false, false, true);
                }
                newPos.y = obj.getPosition().y-obj.getDimensions().y/2 - getDimensions().y/2;

            }else{
                if(!obj.getClimbing())
                {
                    obj.setPosition(sf::Vector2f(obj.getPosition().x, newY));
                    obj.Movable_Object::update(allMaps, playerSet, currentMap,
                               alreadyCheckedx, alreadyCheckedy,
                               false, false, false, true, false);
                }
                newPos.y = obj.getPosition().y+obj.getDimensions().y/2 +getDimensions().y/2;

            }
            setPosition(newPos);
        }
    }
    setPosition(newPos);
}

void Movable_Object::checkYCollisionsWithFallingBlocks(gameMap &map)
{
    sf::Vector2f newPos  = getPosition();
        for(int obNum = 0;obNum < map.getFallingBlocks().size();obNum++)
    {

        if(checkForCollision(map.getFallingBlocks()[obNum]))
        {
                        Falling_Block & obj = map.getFallingBlocks()[obNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            setPosition(newPos);
            bool checkPrev;
            setPosition(sf::Vector2f(getPosition().x, yBeforeMovement));
            if(!checkForCollision(obj))
            {
                checkPrev = false;

            }else{
                checkPrev = true;
            }
            setPosition(newPos);
            if(dir ==1 && (!map.getFallingBlocks()[obNum].getCloud()||(getPosition().y > yBeforeMovement && !checkPrev)))
            {
                obj.setCollision(true);
                setVelocityY(0);
                newPos.y = obj.getPosition().y-obj.getDimensions().y/2 - getDimensions().y/2;
            }else if(!map.getFallingBlocks()[obNum].getCloud()){

                setVelocityY(0);
                newPos.y = obj.getPosition().y+obj.getDimensions().y/2 +getDimensions().y/2;

            }
            setPosition(newPos);
        }
    }
}

void Movable_Object::checkYCollisionsWithMovingBlocks(gameMap &map,
                                                      std::vector<gameMap> &allMaps,
                                                       int currentMap,
                                                        std::vector<Character> & playerSet,
                                                       std::vector<int> & alreadyCheckedx,
                                                       std::vector<int> & alreadyCheckedy,
                                                       bool withGrav)
{
    sf::Vector2f newPos = getPosition();
  for(int movNum =0 ; movNum < map.getMovingBlocks().size();movNum++)
    {
        bool checked = false;
        for(int i =0; i<alreadyCheckedy.size();i++)
        {
            if(alreadyCheckedy[i] == map.getMovingBlocks()[movNum].getId())
            {
                checked = true;
            }
        }
     if(!checked&&checkForCollision(map.getMovingBlocks()[movNum])&&!map.getMovingBlocks()[movNum].getCloud())
        {

            Moving_Block & obj = map.getMovingBlocks()[movNum];
           alreadyCheckedy.push_back(obj.getId());
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            if(dir ==1)
            {

                setVelocityY(obj.getVelocity().y);
                newPos.x = getPosition().x + obj.getVelocity().x;
                  newPos.y = obj.getPosition().y - (getDimensions().y/2+obj.getDimensions().y/2)*dir;
                setPosition(newPos);
                if(obj.getVelocity().x>0)
                {
                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, true, false, false);
                    newPos = getPosition();
                }else
                {
                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, true, false, false, false);
                    newPos = getPosition();
                }

                setPosition(newPos);

                if(obj.getVelocity().y>0)
                {

                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, false, false, true);
                    newPos = getPosition();
                }else
                {

                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, false, true, false);
                    newPos = getPosition();
                }
                setPosition(newPos);

            }else{

                setVelocityY(obj.getVelocity().y);

            }
        }else if(!checked &&checkForCollision(map.getMovingBlocks()[movNum])&&map.getMovingBlocks()[movNum].getCloud() && getPosition().y > yBeforeMovement )
        {
            float tempY = getPosition().y;
            setPosition(sf::Vector2f(getPosition().x, yBeforeMovement));
            if(!checkForCollision(map.getMovingBlocks()[movNum]))
            {
                setPosition(sf::Vector2f(getPosition().x, tempY));
                Moving_Block & obj = map.getMovingBlocks()[movNum];
            alreadyCheckedy.push_back(obj.getId());
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
                setVelocityY(obj.getVelocity().y);
                if(withGrav)
                {
                newPos.x = getPosition().x + obj.getVelocity().x;
                newPos.y = obj.getPosition().y - (getDimensions().y/2+obj.getDimensions().y/2)*dir;
                setPosition(newPos);

                if(obj.getVelocity().x>0)
                {
                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, true, false, false);
                    newPos = getPosition();
                }else
                {
                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, true, false, false, false);
                    newPos = getPosition();
                }
                }

                setPosition(newPos);

                if(obj.getVelocity().y>0)
                {
                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, false, false, true);
                    newPos = getPosition();
                }else
                {
                    update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, false, true, false);
                    newPos = getPosition();
                }
                setPosition(newPos);
            }else{
            setPosition(sf::Vector2f(getPosition().x, tempY));
            }
        }

    }
    setPosition(newPos);
}

void Movable_Object::getAllCorners(gameMap & map,sf::Vector2f &cornerDL, sf::Vector2f &cornerDR,
                   sf::Vector2f &cornerUL, sf::Vector2f &cornerUR,
                   sf::Vector2f &cornerDLxy, sf::Vector2f &cornerDRxy,
                   sf::Vector2f & cornerULxy, sf::Vector2f &cornerURxy,
                   sf::Vector2i &downCorner, sf::Vector2i & upCorner)
{
    Corner CDL = CORNER_DOWNLEFT;
    Corner CDR = CORNER_DOWNRIGHT;
    Corner CUL = CORNER_UPLEFT;
    Corner CUR = CORNER_UPRIGHT;
    cornerDL = getCorner(CDL);
    cornerDR = getCorner(CDR);
    cornerUL = getCorner(CUL);
    cornerUR = getCorner(CUR);
    downCorner = getCornerType(CDL, CDR, map, cornerDLxy, cornerDRxy);
    upCorner = getCornerType(CUL, CUR, map, cornerULxy, cornerURxy);
}

void Movable_Object::checkIfInASlope(gameMap &map)
{
        sf::Vector2i downCorner, upCorner;
    sf::Vector2f cornerDL, cornerDR, cornerUL, cornerUR;
    sf::Vector2f cornerDLxy, cornerDRxy, cornerULxy, cornerURxy;
        std::string dird, diru;
    Corner CDL = CORNER_DOWNLEFT;
    Corner CDR = CORNER_DOWNRIGHT;
    Corner CUL = CORNER_UPLEFT;
    Corner CUR = CORNER_UPRIGHT;
getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);
if(map.getTileType(downCorner.x).getSlanted()=="DL" &&
       checkSlopeDL(map, cornerDL, cornerDLxy))
       {
           sf::Vector2f beforeCorner, beforeCornerxy;
        sf::Vector2i beforeCornerType;
        getCornerBeforeMovement(map,beforeCornerType,
                                beforeCorner, beforeCornerxy,
                                 CORNER_DOWNLEFT);
        if(!map.getTileType(downCorner.x).getCloud() ||
           ((beforeCornerxy.x == cornerDLxy.x  && beforeCornerxy.y == cornerDLxy.y &&
                      (!checkSlopeDL(map, beforeCorner, beforeCornerxy)) ||
                      beforeCornerxy.x > cornerDLxy.x ||
                      beforeCornerxy.y < cornerDLxy.y)))
        {
           setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*cornerDLxy.x+cornerDL.y -
                                 map.getTile(0).getTileDims().y*cornerDLxy.y+getDimensions().x/2,getPosition().y));
        }

       }
       if(map.getTileType(downCorner.y).getSlanted()=="DR" &&
       checkSlopeDR(map, cornerDR, cornerDRxy))
       {
           sf::Vector2f beforeCorner, beforeCornerxy;
                sf::Vector2i beforeCornerType;
        getCornerBeforeMovement(map,beforeCornerType,
                                beforeCorner, beforeCornerxy, CORNER_DOWNRIGHT);
        if(!map.getTileType(downCorner.y).getCloud() ||
           (((beforeCornerxy.x == cornerDRxy.x  && beforeCornerxy.y == cornerDRxy.y &&
                      !checkSlopeDR(map, beforeCorner, beforeCornerxy)) ||
                      beforeCornerxy.x <cornerDRxy.x ||
                      beforeCornerxy.y <cornerDRxy.y)))
        {
           setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*cornerDRxy.x-cornerDR.y +
                                 map.getTile(0).getTileDims().y*(cornerDRxy.y+1)-getDimensions().x/2,getPosition().y));
        }
       }
       if(map.getTileType(upCorner.x).getSlanted()=="UL" &&
       checkSlopeUL(map, cornerUL, cornerULxy))
       {
           setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerULxy.x-1)+cornerUL.y -
                                 map.getTile(0).getTileDims().y*(cornerULxy.y)+getDimensions().x/2,getPosition().y));
       }
       if(map.getTileType(upCorner.y).getSlanted()=="UR" &&
       checkSlopeUR(map, cornerUR, cornerURxy))
       {
           setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerURxy.x)-cornerUR.y +
                                 map.getTile(0).getTileDims().y*(cornerURxy.y)-getDimensions().x/2,getPosition().y));
       }
}

void Movable_Object::checkIfMovedATileForSloped(gameMap & map, std::string &dir,
                                            int dirToCheck, float & yOutput,
                             std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
    Corner C1;
    Corner C2;
    sf::Vector2i corners;
    sf::Vector2f temp, cornerC1xy, cornerC2xy, cornerC1, cornerC2;
    std::string C1dir, C2dir;
    if(dirToCheck > 0)
    {
        C1 = CORNER_DOWNLEFT;
        C2 = CORNER_DOWNRIGHT;
        C1dir = "DL";
        C2dir = "DR";
    }else
    {
        C1 = CORNER_UPLEFT;
        C2 = CORNER_UPRIGHT;
        C1dir = "UL";
        C2dir = "UR";
    }
    corners = getCornerType(C1, C1, map, temp, cornerC1xy);
setPosition(sf::Vector2f(getPosition().x,yBeforeMovement));
   corners = getCornerType(C2, C2, map, temp, cornerC2xy);
   //If we;re moved onver
   if(cornerC1xy.y != cornerC2xy.y)
   {

       setPosition(sf::Vector2f(getPosition().x,
                                map.getTile(0).getTileDims().y*(cornerC2xy.y+(1+dirToCheck)/2) - dirToCheck*getDimensions().y/2 ));

                   cornerC1 = getCorner(C1);
                   cornerC2 = getCorner(C2);
    corners = getCornerType(C1, C2, map, cornerC1xy, cornerC2xy);
placeObjectOnSlope(map, dir, C1dir, C2dir, corners,
                    cornerC1, cornerC2, cornerC1xy, cornerC2xy,
                    yOutput, alreadyCheckedx, alreadyCheckedy,
                    pushedByMovingBlockx, pushedByMovingBlocky);

   }
}
bool Movable_Object::checkSlopeDL(gameMap &map,
                  sf::Vector2f cornerC1,
                  sf::Vector2f cornerC1xy)
{
    return (cornerC1.y - map.getTile(0).getTileDims().y*cornerC1xy.y >
       cornerC1.x - map.getTile(0).getTileDims().x*cornerC1xy.x);
}
bool Movable_Object::checkSlopeDR(gameMap &map,
                  sf::Vector2f cornerC2,
                  sf::Vector2f cornerC2xy)
{
    return (cornerC2.y - map.getTile(0).getTileDims().y*cornerC2xy.y >
       map.getTile(0).getTileDims().x*(cornerC2xy.x+1)-cornerC2.x);
}
bool Movable_Object::checkSlopeUL(gameMap &map,
                  sf::Vector2f cornerC1,
                  sf::Vector2f cornerC1xy)
{
    return (cornerC1.y - map.getTile(0).getTileDims().y*cornerC1xy.y <
       map.getTile(0).getTileDims().x*(cornerC1xy.x+1)-cornerC1.x);
}
bool Movable_Object::checkSlopeUR(gameMap &map,
                  sf::Vector2f cornerC2,
                  sf::Vector2f cornerC2xy)
{
    return (cornerC2.y - map.getTile(0).getTileDims().y*cornerC2xy.y <
       cornerC2.x - map.getTile(0).getTileDims().x*cornerC2xy.x);
}
void Movable_Object::getCornerBeforeMovement(gameMap &map,
                                        sf::Vector2i &cornerType,
                                        sf::Vector2f &corner,
                                        sf::Vector2f &cornerxy,
                                        Corner C1)
{
    sf::Vector2f temp = getPosition();
    sf::Vector2f temp2;
    setPosition(sf::Vector2f(xBeforeMovement, yBeforeMovement));
    cornerType = getCornerType(C1,C1, map, cornerxy, temp2);
    corner = getCorner(C1);
    setPosition(temp);
}

void Movable_Object::placeObjectOnSlope(gameMap &map, std::string &dir, std::string C1dir,
                                   std::string C2dir, sf::Vector2i corners,
                                   sf::Vector2f cornerC1, sf::Vector2f cornerC2,
                                   sf::Vector2f cornerC1xy, sf::Vector2f cornerC2xy,
                                   float &yOutput,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
        if(C1dir == "DL" && map.getTileType(corners.x).getSlanted()==C1dir &&
       checkSlopeDL(map, cornerC1, cornerC1xy))
    {
        sf::Vector2f beforeCorner, beforeCornerxy;
        sf::Vector2i beforeCornerType;
        getCornerBeforeMovement(map,beforeCornerType,
                                beforeCorner, beforeCornerxy,
                                 CORNER_DOWNLEFT);
        if(!map.getTileType(corners.x).getCloud() ||
           ((beforeCornerxy.x == cornerC1xy.x  && beforeCornerxy.y == cornerC1xy.y &&
                      (!checkSlopeDL(map, beforeCorner, beforeCornerxy)) ||
                      beforeCornerxy.x > cornerC1xy.x ||
                      beforeCornerxy.y < cornerC1xy.y)))
        {
        setVelocityY(0);
        dir = C1dir;
        yOutput =  map.getTile(0).getTileDims().y*cornerC1xy.y+cornerC1.x -
                                 map.getTile(0).getTileDims().x*cornerC1xy.x-getDimensions().y/2;
        checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
        }
    }
        if(C2dir == "DR" && map.getTileType(corners.y).getSlanted()==C2dir &&
       checkSlopeDR(map, cornerC2, cornerC2xy))
    {
            sf::Vector2f beforeCorner, beforeCornerxy;
                sf::Vector2i beforeCornerType;
        getCornerBeforeMovement(map,beforeCornerType,
                                beforeCorner, beforeCornerxy, CORNER_DOWNRIGHT);
        if(!map.getTileType(corners.y).getCloud() ||
           (((beforeCornerxy.x == cornerC2xy.x  && beforeCornerxy.y == cornerC2xy.y &&
                      !checkSlopeDR(map, beforeCorner, beforeCornerxy)) ||
                      beforeCornerxy.x <cornerC2xy.x ||
                      beforeCornerxy.y <cornerC2xy.y)))
        {
        setVelocityY(0);
        dir = C2dir;
        yOutput= map.getTile(0).getTileDims().y*cornerC2xy.y+
                                 map.getTile(0).getTileDims().x*(cornerC2xy.x+1)-
                                 cornerC2.x-getDimensions().y/2;
   checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
        }
    }
    if(C1dir == "UL" && map.getTileType(corners.x).getSlanted()==C1dir &&
        checkSlopeUL(map, cornerC1, cornerC1xy))
    {
        setVelocityY(0);
        dir = C1dir;
        yOutput= map.getTile(0).getTileDims().y*cornerC1xy.y+
                                 map.getTile(0).getTileDims().x*(cornerC1xy.x+1)-
                                 cornerC1.x+getDimensions().y/2;

    checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
    }
        if(C2dir == "UR" && map.getTileType(corners.y).getSlanted()==C2dir &&
       checkSlopeUR(map, cornerC2, cornerC2xy))
       {
           setVelocityY(0);
           dir = C2dir;
           yOutput =  map.getTile(0).getTileDims().y*cornerC2xy.y+cornerC2.x -
                                 map.getTile(0).getTileDims().x*cornerC2xy.x+getDimensions().y/2;

       checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
       }


}

void Movable_Object::checkIfBetweenSlopedAndBlock(gameMap &map, std::string dir,
                                             sf::Vector2i &downCorner,
                                             sf::Vector2i &upCorner,
                                             sf::Vector2f &cornerDLxy,
                                             sf::Vector2f &cornerDRxy,
                                             sf::Vector2f &cornerULxy,
                                             sf::Vector2f &cornerURxy,
                                             sf::Vector2f &cornerDL,
                                             sf::Vector2f &cornerDR,
                                             sf::Vector2f &cornerUL,
                                             sf::Vector2f &cornerUR,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky )
{
        if((dir == "DL" || dir == "DR") && (!map.getTileType(upCorner.x).getWalkable()||
       !map.getTileType(upCorner.y).getWalkable() ||
       ((map.getTileType(upCorner.x).getSlanted() == "DL" ||
         map.getTileType(upCorner.x).getSlanted() == "DR" ||
         map.getTileType(upCorner.y).getSlanted() == "DL" ||
         map.getTileType(upCorner.y).getSlanted() == "DR" ||
         (dir == "DL" && map.getTileType(upCorner.x).getSlanted() == "UR" &&
          (cornerULxy.x != cornerURxy.x || checkSlopeUR(map, cornerUR, cornerURxy))) ||
         (dir == "DR" && map.getTileType(upCorner.y).getSlanted() == "UL" &&
          (cornerULxy.x != cornerURxy.x || checkSlopeUL(map, cornerUL, cornerULxy))) ) &&
         cornerULxy.y < cornerDLxy.y)))
          {
        if(dir == "DL")
        {
        setPosition(sf::Vector2f( map.getTile(0).getTileDims().x*cornerULxy.x +
                                 getDimensions().x/2 + getDimensions().y,
                                 map.getTile(0).getTileDims().y*cornerDRxy.y +
                                 getDimensions().y/2));
        checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
        }else if(dir == "DR")
        {
            setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerDRxy.x+1) -
                                 getDimensions().x/2 - getDimensions().y,
                                 map.getTile(0).getTileDims().y*cornerDRxy.y +
                                 getDimensions().y/2));
        checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
        }
    }
            if((dir == "UL" || dir == "UR") && (!map.getTileType(downCorner.x).getWalkable()||
       !map.getTileType(downCorner.y).getWalkable() ||
       ((map.getTileType(downCorner.x).getSlanted() == "UL" ||
         map.getTileType(downCorner.x).getSlanted() == "UR" ||
         map.getTileType(downCorner.y).getSlanted() == "UL" ||
         map.getTileType(downCorner.y).getSlanted() == "UR"||
         (dir == "UL" && map.getTileType(downCorner.x).getSlanted() == "DR" &&
          (cornerULxy.x != cornerURxy.x ||checkSlopeDR(map, cornerDR, cornerDRxy))) ||
         (dir == "UR" && map.getTileType(downCorner.y).getSlanted() == "DL" &&
          (cornerULxy.x != cornerURxy.x || checkSlopeDL(map, cornerDL, cornerDLxy)))) &&
         cornerULxy.y < cornerDLxy.y)))
          {
        if(dir == "UL")
        {
        setPosition(sf::Vector2f( map.getTile(0).getTileDims().x*cornerULxy.x +
                                 getDimensions().x/2 + getDimensions().y,
                                 map.getTile(0).getTileDims().y*(cornerULxy.y+1) -
                                 getDimensions().y/2));
        checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
        }else if(dir == "UR")
        {
            setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerDRxy.x+1) -
                                 getDimensions().x/2 - getDimensions().y,
                                 map.getTile(0).getTileDims().y*(cornerULxy.y+1) -
                                 getDimensions().y/2));
        checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
        }
    }

}

void Movable_Object::checkForSideConcave(gameMap &map,
                                             sf::Vector2i &downCorner,
                                             sf::Vector2i &upCorner,
                                             sf::Vector2f &cornerDLxy,
                                             sf::Vector2f &cornerDRxy,
                                             sf::Vector2f &cornerULxy,
                                             sf::Vector2f &cornerURxy,
                                             sf::Vector2f &cornerDL,
                                             sf::Vector2f &cornerDR,
                                             sf::Vector2f &cornerUL,
                                             sf::Vector2f &cornerUR,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
    if((map.getTileType(downCorner.x).getSlanted() == "DL" &&
        map.getTileType(upCorner.x).getSlanted() == "UL" &&
        (checkSlopeDL(map, cornerDL, cornerDLxy) ||
         (checkSlopeUL(map, cornerUL, cornerULxy) ) )))
       {
           setPosition(sf::Vector2f((cornerULxy.x)*map.getTile(0).getTileDims().x+
                                    getDimensions().x/2 + getDimensions().y/2,
                                    (cornerULxy.y+1)*map.getTile(0).getTileDims().y));
               checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
       }
         if((map.getTileType(downCorner.x).getSlanted() == "DR" &&
        map.getTileType(upCorner.x).getSlanted() == "UR" &&
        (checkSlopeDR(map, cornerDR, cornerDRxy) ||
         (checkSlopeUR(map, cornerUR, cornerURxy) ) )))
       {
           setPosition(sf::Vector2f((cornerULxy.x+1)*map.getTile(0).getTileDims().x-
                                    getDimensions().x/2 - getDimensions().y/2,
                                    (cornerULxy.y+1)*map.getTile(0).getTileDims().y));
               checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
       }
}

void Movable_Object::checkIfMovedToTopOfSlope(gameMap &map, sf::Vector2i corners,
                                         int dir, sf::Vector2f cornerC1xy,
                                         sf::Vector2f cornerC2xy)
{
        std::string C1, C2;
    sf::Vector2f beforeCornerC1, beforeCornerC1xy, beforeCornerC2, beforeCornerC2xy;
        sf::Vector2i beforeCornerTypeC1, beforeCornerTypeC2;
    if(dir == 1)
    {
     C1 = "DR";
     C2 = "DL";
     getCornerBeforeMovement(map,beforeCornerTypeC1,
                                beforeCornerC1, beforeCornerC1xy,
                                 CORNER_DOWNRIGHT);
    getCornerBeforeMovement(map,beforeCornerTypeC2,
                                beforeCornerC2, beforeCornerC2xy,
                                 CORNER_DOWNLEFT);

    }else
    {
      C1 = "UR";
     C2 = "UL";

    }
     if((map.getTileType(corners.x).getSlanted()==C1 &&
         (!map.getTileType(corners.x).getCloud() || dir != 1 ||
          (beforeCornerC1xy.x == cornerC1xy.x &&
           beforeCornerC1xy.y == cornerC1xy.y &&
           !checkSlopeDR(map, beforeCornerC1, beforeCornerC1xy)))&&
       cornerC1xy.x < cornerC2xy.x && (getPosition().x-xBeforeMovement) >0) || (cornerC1xy.x < cornerC2xy.x &&
       (!map.getTileType(corners.y).getCloud() || dir != 1 ||
          (beforeCornerC2xy.x == cornerC2xy.x &&
           beforeCornerC2xy.y == cornerC2xy.y &&
           !checkSlopeDL(map, beforeCornerC2, beforeCornerC2xy)))&&
        map.getTileType(corners.y).getSlanted()==C2 && (getPosition().x-xBeforeMovement) <0))
    {
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC1xy.y+(1-dir)/2)
                                 -dir*getDimensions().y/2));
                setVelocityY(0);
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC1xy.y+(1-dir)/2)
                                 -dir*getDimensions().y/2));

    }
}

void Movable_Object::checkSidesOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                        sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                        sf::Vector2f cornerDRxy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
        if(((map.getTileType(downCorner.x).getSlanted()=="DR"
             && !map.getTileType(downCorner.x).getCloud()) ||
        (map.getTileType(upCorner.x).getSlanted()=="DR"
            && !map.getTileType(upCorner.x).getCloud())||
            (map.getTileType(downCorner.x).getSlanted()=="UR"
             && !map.getTileType(downCorner.x).getCloud()) ||
        (map.getTileType(upCorner.x).getSlanted()=="UR"
         && !map.getTileType(upCorner.x).getCloud()))&&
       cornerDLxy.x < cornerDRxy.x)
    {

        setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*cornerDRxy.x+getDimensions().x/2,getPosition().y));
    checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
    }
    if(cornerDLxy.x < cornerDRxy.x &&
       ((map.getTileType(downCorner.y).getSlanted()=="DL"
         && !map.getTileType(downCorner.y).getCloud())||
        (map.getTileType(upCorner.y).getSlanted()=="DL"
         && !map.getTileType(upCorner.y).getCloud())||
        (map.getTileType(downCorner.y).getSlanted()=="UL"
        && !map.getTileType(downCorner.y).getCloud()) ||
        (map.getTileType(upCorner.y).getSlanted()=="UL"
         && !map.getTileType(upCorner.y).getCloud())))
    {
        setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerDLxy.x+1)-getDimensions().x/2,getPosition().y));
    checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
    }
}

void Movable_Object::checkIfHitSideEdgeOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                                sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                                sf::Vector2f cornerURxy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{

    if(((map.getTileType(upCorner.x).getSlanted()=="DL"
         && !map.getTileType(upCorner.x).getCloud())||
        (map.getTileType(downCorner.x).getSlanted()=="UL"
         && !map.getTileType(downCorner.x).getCloud())) &&
       cornerDLxy.y > cornerURxy.y )
    {
        setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*
                                 (cornerURxy.x)+getDimensions().x/2,getPosition().y));
    checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
    }
    if(((map.getTileType(upCorner.y).getSlanted()=="DR"
         && !map.getTileType(upCorner.y).getCloud())||
        (map.getTileType(downCorner.y).getSlanted()=="UR"
         && !map.getTileType(downCorner.y).getCloud())) &&
       cornerDLxy.y > cornerURxy.y )
    {
        setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*
                                 (cornerURxy.x)-getDimensions().x/2,getPosition().y));
    checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
    }
}

void Movable_Object::checkIfHitTopBotEdgeOfSlopedBlock(gameMap &map, int dir,
                                                  sf::Vector2i corners,
                                                  sf::Vector2f cornerC1xy,
                                                  sf::Vector2f cornerC2xy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
    std::string C1, C2;
        sf::Vector2f beforeCornerC1, beforeCornerC1xy, beforeCornerC2, beforeCornerC2xy;
        sf::Vector2i beforeCornerTypeC1, beforeCornerTypeC2;
    if(dir ==1)
    {
        C1 = "DR";
        C2 = "DL";
        sf::Vector2f temp = getPosition();
        setPosition(sf::Vector2f(getPosition().x,yBeforeMovement));
        beforeCornerTypeC1 = getCornerType(CORNER_DOWNRIGHT, CORNER_DOWNLEFT, map, beforeCornerC1xy, beforeCornerC2xy);
setPosition(temp);
    }else
    {
        C1 = "UR";
        C2 = "UL";
    }

     if((map.getTileType(corners.x).getSlanted()==C1 &&
         (!map.getTileType(corners.x).getCloud() || (beforeCornerC1xy.y < cornerC1xy.y)) &&
       cornerC1xy.x < cornerC2xy.x) || (cornerC1xy.x < cornerC2xy.x &&
        (!map.getTileType(corners.y).getCloud() || (beforeCornerC2xy.y < cornerC2xy.y)) &&
       map.getTileType(corners.y).getSlanted()==C2))
       {
        setVelocityY(0);
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC2xy.y+(1-dir)/2)
                                 -dir*getDimensions().y/2));
    checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
    }
}

void Movable_Object::checkIfHitTopBotOfSlopedBlock(gameMap &map, int dir,
                                              sf::Vector2i corners,
                                              sf::Vector2f cornerC1xy,
                                              sf::Vector2f cornerC2xy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{
    std::string C1, C2;
    if(dir == 1)
    {
        C1 = "DL";
        C2 = "DR";
    }else
    {
        C1 = "UL";
        C2 = "UR";
    }
    if(
       (((map.getTileType(corners.x).getSlanted() == C1
          && !map.getTileType(corners.x).getCloud()) ||
         (map.getTileType(corners.x).getSlanted() == C2
          && !map.getTileType(corners.x).getCloud()) ||
         (map.getTileType(corners.y).getSlanted() == C1
          && !map.getTileType(corners.y).getCloud()) ||
         (map.getTileType(corners.y).getSlanted() == C2
          && !map.getTileType(corners.y).getCloud())
         ) &&
         cornerC1xy.y < cornerC2xy.y))
          {
              setVelocityY(0);
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC2xy.y)
                                 +dir*getDimensions().y/2));
          checkForDeath(map, alreadyCheckedx, pushedByMovingBlockx);
        checkForDeath(map, alreadyCheckedy, pushedByMovingBlocky);
          }
}

void Movable_Object::checkSlopedCorners(gameMap &map, bool checkLR,
                                        float &afterY,
                                        std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky)
{

    sf::Vector2i downCorner, upCorner;
    sf::Vector2f cornerDL, cornerDR, cornerUL, cornerUR;
    sf::Vector2f cornerDLxy, cornerDRxy, cornerULxy, cornerURxy;
        std::string dird, diru;
    Corner CDL = CORNER_DOWNLEFT;
    Corner CDR = CORNER_DOWNRIGHT;
    Corner CUL = CORNER_UPLEFT;
    Corner CUR = CORNER_UPRIGHT;
getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);


float yHold = getPosition().y;
sf::Vector2f temp;
float afterMove = getPosition().y;
float yCheck = getPosition().y;
if(!checkLR && getVelocity().y >0)
{
    checkIfMovedATileForSloped(map, dird, 1, yCheck, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);


}else if (!checkLR && getVelocity().y <0)
{
    checkIfMovedATileForSloped(map, diru, -1, yCheck, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
}
setPosition(sf::Vector2f(getPosition().x,yCheck));
getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);
if(afterMove == yCheck)
{

        placeObjectOnSlope(map, dird, "DL", "DR", downCorner,
                    cornerDL, cornerDR, cornerDLxy, cornerDRxy,
                    yHold, alreadyCheckedx, alreadyCheckedy,
                    pushedByMovingBlockx, pushedByMovingBlocky);
        placeObjectOnSlope(map, diru, "UL", "UR", upCorner,
                    cornerUL, cornerUR, cornerULxy, cornerURxy,
                    yHold, alreadyCheckedx, alreadyCheckedy,
                    pushedByMovingBlockx, pushedByMovingBlocky);

     setPosition(sf::Vector2f(getPosition().x,yHold));
}
    if(checkLR)
{
         getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);

checkIfBetweenSlopedAndBlock(map, dird, downCorner, upCorner, cornerDLxy,
                            cornerDRxy,cornerULxy, cornerURxy, cornerDL,
                             cornerDR, cornerUL, cornerUR,
                             alreadyCheckedx, alreadyCheckedy,
                    pushedByMovingBlockx, pushedByMovingBlocky);
checkIfBetweenSlopedAndBlock(map, diru, downCorner, upCorner, cornerDLxy,
                            cornerDRxy,cornerULxy, cornerURxy, cornerDL,
                             cornerDR, cornerUL, cornerUR,
                             alreadyCheckedx, alreadyCheckedy,
                    pushedByMovingBlockx, pushedByMovingBlocky);
     getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);

                   checkForSideConcave(map, downCorner, upCorner, cornerDLxy,
                            cornerDRxy,cornerULxy, cornerURxy, cornerDL,
                             cornerDR, cornerUL, cornerUR, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
     getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);

checkIfMovedToTopOfSlope(map, downCorner, 1, cornerDLxy, cornerDRxy);
checkIfMovedToTopOfSlope(map, upCorner, -1, cornerULxy, cornerURxy);
         getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);


   checkSidesOfSlopedBlock(map, downCorner, upCorner, cornerDLxy, cornerDRxy, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);

    checkIfHitSideEdgeOfSlopedBlock(map, downCorner, upCorner,  cornerDLxy, cornerURxy, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);

}

    if(!checkLR)
    {
   getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);

checkIfHitTopBotEdgeOfSlopedBlock(map,1 , downCorner, cornerDLxy, cornerDRxy, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
checkIfHitTopBotEdgeOfSlopedBlock(map,-1 , upCorner, cornerULxy, cornerURxy, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
checkIfHitTopBotOfSlopedBlock(map, 1, upCorner, cornerULxy, cornerDLxy, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);
checkIfHitTopBotOfSlopedBlock(map, -1, downCorner, cornerULxy, cornerDLxy, alreadyCheckedx,
                               alreadyCheckedy, pushedByMovingBlockx,
                               pushedByMovingBlocky);

    }
    afterY = getPosition().y;
    setPosition(sf::Vector2f(getPosition().x, afterMove));
}


