
#include <SFML/Graphics.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <exception>
#include "tileType.h"
#include "tile.h"
#include "ladderType.h"
#include "door.h"
#include "character.h"
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"
#include "moving_block.h"
#include "falling_block.h"
#include "loadMap.h"

Character::Character(sf::Vector2f p_position, sf::Vector2f p_dimensions,
                     float p_walkSpeed, float p_runSpeed,float p_swimSpeed,
                      int p_ssNum,
                     sf::Vector2u p_spriteLoc,sf::Vector2f p_jumpHeight,
                     sf::Vector2f p_swimJumpHeight,sf::Vector2f p_wallJumpPower ):
                         jumping(false), climbing(false), relativeGrav(1.0),
                          running(false), canEnterDoor(true), swimming(false),
                          normalRelGrav(1.0), waterGravEffects(0.2), health(4),
                          wallJumping(false), canWallJump(false), wallJumpFrameCounter(0),
                          wallJump(true), currentMap(0), grounded(false), frameJump(2)
{
    setWalkSpeed(p_walkSpeed);
    setRunSpeed(p_runSpeed);
    setSwimSpeed(p_swimSpeed);
    setPosition(p_position);
    setDimensions(p_dimensions);
    spriteDims = p_dimensions;
    setSSNum(p_ssNum);
    setSpriteLoc(p_spriteLoc);
    setJumpHeight(p_jumpHeight);
    setSwimJumpHeight(p_swimJumpHeight);
    setWallJumpPower(p_wallJumpPower);
    setPushingSpeed(p_runSpeed);
    setYBeforeMovement();
    setXBeforeMovement();
}
bool Character::getClimbing()
{
    return climbing;
}
void Character::setWallJump(bool p_wallJump)
{
    wallJump = p_wallJump;
}
float Character::getRunSpeed()
{
    return runSpeed;
}
bool Character::getWallJump()
{
    return wallJump;
}
void Character::setNormalRelGrav(float p_normRelGra)
{
    normalRelGrav = p_normRelGra;
}
float Character::getNormalRelGrav()
{
    return normalRelGrav;
}
void Character::resetRelGrav()
{
    relativeGrav = normalRelGrav;
}

//Sets the character's speed in pixels/frame
void Character::setWalkSpeed(float p_speed)
{
    walkSpeed = p_speed;
}
void Character::setRunSpeed(float p_speed)
{
    runSpeed = p_speed;
}
void Character::setSwimSpeed(float p_speed)
{
    swimSpeed = p_speed;
}


//Gets the speed of the character in pixels/frame
float Character::getSpeed()
{
    if(running && ! swimming)
    {
        return runSpeed;
    }else if(swimming)
    {
        return swimSpeed;
    }else
    {
    return walkSpeed;
    }
}


    void Character::setLeft(bool p_left)
    {
        left = p_left;
    }
    void Character::setRight(bool p_right)
    {
        right = p_right;
    }
    void Character::setUp(bool p_up)
    {
        up = p_up;
    }
    void Character::setDown(bool p_down)
    {
        down = p_down;
    }
    void Character::setSpace(bool p_space)
    {
        space=p_space;
    }
    void Character::setRun(bool p_run)
    {
        run = p_run;
    }


    int Character::getCurrentMap()
    {
        return currentMap;
    }
    void Character::setCurrentMap(int p_currentMap)
    {
        currentMap = p_currentMap;
    }

//Updates the character based on keypresses, gravity, etc
void Character::update(std::vector<gameMap> &allMaps,
                       std::vector< std::vector< std::vector < sf::Sprite > > > &spritesToDraw,
                       std::vector<sf::Texture> &textureSheet,
                       std::vector<Character> & playerSet)
{
    gameMap & map = allMaps[currentMap];
    //frameJump is the ammout of frames after you start falling that you
    //can still jump
float yAfterSloped = getPosition().y;
    setYBeforeMovement();
    setXBeforeMovement();

    //A temporary holder
    sf::Vector2f posc1, posc2,posForCloud;

    //New Pos is the position we move to after everything is said and done
    sf::Vector2f newPos(getPosition().x, getPosition().y);

    //Cannot run and climb at the same time
    checkForRunning();

    //First, we check for ladders, short circuit by only checking
    //if you are holding up/down and are not climbing
    if(!climbing && (up||down))
    {
        checkForLadders(map, newPos, playerSet);
    }

    //Checks if you are trying to jump while underwater
    jumpWhileSwimmingCheck(map);


    //This section checks if you are currently on a ladder and all corners
    //are walkable (then you can jump). If any corner is unwalkable, then
    //we can no longer jump ( since we are inside a block)
    checkIfClimbingAndInsideBlock(map);


    //If we need to move either left of right, or there is momentum
    if( (left ^ right) || fabs(getVelocity().x) > 0.01)
    {
        //if you are holding left
        if(left)
        {
            holdingLeft(map);
        }

        //if you are holding right
        if(right)
        {
            holdingRight(map);
        }


        //If there is some velocity to the left
        if(getVelocity().x<0)
        {
            movingLeft(map, yAfterSloped);
        }else if(getVelocity().x>0)
        {
           movingRight(map, yAfterSloped);
        }
    checkXCollisionsWithMovableObjects(map, allMaps, playerSet, map.getAllObjects(), grounded, "Movable Objects");
   checkXCollisionsWithMovableObjects(map, allMaps, playerSet, playerSet, false, "characters");
    checkXCollisionsWithMovableObjects(map, allMaps, playerSet, map.getFallingBlocks(), false, "Falling Blocks");

    }else
    {
        //If there's very low velocity and neither key is down, set it to zero
        setVelocityX(0);
    }
     checkXCollisionsWithMovableObjects(map, allMaps, playerSet, map.getMovingBlocks(), false, "Moving Blocks");
setPosition(sf::Vector2f(getPosition().x, yAfterSloped));
    //BEGIN Y DIRECTION CALCS

    //
    float gravEffects = copysign(1,map.getGravity())*copysign(1,relativeGrav);
    float gravForce= map.getGravity()*relativeGrav;
//setYBeforeMovement();
//setXBeforeMovement();
varJumpHeight(gravEffects, gravForce);

gravityEffects(map, playerSet, gravEffects, gravForce);

    //Check if up, down or momentum
    if( (up ^ down) || fabs(getVelocity().y) > 0.01)
    {

        //This buffer let's you jump a few frames after starting to fall
        if(fabs(getVelocity().y) > fabs(gravForce)*(frameJump+0.1) && !swimming)
        {
            canJump= false;
            grounded = false;
        }

        //If it's a top down game, you can move left and right
        checkIfUDHeld(map);

        //Same as before but moving up
        if(getVelocity().y<0)
        {
            checkUp(map, playerSet, gravEffects, gravForce);
        }

        //Same as up put for down
        if(getVelocity().y>0)
        {
           checkDown(map, playerSet, gravEffects, gravForce);
        }
            checkSlopedCorners(map,false,yAfterSloped);
            setPosition(sf::Vector2f(getPosition().x, yAfterSloped));
            checkSlopedCorners(map,false, yAfterSloped);
            setPosition(sf::Vector2f(getPosition().x, yAfterSloped));


 checkYCollisionsWithMovableObjects(map, allMaps, playerSet);
  checkYCollisionsWithCharacters(map, allMaps, playerSet);
  checkYCollisionsWithFallingBlocks(map, allMaps, playerSet);
      }else{
     setVelocityY(0);
    }
    checkYCollisionsWithMovingBlocks(map, allMaps, playerSet);
checkIfInASlope(map);
    if(!wallJumping)
    {
        setVelocityX(0);
    }
    //If you're climbing, you don't fall
    if(!map.getSideScroll() || climbing)
    {
        setVelocityY(0);
    }
    //Check for doors, must release up to check again.
    if(up && canEnterDoor)
    {
        canEnterDoor = false;
        checkForDoor(allMaps, currentMap, spritesToDraw, textureSheet);
    }else if(!up && ! canEnterDoor)
    {
        canEnterDoor = true;
    }

       setYBeforeMovement();
    setXBeforeMovement();
   sprite.setPosition(sf::Vector2f(round(getPosition().x),round(getPosition().y)));
}



//Set the jump height
//x is impulse
//y is frame buffer
void Character::setJumpHeight(sf::Vector2f p_jumpHeight)
{
    jumpHeight=p_jumpHeight;
}

//Return the jumpHeight
sf::Vector2f Character::getJumpHeight()
{
    if(swimming)
    {
        return getSwimJumpHeight();
    }else{
    return jumpHeight;
    }
}

void Character::setSwimJumpHeight(sf::Vector2f p_swimJumpHeight)
{
    swimJumpHeight=p_swimJumpHeight;
}

//Return the jumpHeight
sf::Vector2f Character::getSwimJumpHeight()
{
    return swimJumpHeight;
}


//Functions that checks for ladders in all four corners and climbs
//if you xan
void Character::checkForLadders(gameMap &map, sf::Vector2f &newPos, std::vector<Character> & playerSet)
{
    bool atLeastOneLadder = false;
    bool col = false;
    bool ladder[4] = {false};
    sf::Vector2i corners[2];
    sf::Vector2f cornersTemp[4];
    int ladderType[4] = {0};
    corners[0] = getCornerType(CORNER_UPLEFT,CORNER_DOWNLEFT,map,cornersTemp[0], cornersTemp[1]);
    corners[1] = getCornerType(CORNER_UPRIGHT,CORNER_DOWNRIGHT,map,cornersTemp[2], cornersTemp[3]);
    //Check each corner for a ladder
    if(map.getTileType(corners[0].x).getLadder()>0)
    {
        ladder[0] = true;
        ladderType[0] = map.getTileType(corners[0].x).getLadder();
        atLeastOneLadder = true;
    }
    if(map.getTileType(corners[0].y).getLadder()>0)
    {
        ladder[1] = true;
        ladderType[1] = map.getTileType(corners[0].y).getLadder();
        atLeastOneLadder = true;
    }
    if(map.getTileType(corners[1].x).getLadder()>0)
    {
        ladder[2] = true;
        ladderType[2] = map.getTileType(corners[1].x).getLadder();
        atLeastOneLadder = true;;
    }
    if(map.getTileType(corners[1].y).getLadder()>0)
    {
        ladder[3] = true;
        ladderType[3] = map.getTileType(corners[1].y).getLadder();
        atLeastOneLadder = true;
    }
float tempx = getPosition().x;
    //if there's a ladder in the corner, we check to see if we can climb
    if(atLeastOneLadder)
    {
        for(int cornerNum = 0; cornerNum<4; cornerNum++)
        {
            float ladderCenter = cornersTemp[cornerNum].x*map.getTile(0).getTileDims().x+map.getLadderType(ladderType[cornerNum]).getCenter();
            float ladderWidth =map.getLadderType(ladderType[cornerNum]).getWidth()/2;

            //If we are within range of a ladder, climb it!
            if(ladder[cornerNum]&& getPosition().x <= ladderCenter + ladderWidth + getDimensions().x/2 && getPosition().x >= ladderCenter - ladderWidth -getDimensions().x/2)
            {

                newPos.x = ladderCenter;
                setPosition(newPos);
                for(int charNum =0 ; charNum < playerSet.size();charNum++)
                {
                    if(checkForCollision(playerSet[charNum]))
                    {
                        col = true;
                    }
                }
                for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
                {
                    if(checkForCollision(map.getAllObjects()[obNum]))
                    {
                        col = true;
                    }
                }
                if(col)
                {
                    newPos.x = tempx;
                    setPosition(newPos);
                }else
                {
                               climbing = true;
                jumping = false;
                }
                break;
            }
        }
    }
}
void Character::checkForDoor(std::vector<gameMap> &allMaps, int &currentMap,std::vector< std::vector< std::vector < sf::Sprite > > > &spritesToDraw,std::vector<sf::Texture> &textureSheet)
{

    gameMap & map = allMaps[currentMap];

    //Get all the corners
    sf::Vector2i corners[2];
    sf::Vector2f cornersTemp[4];
    corners[0] = getCornerType(CORNER_UPLEFT,CORNER_DOWNLEFT,map,cornersTemp[0], cornersTemp[1]);
    corners[1] = getCornerType(CORNER_UPRIGHT,CORNER_DOWNRIGHT,map,cornersTemp[2], cornersTemp[3]);

    //If you are completely in a door tile and it's the same door
    if(map.getTileType(corners[1].y).getDoor()&&
       map.getTileType(corners[1].x).getDoor()&&
       map.getTileType(corners[0].y).getDoor()&&
       map.getTileType(corners[0].x).getDoor()&&
       cornersTemp[0].x==cornersTemp[1].x&&
       cornersTemp[1].x == cornersTemp[2].x&&
       cornersTemp[2].x == cornersTemp[3].x&&
       cornersTemp[0].y==cornersTemp[1].y&&
       cornersTemp[1].y == cornersTemp[2].y &&
       cornersTemp[2].y == cornersTemp[3].y
       )
    {
        //we can enter the door
        Door & doorToEnter = map.getDoor(cornersTemp[0].x,cornersTemp[0].y);
      //if we have already loaded the map, don't do it again
      if(doorToEnter.getAlreadyLoaded())
      {
          currentMap = doorToEnter.getNewDoorMapTypeNum();
          setPosition(sf::Vector2f((doorToEnter.getNewDoorLoc().x+0.5)*(allMaps[currentMap].getTile(0).getTileDims().x),(doorToEnter.getNewDoorLoc().y+1)*(allMaps[currentMap].getTile(0).getTileDims().x)-getDimensions().y/2));
      }else{
          //Load the new map and set position
          currentMap = allMaps.size();
          loadGameMap(doorToEnter.getMapType(),allMaps);
          setPosition(sf::Vector2f((doorToEnter.getNewDoorLoc().x+0.5)*(allMaps[currentMap].getTile(0).getTileDims().x),((doorToEnter).getNewDoorLoc().y+1)*(allMaps[currentMap].getTile(0).getTileDims().x)-getDimensions().y/2));
          spritesToDraw.push_back(updateMapSprites(allMaps[currentMap],textureSheet));
          //check to see if any of the new doors link to the old map
              for(int mapNum = 0; mapNum < (allMaps.size());mapNum++)
              {

                for(int doorNum = 0;doorNum < allMaps[mapNum].getFullDoorSet().size();doorNum++)
          {
                              for(int mapNum2 = 0; mapNum2 < (allMaps.size());mapNum2++)
              {

                  if((allMaps[mapNum].getFullDoorSet())[doorNum].getMapType()==allMaps[mapNum2].getLoadedFrom()&&!(allMaps[mapNum].getFullDoorSet())[doorNum].getAlreadyLoaded())
                  {
                      (allMaps[mapNum].getFullDoorSet())[doorNum].setAlreadyLoaded(true);
                      (allMaps[mapNum].getFullDoorSet())[doorNum].setNewDoorMapTypeNum(mapNum2);
                      allMaps[mapNum].getDoor((allMaps[mapNum].getFullDoorSet())[doorNum].getDoorLoc().x,
                                                     (allMaps[mapNum].getFullDoorSet())[doorNum].getDoorLoc().y).setAlreadyLoaded(true);
                                            allMaps[mapNum].getDoor((allMaps[mapNum].getFullDoorSet())[doorNum].getDoorLoc().x,
                                                     (allMaps[mapNum].getFullDoorSet())[doorNum].getDoorLoc().y).setNewDoorMapTypeNum(mapNum2);
                  }
              }
          }
        }
      }
    }
}
void Character::checkForSwimming(gameMap &map)
{
        sf::Vector2i corners[2];
    sf::Vector2f cornersTemp[4];
    corners[0] = getCornerType(CORNER_UPLEFT,CORNER_DOWNLEFT,map,cornersTemp[0], cornersTemp[1]);
    corners[1] = getCornerType(CORNER_UPRIGHT,CORNER_DOWNRIGHT,map,cornersTemp[2], cornersTemp[3]);

    if(map.getTileType(corners[0].x).getWater()||
       map.getTileType(corners[0].y).getWater()||
       map.getTileType(corners[1].x).getWater()||
       map.getTileType(corners[1].y).getWater())
    {
        swimming = true;
        relativeGrav = waterGravEffects;
    }else
    {
        swimming = false;
        resetRelGrav();
    }

}

    int Character::getHealth()
    {
        return health;
    }
    void Character::changeHealth(int amount)
    {
        health+=amount;
    }
    void Character::setHealth(int hp)
    {
        health = hp;
    }
sf::Vector2f Character::getWallJumpPower()
{
    return wallJumpPower;
}
void Character::setWallJumpPower(sf::Vector2f p_wallJumpPower)
{
    wallJumpPower = p_wallJumpPower;
}
void Character::setClimbing(bool p_climbing)
{
    climbing = p_climbing;
}
void Character::death()
{
    setPosition(sf::Vector2f(100,100));
    setVelocityX(0);
    setVelocityY(0);
}

void Character::getAllCorners(gameMap & map,sf::Vector2f &cornerDL, sf::Vector2f &cornerDR,
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

void Character::checkIfMovedATileForSloped(gameMap & map, std::string &dir,
                                            int dirToCheck, float & yOutput)
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
setPosition(sf::Vector2f(getPosition().x,getPosition().y-getVelocity().y));
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
                    yOutput);

   }
}
bool Character::checkSlopeDL(gameMap &map,
                  sf::Vector2f cornerC1,
                  sf::Vector2f cornerC1xy)
{
    return (cornerC1.y - map.getTile(0).getTileDims().y*cornerC1xy.y >
       cornerC1.x - map.getTile(0).getTileDims().x*cornerC1xy.x);
}
bool Character::checkSlopeDR(gameMap &map,
                  sf::Vector2f cornerC2,
                  sf::Vector2f cornerC2xy)
{
    return (cornerC2.y - map.getTile(0).getTileDims().y*cornerC2xy.y >
       map.getTile(0).getTileDims().x*(cornerC2xy.x+1)-cornerC2.x);
}
bool Character::checkSlopeUL(gameMap &map,
                  sf::Vector2f cornerC1,
                  sf::Vector2f cornerC1xy)
{
    return (cornerC1.y - map.getTile(0).getTileDims().y*cornerC1xy.y <
       map.getTile(0).getTileDims().x*(cornerC1xy.x+1)-cornerC1.x);
}
bool Character::checkSlopeUR(gameMap &map,
                  sf::Vector2f cornerC2,
                  sf::Vector2f cornerC2xy)
{
    return (cornerC2.y - map.getTile(0).getTileDims().y*cornerC2xy.y <
       cornerC2.x - map.getTile(0).getTileDims().x*cornerC2xy.x);
}

void Character::getCornerBeforeMovement(gameMap &map,
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

void Character::checkIfInASlope(gameMap &map)
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
           (!down && (beforeCornerxy.x == cornerDLxy.x  && beforeCornerxy.y == cornerDLxy.y &&
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
           (!down && ((beforeCornerxy.x == cornerDRxy.x  && beforeCornerxy.y == cornerDRxy.y &&
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
void Character::placeObjectOnSlope(gameMap &map, std::string &dir, std::string C1dir,
                                   std::string C2dir, sf::Vector2i corners,
                                   sf::Vector2f cornerC1, sf::Vector2f cornerC2,
                                   sf::Vector2f cornerC1xy, sf::Vector2f cornerC2xy,
                                   float &yOutput )
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
           (!down && (beforeCornerxy.x == cornerC1xy.x  && beforeCornerxy.y == cornerC1xy.y &&
                      (!checkSlopeDL(map, beforeCorner, beforeCornerxy)) ||
                      beforeCornerxy.x > cornerC1xy.x ||
                      beforeCornerxy.y < cornerC1xy.y)))
        {
        dir = C1dir;
        hitTheFloor();
        yOutput =  map.getTile(0).getTileDims().y*cornerC1xy.y+cornerC1.x -
                                 map.getTile(0).getTileDims().x*cornerC1xy.x-getDimensions().y/2;
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
           (!down && ((beforeCornerxy.x == cornerC2xy.x  && beforeCornerxy.y == cornerC2xy.y &&
                      !checkSlopeDR(map, beforeCorner, beforeCornerxy)) ||
                      beforeCornerxy.x <cornerC2xy.x ||
                      beforeCornerxy.y <cornerC2xy.y)))
        {


        dir = C2dir;
        hitTheFloor();
        yOutput= map.getTile(0).getTileDims().y*cornerC2xy.y+
                                 map.getTile(0).getTileDims().x*(cornerC2xy.x+1)-
                                 cornerC2.x-getDimensions().y/2;
        }
    }
    if(C1dir == "UL" && map.getTileType(corners.x).getSlanted()==C1dir &&
        checkSlopeUL(map, cornerC1, cornerC1xy))
    {
        dir = C1dir;
        hitTheCeiling();
        yOutput= map.getTile(0).getTileDims().y*cornerC1xy.y+
                                 map.getTile(0).getTileDims().x*(cornerC1xy.x+1)-
                                 cornerC1.x+getDimensions().y/2;
    }
        if(C2dir == "UR" && map.getTileType(corners.y).getSlanted()==C2dir &&
       checkSlopeUR(map, cornerC2, cornerC2xy))
       {
           dir = C2dir;
          hitTheCeiling();
           yOutput =  map.getTile(0).getTileDims().y*cornerC2xy.y+cornerC2.x -
                                 map.getTile(0).getTileDims().x*cornerC2xy.x+getDimensions().y/2;
       }


}

void Character::checkIfBetweenSlopedAndBlock(gameMap &map, std::string dir,
                                             sf::Vector2i &downCorner,
                                             sf::Vector2i &upCorner,
                                             sf::Vector2f &cornerDLxy,
                                             sf::Vector2f &cornerDRxy,
                                             sf::Vector2f &cornerULxy,
                                             sf::Vector2f &cornerURxy,
                                             sf::Vector2f &cornerDL,
                                             sf::Vector2f &cornerDR,
                                             sf::Vector2f &cornerUL,
                                             sf::Vector2f &cornerUR)
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
        }else if(dir == "DR")
        {


            setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerDRxy.x+1) -
                                 getDimensions().x/2 - getDimensions().y,
                                 map.getTile(0).getTileDims().y*cornerDRxy.y +
                                 getDimensions().y/2));
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
        }else if(dir == "UR")
        {
            setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*(cornerDRxy.x+1) -
                                 getDimensions().x/2 - getDimensions().y,
                                 map.getTile(0).getTileDims().y*(cornerULxy.y+1) -
                                 getDimensions().y/2));
        }
    }

}

void Character::checkForSideConcave(gameMap &map,
                                             sf::Vector2i &downCorner,
                                             sf::Vector2i &upCorner,
                                             sf::Vector2f &cornerDLxy,
                                             sf::Vector2f &cornerDRxy,
                                             sf::Vector2f &cornerULxy,
                                             sf::Vector2f &cornerURxy,
                                             sf::Vector2f &cornerDL,
                                             sf::Vector2f &cornerDR,
                                             sf::Vector2f &cornerUL,
                                             sf::Vector2f &cornerUR)
{
    if((map.getTileType(downCorner.x).getSlanted() == "DL" &&
        map.getTileType(upCorner.x).getSlanted() == "UL" &&
        (checkSlopeDL(map, cornerDL, cornerDLxy) ||
         (checkSlopeUL(map, cornerUL, cornerULxy) ) )))
       {
           setPosition(sf::Vector2f((cornerULxy.x)*map.getTile(0).getTileDims().x+
                                    getDimensions().x/2 + getDimensions().y/2,
                                    (cornerULxy.y+1)*map.getTile(0).getTileDims().y));
       }
         if((map.getTileType(downCorner.x).getSlanted() == "DR" &&
        map.getTileType(upCorner.x).getSlanted() == "UR" &&
        (checkSlopeDR(map, cornerDR, cornerDRxy) ||
         (checkSlopeUR(map, cornerUR, cornerURxy) ) )))
       {
           setPosition(sf::Vector2f((cornerULxy.x+1)*map.getTile(0).getTileDims().x-
                                    getDimensions().x/2 - getDimensions().y/2,
                                    (cornerULxy.y+1)*map.getTile(0).getTileDims().y));
       }
}

void Character::checkIfMovedToTopOfSlope(gameMap &map, sf::Vector2i corners,
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
       cornerC1xy.x < cornerC2xy.x && getVelocity().x >0) || (cornerC1xy.x < cornerC2xy.x &&
       (!map.getTileType(corners.y).getCloud() || dir != 1 ||
          (beforeCornerC2xy.x == cornerC2xy.x &&
           beforeCornerC2xy.y == cornerC2xy.y &&
           !checkSlopeDL(map, beforeCornerC2, beforeCornerC2xy)))&&
        map.getTileType(corners.y).getSlanted()==C2 && getVelocity().x <0))
    {
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC1xy.y+(1-dir)/2)
                                 -dir*getDimensions().y/2));
if(dir == 1)
{
hitTheFloor();
}else{
 hitTheCeiling();
}
    }
}

void Character::checkSidesOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                        sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                        sf::Vector2f cornerDRxy)
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
hitAWall();
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
        hitAWall();
    }
}

void Character::checkIfHitSideEdgeOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                                sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                                sf::Vector2f cornerURxy)
{

    if(((map.getTileType(upCorner.x).getSlanted()=="DL"
         && !map.getTileType(upCorner.x).getCloud())||
        (map.getTileType(downCorner.x).getSlanted()=="UL"
         && !map.getTileType(downCorner.x).getCloud())) &&
       cornerDLxy.y > cornerURxy.y )
    {
        setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*
                                 (cornerURxy.x)+getDimensions().x/2,getPosition().y));
    }
    if(((map.getTileType(upCorner.y).getSlanted()=="DR"
         && !map.getTileType(upCorner.y).getCloud())||
        (map.getTileType(downCorner.y).getSlanted()=="UR"
         && !map.getTileType(downCorner.y).getCloud())) &&
       cornerDLxy.y > cornerURxy.y )
    {
        setPosition(sf::Vector2f(map.getTile(0).getTileDims().x*
                                 (cornerURxy.x)-getDimensions().x/2,getPosition().y));
    }
}

void Character::checkIfHitTopBotEdgeOfSlopedBlock(gameMap &map, int dir,
                                                  sf::Vector2i corners,
                                                  sf::Vector2f cornerC1xy,
                                                  sf::Vector2f cornerC2xy)
{
    std::string C1, C2;
        sf::Vector2f beforeCornerC1, beforeCornerC1xy, beforeCornerC2, beforeCornerC2xy;
        sf::Vector2i beforeCornerTypeC1, beforeCornerTypeC2;
    if(dir ==1)
    {
        C1 = "DR";
        C2 = "DL";
        sf::Vector2f temp = getPosition();
        setPosition(sf::Vector2f(getPosition().x,getPosition().y-getVelocity().y));
        beforeCornerTypeC1 = getCornerType(CORNER_DOWNRIGHT, CORNER_DOWNLEFT, map, beforeCornerC1xy, beforeCornerC2xy);
setPosition(temp);
    }else
    {
        C1 = "UR";
        C2 = "UL";
    }

     if((map.getTileType(corners.x).getSlanted()==C1 &&
         (!map.getTileType(corners.x).getCloud() || (!down && beforeCornerC1xy.y < cornerC1xy.y)) &&
       cornerC1xy.x < cornerC2xy.x) || (cornerC1xy.x < cornerC2xy.x &&
        (!map.getTileType(corners.y).getCloud() || (!down&& beforeCornerC2xy.y < cornerC2xy.y)) &&
       map.getTileType(corners.y).getSlanted()==C2))
       {
        if(dir == 1)
        {
        hitTheFloor();
        }else
        {
            hitTheCeiling();
        }
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC2xy.y+(1-dir)/2)
                                 -dir*getDimensions().y/2));
    }
}

void Character::checkIfHitTopBotOfSlopedBlock(gameMap &map, int dir,
                                              sf::Vector2i corners,
                                              sf::Vector2f cornerC1xy,
                                              sf::Vector2f cornerC2xy)
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
              if(dir == 1)
              {
              hitTheCeiling();
              }else
              {
                  hitTheFloor();
              }
        setPosition(sf::Vector2f(getPosition().x,
                                 map.getTile(0).getTileDims().y*(cornerC2xy.y)
                                 +dir*getDimensions().y/2));
          }
}

void Character::checkSlopedCorners(gameMap &map, bool checkLR, float & afterY)
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
    checkIfMovedATileForSloped(map, dird, 1, yCheck);


}else if (!checkLR && getVelocity().y <0)
{
    checkIfMovedATileForSloped(map, diru, -1, yCheck);
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
                    yHold);
        placeObjectOnSlope(map, diru, "UL", "UR", upCorner,
                    cornerUL, cornerUR, cornerULxy, cornerURxy,
                    yHold);

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
                             cornerDR, cornerUL, cornerUR);
checkIfBetweenSlopedAndBlock(map, diru, downCorner, upCorner, cornerDLxy,
                            cornerDRxy,cornerULxy, cornerURxy, cornerDL,
                             cornerDR, cornerUL, cornerUR);
     getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);

                   checkForSideConcave(map, downCorner, upCorner, cornerDLxy,
                            cornerDRxy,cornerULxy, cornerURxy, cornerDL,
                             cornerDR, cornerUL, cornerUR);
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


   checkSidesOfSlopedBlock(map, downCorner, upCorner, cornerDLxy, cornerDRxy);

    checkIfHitSideEdgeOfSlopedBlock(map, downCorner, upCorner,  cornerDLxy, cornerURxy);

}

    if(!checkLR)
    {
   getAllCorners(map, cornerDL, cornerDR,
                   cornerUL, cornerUR,
                   cornerDLxy, cornerDRxy,
                   cornerULxy, cornerURxy,
                   downCorner, upCorner);

checkIfHitTopBotEdgeOfSlopedBlock(map,1 , downCorner, cornerDLxy, cornerDRxy);
checkIfHitTopBotEdgeOfSlopedBlock(map,-1 , upCorner, cornerULxy, cornerURxy);
checkIfHitTopBotOfSlopedBlock(map, 1, upCorner, cornerULxy, cornerDLxy);
checkIfHitTopBotOfSlopedBlock(map, -1, downCorner, cornerULxy, cornerDLxy);

    }
    afterY = getPosition().y;
    setPosition(sf::Vector2f(getPosition().x, afterMove));
}

void Character::hitTheFloor()
{
            jumping = false;
                canWallJump = false;
                wallJumping= false;
                grounded = true;
                if(getVelocity().y > 0)
                {
                setVelocityY(0);
                }
                if(!space)
                {
                    canJump = true;
                }
}

void Character::checkForRunning()
{
    if(run && !climbing)
    {
        running=true;
    }else{
        running = false;
    }
}

void Character::jumpWhileSwimmingCheck(gameMap &map)
{
    //Jump and swimming
    if(!space && swimming)
    {
        canJump = true;
    }
    if(swimming)
    {
        checkForSwimming(map);
        if(!swimming)
        {
            jumping = false;
            canJump= false;
            grounded = false;
        }
    }else
    {
        checkForSwimming(map);
    }
}

void Character::checkIfClimbingAndInsideBlock(gameMap &map)
{
    if(climbing)
    {
        sf::Vector2i corners[2];
        sf::Vector2f cornersTemp[4];
        corners[0] = getCornerType(CORNER_UPLEFT,CORNER_DOWNLEFT,
                                   map,cornersTemp[0], cornersTemp[1]);
        corners[1] = getCornerType(CORNER_UPRIGHT,CORNER_DOWNRIGHT,
                                   map,cornersTemp[2], cornersTemp[3]);
        if(map.getTileType(corners[1].x).getWalkable() &&
           map.getTileType(corners[0].x).getWalkable() &&
           map.getTileType(corners[1].y).getWalkable() &&
           map.getTileType(corners[0].y).getWalkable())
        {
            if(!space)
            {
            canJump = true;
            }
        }else{
            canJump = false;
        }
    }
}

void Character::holdingLeft(gameMap &map)
{
    sf::Vector2f posc1, posc2;
                //We once again need to check if we are inside a block to see if we
            //can move left or right
            if(climbing)
            {
                sf::Vector2i corners = getCornerType(CORNER_UPLEFT,
                                                     CORNER_DOWNLEFT,
                                                     map, posc1,posc2);
                if(map.getTileType(corners.x).getWalkable()&&
               map.getTileType(corners.y).getWalkable())
               {
                   setVelocityX(-getSpeed());
                    climbing = false;
               }
            }else{
               if(!wallJumping)
               {
                   setVelocityX(-getSpeed());
               }else if(wallJumping && wallJumpFrameCounter<getWallJumpPower().y)
               {
                   wallJumpFrameCounter++;
               }else
               {
                   setVelocityX(0);
                   wallJumping = false;
                   wallJumpFrameCounter=0;
               }

            }
}

void Character::holdingRight(gameMap &map)
{
    sf::Vector2f posc1, posc2;
    if(climbing)
            {
                sf::Vector2i corners = getCornerType(CORNER_UPRIGHT,
                                                     CORNER_DOWNRIGHT,
                                                     map, posc1,posc2);
                if(map.getTileType(corners.x).getWalkable()&&
               map.getTileType(corners.y).getWalkable())
               {
                   setVelocityX(getSpeed());
                    climbing = false;
               }
            }else{
                if(!wallJumping)
               {
                   setVelocityX(getSpeed());
               }else if(wallJumping && wallJumpFrameCounter<getWallJumpPower().y)
               {
                   wallJumpFrameCounter++;
               }else
               {
                   setVelocityX(0);
                   wallJumping = false;
                   wallJumpFrameCounter=0;
               }

            }
}

void Character::movingLeft(gameMap &map, float &afterY)
{
    sf::Vector2f newPos(getPosition().x, getPosition().y);
    sf::Vector2f posc1, posc2;
    //Move the character
            newPos.x = getPosition().x+getVelocity().x;
            setPosition(newPos);

            checkSlopedCorners(map, true, afterY);
float tempY = getPosition().y;
           setPosition(sf::Vector2f(getPosition().x, afterY));
           newPos = getPosition();

            //check top left/bottom left corner and get their type
            sf::Vector2i corners = getCornerType(CORNER_UPLEFT,
                                                 CORNER_DOWNLEFT,
                                                 map, posc1,posc2);

            //Check to set if either of the new corners are not walkable
            //If they are not, we reposition
            if(!map.getTileType(corners.x).getWalkable()||
               !map.getTileType(corners.y).getWalkable())
            {
                //Stop all velocity
                hitAWall();

                if(space && wallJump && canWallJump &&
                   !map.getTileType(corners.y).getWalkable() &&
                   !map.getTileType(corners.x).getWalkable())
                {
                    canWallJump = false;
                    canJump = true;
                    wallJumping = true;
                    setVelocityX(getWallJumpPower().x);
                }

                //Reposition the character
                newPos.x = ((1+posc1.x)*(map.getTile(0).getTileDims().x)+
                            (getDimensions().x)/2);
            }
            setPosition(sf::Vector2f(newPos.x, tempY));
}

void Character::movingRight(gameMap &map, float &afterY)
{
    sf::Vector2f newPos(getPosition().x, getPosition().y);
    sf::Vector2f posc1, posc2;
    //Move the character
            newPos.x = getPosition().x+getVelocity().x;
            setPosition(newPos);
           checkSlopedCorners(map, true, afterY);
           float tempY = getPosition().y;
           setPosition(sf::Vector2f(getPosition().x, afterY));
           newPos = getPosition();

            //check top left/bottom right corner and get their type
            sf::Vector2i corners = getCornerType(CORNER_UPRIGHT,
                                                 CORNER_DOWNRIGHT,
                                                 map, posc1,posc2);

            //Check to set if either of the new corners are not walkable
            //If they are not, we reposition
            if(!map.getTileType(corners.x).getWalkable()||
               !map.getTileType(corners.y).getWalkable())
            {
                //Stop all velocity
                hitAWall();

                if(space && wallJump && canWallJump &&
                   !map.getTileType(corners.y).getWalkable() &&
                   !map.getTileType(corners.x).getWalkable())
                {
                    canWallJump = false;
                    canJump = true;
                    wallJumping = true;
                    setVelocityX(-getWallJumpPower().x);
                }

                //Reposition the character
                newPos.x = ((posc1.x)*(map.getTile(0).getTileDims().x)-
                            (getDimensions().x)/2);
            }
            setPosition(sf::Vector2f(newPos.x, tempY));
}

void Character::hitAWall()
{
    setVelocityX(0);
    wallJumpFrameCounter=0;
    wallJumping = false;
    if(!space && !canWallJump && !canJump)
    {
        canWallJump = true;
    }else if(canJump)
    {
        canWallJump = false;
    }
}
template <typename T>
void Character::checkXCollisionsWithMovableObjects(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet,
                        std::vector<T> & ObjectSet, bool theChecker,
                        std::string whatWeAreChecking )
{
    sf::Vector2f newPos = getPosition();
              for(int obNum = 0;obNum < ObjectSet.size();obNum++)
    {
        if(checkForCollision(ObjectSet[obNum]) &&
           !ObjectSet[obNum].getCloud())
        {
            T & obj = ObjectSet[obNum];
            int dir = copysign(1,obj.getPosition().x - getPosition().x);

            std::vector <int> alreadyCheckedx;
            std::vector <int> alreadyCheckedy;
            alreadyCheckedx.push_back(id);
            float newX = getPosition().x;
            if((fabs(getPosition().x - obj.getPosition().x)<
               obj.getPushingSpeed() || whatWeAreChecking == "characters")&&
                whatWeAreChecking != "Moving Blocks")
            {
                newX = getPosition().x + (getDimensions().x/2+obj.getDimensions().x/2)*dir;
            }else if(whatWeAreChecking != "Moving Blocks")
            {
                newX = obj.getPosition().x + obj.getPushingSpeed()*dir;
            }

            if(dir ==1)
            {
                if(theChecker || (!obj.getClimbing() && whatWeAreChecking == "characters"))
                {
                      obj.setPosition(sf::Vector2f(newX,obj.getPosition().y));
                    obj.Movable_Object::update(allMaps, playerSet,currentMap,
                               alreadyCheckedx, alreadyCheckedy,
                               false, false, true, false, false);
                }
                newPos.x = obj.getPosition().x-obj.getDimensions().x/2 - getDimensions().x/2;

            }else{
                if(theChecker || (!obj.getClimbing() && whatWeAreChecking == "characters"))
                {
                    obj.setPosition(sf::Vector2f(newX,obj.getPosition().y));
                    obj.Movable_Object::update(allMaps, playerSet, currentMap,
                               alreadyCheckedx, alreadyCheckedy,
                               false, true, false, false, false);
                }
                newPos.x = obj.getPosition().x+obj.getDimensions().x/2 +getDimensions().x/2;

            }
            setPosition(newPos);
        }
    }
}

void Character::varJumpHeight(float gravEffects,float gravForce)
{
      //Jumping determines if you are in the middle of a jump
    //jumping ends when you release space or reach max jump power
    if(jumping)
    {
        //if you are still holding the space key and can still jump higher
        //keep going up
        if(space && jumpFrameCounter < getJumpHeight().y)
        {
            jumpFrameCounter++;
        }else{
            //If you release space or reach max, get velocity to max
            //aftwr frame buffer and no longer jumping
            setVelocityY(-gravEffects*getJumpHeight().x+
                         (getJumpHeight().y)*gravForce);
            jumping = false;
        }
    }
}

void Character::gravityEffects(gameMap & map, std::vector<Character> & playerSet,
                                float gravEffects,float gravForce)
{
    //If the game is set to sideScroller and you are either not no pressing
    //space or cannot jump
    if(map.getSideScroll() && (!space || !canJump) && !climbing )
    {

        //Add gravity to the velocity
        setVelocityY(gravForce+getVelocity().y);

        //Limit the velocity to 1 tile/frame
        if(fabs(getVelocity().y) >((map.getTile(0).getTileDims().y/2 +playerSet[0].getDimensions().y/2))-1)
        {
            setVelocityY((gravEffects*
                          (map.getTile(0).getTileDims().y/2 +playerSet[0].getDimensions().y/2))-
                         gravEffects);
        }else if(swimming && abs(getVelocity().y) >(map.getTile(0).getTileDims().y)/8-1)
        {
                        setVelocityY(copysign(1,getVelocity().y)*(gravEffects*
                          map.getTile(0).getTileDims().y)/8-
                         gravEffects);
        }
    }else if(map.getSideScroll() && space && canJump)
    {
        //Reset the jumping frame counter
        jumpFrameCounter = 0;

        //set the Y velocity to the jump
        setVelocityY(-gravEffects*getJumpHeight().x);

        //You sre now jumping and cannot jump again until landing
        climbing = false;
        jumping = true;
        canJump = false;
        grounded = false;
    }
}

    void Character::checkIfUDHeld(gameMap &map)
    {
        if(up && (!map.getSideScroll()|| climbing))
        {
            setVelocityY(-getSpeed());
        }
        if(down && (!map.getSideScroll()|| climbing))
        {
            setVelocityY(getSpeed());
        }
    }

void Character::checkUp(gameMap & map, std::vector<Character> & playerSet,
                        float gravEffects,float gravForce )
{
    sf::Vector2f newPos = getPosition();
    sf::Vector2f posForCloud, posc1, posc2;
            bool cloudHit = false;
            sf::Vector2i cornersBeforeMovement = getCornerType(CORNER_UPLEFT,
                                                 CORNER_UPRIGHT,
                                                 map, posForCloud,posc2);
            newPos.y = getPosition().y+getVelocity().y;
            setPosition(newPos);

            //Check all the corners if we are climbing and if they are
            //all not ladders, we can fall
            sf::Vector2i corners2 = getCornerType(CORNER_DOWNLEFT,
                                                    CORNER_DOWNRIGHT,
                                                    map, posc1,posc2);
            sf::Vector2i corners = getCornerType(CORNER_UPLEFT,
                                                CORNER_UPRIGHT,
                                                map, posc1,posc2);
            if(climbing)
            {

                if(map.getTileType(corners.x).getLadder()==0&&
                    map.getTileType(corners.y).getLadder()==0&&
                    map.getTileType(corners2.x).getLadder()==0&&
                    map.getTileType(corners2.y).getLadder()==0)
                {
                    climbing = false;
                }
            }
            //check for ladders before anything else
            if(up && !climbing)
                checkForLadders(map, newPos, playerSet);

                if(!up &&
                   ((map.getTileType(cornersBeforeMovement.x).getCloud()&&
                     map.getTileType(cornersBeforeMovement.x).getSlanted() == "No") ||
                    (map.getTileType(cornersBeforeMovement.y).getCloud() &&
                     map.getTileType(cornersBeforeMovement.y).getSlanted() == "No")) &&
                   posForCloud.y != posc1.y && gravEffects<0)
                {
                    cloudHit = true;
                }


            if(((!map.getTileType(corners.x).getWalkable()||
                !map.getTileType(corners.y).getWalkable())||cloudHit)&&
               (!climbing||map.getTileType(corners.x).getLadder()==0
                ||map.getTileType(corners.y).getLadder()==0))
            {
                hitTheCeiling();
                newPos.y = ((1+posc1.y)*(map.getTile(0).getTileDims().y)+
                            (getDimensions().y)/2);
            }
            setPosition(newPos);
}

void Character::checkDown(gameMap  &map, std::vector<Character> & playerSet,
                        float gravEffects,float gravForce )
{
     sf::Vector2f newPos = getPosition();
    sf::Vector2f posForCloud, posc1, posc2;
            bool cloudHit = false;
            sf::Vector2i cornersBeforeMovement = getCornerType(CORNER_DOWNLEFT,
                                                 CORNER_DOWNRIGHT,
                                                 map, posForCloud,posc2);
            newPos.y = getPosition().y+getVelocity().y;
            setPosition(newPos);
            sf::Vector2i corners2 = getCornerType(CORNER_UPLEFT,
                                                    CORNER_UPRIGHT,
                                                    map, posc1,posc2);
            sf::Vector2i corners = getCornerType(CORNER_DOWNLEFT,
                                                 CORNER_DOWNRIGHT,
                                                 map, posc1,posc2);
            if(climbing)
            {
                if(map.getTileType(corners.x).getLadder()==0&&
                    map.getTileType(corners.y).getLadder()==0&&
                    map.getTileType(corners2.x).getLadder()==0&&
                    map.getTileType(corners2.y).getLadder()==0)
                {
                    climbing = false;
                }
            }

            //check for ladders before anything else
            if(down && !climbing)
            {
                checkForLadders(map, newPos, playerSet);
            }
                if(!down &&
                   ((map.getTileType(corners.x).getCloud()&&
                     map.getTileType(corners.x).getSlanted() == "No") ||
                    (map.getTileType(corners.y).getCloud()&&
                     map.getTileType(corners.y).getSlanted() == "No")) &&
                   posForCloud.y != posc1.y && gravEffects>0)
                {
                    cloudHit = true;
                }
                //If you hit a deadly spike
                if((map.getTileType(corners.x).getDeadly() ||
                    map.getTileType(corners.y).getDeadly())&&
                   map.getTileType(corners.x).getWalkable() &&
                    map.getTileType(corners.y).getWalkable() &&
                   posForCloud.y != posc1.y && gravEffects>0)
                   {
                       setHealth(0);
                       std::cout << "YOU ARE DEAD" << std::endl;
                       map.getKillList().push_back(id);
                       newPos = getPosition();
                   }

            if(((!map.getTileType(corners.x).getWalkable()||
               !map.getTileType(corners.y).getWalkable())||(cloudHit))&&
               (!climbing||map.getTileType(corners.x).getLadder()==0
                ||map.getTileType(corners.y).getLadder()==0))
            {
                hitTheFloor();
                newPos.y = ((posc1.y)*(map.getTile(0).getTileDims().y)-
                            (getDimensions().y)/2);
            }
            setPosition(newPos);
}

void Character::hitTheCeiling()
{
    jumping = false;
                    wallJumping=false;
                    canWallJump= false;
                    if(getVelocity().y <0)
                    {
                        setVelocityY(0);
                    }
}

void Character::checkYCollisionsWithMovableObjects(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet)
{
sf::Vector2f newPos = getPosition();
for(int obNum = 0;obNum < map.getAllObjects().size();obNum++)
    {
        if(checkForCollision(map.getAllObjects()[obNum]))
        {

            Movable_Object & obj = map.getAllObjects()[obNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            std::vector <int> alreadyCheckedx;
            std::vector <int> alreadyCheckedy;
            alreadyCheckedy.push_back(id);

            float newY = getPosition().y + (getDimensions().y/2+obj.getDimensions().y/2)*dir;
            if(dir ==1)
            {
                hitTheFloor();
                setVelocityY(std::max(obj.getVelocity().y,static_cast<float>(0.0)));
                      obj.setPosition(sf::Vector2f(obj.getPosition().x, (newY)));

                    obj.update(allMaps, playerSet,currentMap, alreadyCheckedx, alreadyCheckedy, false, false, false, false, true);

                newPos.y = obj.getPosition().y-obj.getDimensions().y/2 - getDimensions().y/2;
            }else{

              if(obj.getLiftable())
                {
                    obj.setVelocityY(0);
                    obj.setPosition(sf::Vector2f(obj.getPosition().x, newY));

                    obj.update(allMaps, playerSet, currentMap, alreadyCheckedx, alreadyCheckedy, false, false, false, true, false);
                }else
                {

                    hitTheCeiling();
                }
                if(obj.getPosition().y != newY)
                {
                    hitTheCeiling();
                }
                newPos.y = obj.getPosition().y+obj.getDimensions().y/2 +getDimensions().y/2;

            }
            setPosition(newPos);
        }
    }
     setPosition(newPos);
                       }

void Character::checkYCollisionsWithCharacters(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet)
{
    sf::Vector2f newPos = getPosition();
        for(int charNum =0 ; charNum < playerSet.size();charNum++)
    {
     if(checkForCollision(playerSet[charNum]))
        {
            Character & obj = playerSet[charNum];
            std::vector <int> alreadyCheckedx;
            std::vector <int> alreadyCheckedy;
            alreadyCheckedy.push_back(id);
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            if(dir ==1)
            {
                hitTheFloor();
                setVelocityY(std::max(static_cast<float>(0),obj.getVelocity().y));
                if(!obj.getClimbing())
                {
                            obj.setPosition(sf::Vector2f(obj.getPosition().x,getPosition().y +
                                                          (getDimensions().y/2+obj.getDimensions().y/2)*dir));
                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                                            alreadyCheckedx, alreadyCheckedy,
                                            false, false, false, false, true);
                }

               newPos.y = obj.getPosition().y-obj.getDimensions().y/2 - getDimensions().y/2;
            }else{
                if(!obj.getClimbing())
                {
                      obj.setPosition(sf::Vector2f(obj.getPosition().x,getPosition().y +
                                                          (getDimensions().y/2+obj.getDimensions().y/2)*dir));
                obj.Movable_Object::update(allMaps, playerSet, currentMap,
                                            alreadyCheckedx, alreadyCheckedy,
                                            false, false, false, true ,false);
                }else
                {
                    hitTheCeiling();

                }

              newPos.y = obj.getPosition().y+obj.getDimensions().y/2 +getDimensions().y/2;
            }
            setPosition(newPos);
    }
    }
    setPosition(newPos);

}

void Character::checkYCollisionsWithFallingBlocks(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet)
{
    sf::Vector2f newPos = getPosition();
for(int obNum = 0;obNum < map.getFallingBlocks().size();obNum++)
    {

        if(checkForCollision(map.getFallingBlocks()[obNum]) )
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
                hitTheFloor();
                newPos.y = obj.getPosition().y-obj.getDimensions().y/2 - getDimensions().y/2;
            }else if(!map.getFallingBlocks()[obNum].getCloud()){
                hitTheCeiling();
                newPos.y = obj.getPosition().y+obj.getDimensions().y/2 +getDimensions().y/2;

            }
            setPosition(newPos);
        }
    }
     setPosition(newPos);
}

void Character::checkYCollisionsWithMovingBlocks(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet)
{
    sf::Vector2f newPos = getPosition();
        for(int movNum =0 ; movNum < map.getMovingBlocks().size();movNum++)
    {

     if(checkForCollision(map.getMovingBlocks()[movNum])&& !map.getMovingBlocks()[movNum].getCloud())
        {
            Moving_Block & obj = map.getMovingBlocks()[movNum];
            int dir = copysign(1,obj.getPosition().y - getPosition().y);
            newPos.y = obj.getPosition().y - (getDimensions().y/2+obj.getDimensions().y/2)*dir;
              setPosition(newPos);


            if(dir ==1)
            {
               AttachedToMovingBlock(map, allMaps, playerSet,
                                  newPos, obj);

            }else{

                hitTheCeiling();

            }
        }else if(checkForCollision(map.getMovingBlocks()[movNum])&& !down && map.getMovingBlocks()[movNum].getCloud()&& getPosition().y > yBeforeMovement)
        {

            float tempY = getPosition().y;
            setPosition(sf::Vector2f(getPosition().x,yBeforeMovement));
            if(!checkForCollision(map.getMovingBlocks()[movNum]))
            {

              setPosition(sf::Vector2f(getPosition().x,tempY));
              Moving_Block & obj = map.getMovingBlocks()[movNum];

            AttachedToMovingBlock(map, allMaps, playerSet,
                                  newPos, obj);
            }else{
                setPosition(sf::Vector2f(getPosition().x,tempY));
                newPos = getPosition();
            }
        }
        }
 setPosition(newPos);
}

void Character::AttachedToMovingBlock(gameMap &map,
                                      std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet,
                       sf::Vector2f &newPos,
                       Moving_Block & obj)
{
    std::vector <int> alreadyCheckedx;
            std::vector <int> alreadyCheckedy;
            alreadyCheckedy.push_back(obj.getId());
                int dir = copysign(1,obj.getPosition().y - getPosition().y);
            newPos.y = obj.getPosition().y - (getDimensions().y/2+obj.getDimensions().y/2)*dir;
              setPosition(newPos);
              hitTheFloor();
              setVelocityY(obj.getVelocity().y);

                newPos.x = getPosition().x + obj.getVelocity().x;
                setPosition(newPos);
                if(obj.getVelocity().x>0)
                {
                    Movable_Object::update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, true, false, false);
                    newPos = getPosition();
                }else
                {
                    Movable_Object::update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, true, false, false, false);
                    newPos = getPosition();
                }
                setPosition(newPos);
                if(obj.getVelocity().y>0)
                {
                    Movable_Object::update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, false, false, true);
                    newPos = getPosition();
                }else
                {
                    Movable_Object::update(allMaps,playerSet,currentMap,alreadyCheckedx,
                                           alreadyCheckedy,false, false, false, true, false);
                    newPos = getPosition();
                }
}
