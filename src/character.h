
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"

#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"
#include "falling_block.h"

#ifndef CHARACTER_H
#define CHARACTER_H

class gameMap;

class Character : public Movable_Object
{
private:
    //Pixels Per 60 frames?
    float walkSpeed, runSpeed, swimSpeed;
    Character();
float frameJump;
    bool canJump;
    sf::Vector2f jumpHeight;
    int jumpFrameCounter, wallJumpFrameCounter;
    bool jumping;
    bool climbing;
    float relativeGrav;
    bool running;
    bool canEnterDoor;
    bool swimming;
    bool grounded;
    float normalRelGrav;
    sf::Vector2f swimJumpHeight;
    float waterGravEffects;
    int health;
    bool wallJump;
    bool wallJumping;
    bool canWallJump;
    sf::Vector2f wallJumpPower;
    int currentMap;
    bool left,right,up,down,space,run;
public:

Character(sf::Vector2f p_position, sf::Vector2f p_dimensions,
                     float p_walkSpeed, float p_runSpeed,float p_swimSpeed,
                      int p_ssNum,
                     sf::Vector2u p_spriteLoc,sf::Vector2f p_jumpHeight,
                     sf::Vector2f p_swimJumpHeight,sf::Vector2f p_wallJumpPower);
    void setWalkSpeed(float p_speed);
    void setRunSpeed(float p_speed);
    void setSwimSpeed(float p_speed);
    float getSpeed();
    int getHealth();
    int getCurrentMap();
    void setCurrentMap(int p_currentMap);
    bool getClimbing();
    void setLeft(bool p_left);
    void setRight(bool p_right);
    void setUp(bool p_up);
    void setDown(bool p_down);
    void setSpace(bool p_space);
    void setRun(bool p_run);
    float getRunSpeed();
    void changeHealth(int amount);
    void setHealth(int hp);
    float getNormalRelGrav();
    void setNormalRelGrav(float p_normRelGra);
    void resetRelGrav();
        void update(std::vector<gameMap> &allMaps,
                       std::vector< std::vector< std::vector < sf::Sprite > > > &spritesToDraw,
                       std::vector<sf::Texture> &textureSheet,
                       std::vector<Character> & playerSet);
void setJumpHeight(sf::Vector2f p_jumpHeight);
sf::Vector2f getJumpHeight();
void checkForLadders(gameMap &map, sf::Vector2f &newPos,std::vector<Character> & playerSet);
void checkForDoor(std::vector<gameMap> &allMaps,
             int &currentMap,std::vector< std::vector< std::vector < sf::Sprite > > > &spritesToDraw,
             std::vector<sf::Texture> &textureSheet);
sf::Vector2f getSwimJumpHeight();
void setSwimJumpHeight(sf::Vector2f p_swimJumpHeight);
void checkForSwimming(gameMap &map);
bool getWallJump();
void setWallJump(bool p_wallJump);
sf::Vector2f getWallJumpPower();
void setWallJumpPower(sf::Vector2f p_wallJumpPower);
void setClimbing(bool p_climbing);
void death();
void checkSlopedCorners(gameMap &map, bool checkLR, float &afterY);
void hitTheFloor();
void checkForRunning();
void jumpWhileSwimmingCheck(gameMap &map);
void checkIfClimbingAndInsideBlock(gameMap &map);
void holdingRight(gameMap &map);
void holdingLeft(gameMap &map);
void movingLeft(gameMap &map, float &afterY);
void movingRight(gameMap &map, float &afterY);
void hitAWall();
template <typename T>
void checkXCollisionsWithMovableObjects(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet,
                        std::vector<T> & ObjectSet, bool theChecker,
                        std::string whatWeAreChecking );
void varJumpHeight(float gravEffects,float gravForce);
void gravityEffects(gameMap & map, std::vector<Character> & playerSet,
                     float gravEffects,float gravForce);
void checkUp(gameMap &map, std::vector<Character> & playerSet,
             float gravEffects,float gravForce);
void checkIfUDHeld(gameMap &map);
void checkDown(gameMap &map, std::vector<Character> & playerSet,
                        float gravEffects,float gravForce );
void hitTheCeiling();
void checkYCollisionsWithMovableObjects(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet);
void checkYCollisionsWithCharacters(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet);
void checkYCollisionsWithFallingBlocks(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet);
void checkYCollisionsWithMovingBlocks(gameMap &map,std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet);
void AttachedToMovingBlock(gameMap &map,
                                      std::vector<gameMap> &allMaps,
                       std::vector<Character> & playerSet,
                       sf::Vector2f &newPos,
                       Moving_Block & obj);
void getAllCorners(gameMap & map, sf::Vector2f &cornerDL, sf::Vector2f &cornerDR,
                   sf::Vector2f &cornerUL, sf::Vector2f &cornerUR,
                   sf::Vector2f &cornerDLxy, sf::Vector2f &cornerDRxy,
                   sf::Vector2f & cornerULxy, sf::Vector2f &cornerURxy,
                   sf::Vector2i &downCorner, sf::Vector2i & upCorner);
void checkIfMovedATileForSloped(gameMap & map, std::string &dir,
                                            int dirToCheck, float & yOutput);
void placeObjectOnSlope(gameMap &map, std::string &dir, std::string C1dir,
                                   std::string C2dir, sf::Vector2i corners,
                                   sf::Vector2f cornerC1, sf::Vector2f cornerC2,
                                   sf::Vector2f cornerC1xy, sf::Vector2f cornerC2xy,
                                   float &yOutput);
void checkIfBetweenSlopedAndBlock(gameMap &map, std::string dir,
                                             sf::Vector2i &downCorner,
                                             sf::Vector2i &upCorner,
                                             sf::Vector2f &cornerDLxy,
                                             sf::Vector2f &cornerDRxy,
                                             sf::Vector2f &cornerULxy,
                                             sf::Vector2f &cornerURxy,
                                             sf::Vector2f &cornerDL,
                                             sf::Vector2f &cornerDR,
                                             sf::Vector2f &cornerUL,
                                             sf::Vector2f &cornerUR);
void checkIfMovedToTopOfSlope(gameMap &map, sf::Vector2i corners,
                                         int dir, sf::Vector2f cornerC1xy,
                                         sf::Vector2f cornerC2xy);
void checkSidesOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                        sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                        sf::Vector2f cornerDRxy);
void checkIfHitSideEdgeOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                                sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                                sf::Vector2f cornerURxy);
void checkIfHitTopBotEdgeOfSlopedBlock(gameMap &map, int dir,
                                                  sf::Vector2i corners,
                                                  sf::Vector2f cornerC1xy,
                                                  sf::Vector2f cornerC2xy);
void checkIfHitTopBotOfSlopedBlock(gameMap &map, int dir,
                                              sf::Vector2i corners,
                                              sf::Vector2f cornerC1xy,
                                              sf::Vector2f cornerC2xy);
bool checkSlopeDL(gameMap &map,
                  sf::Vector2f cornerC1,
                  sf::Vector2f cornerC1xy);
bool checkSlopeDR(gameMap &map,
                  sf::Vector2f cornerC2,
                  sf::Vector2f cornerC2xy);
bool checkSlopeUL(gameMap &map,
                  sf::Vector2f cornerC1,
                  sf::Vector2f cornerC1xy);
bool checkSlopeUR(gameMap &map,
                  sf::Vector2f cornerC2,
                  sf::Vector2f cornerC2xy);
void checkForSideConcave(gameMap &map,
                                             sf::Vector2i &downCorner,
                                             sf::Vector2i &upCorner,
                                             sf::Vector2f &cornerDLxy,
                                             sf::Vector2f &cornerDRxy,
                                             sf::Vector2f &cornerULxy,
                                             sf::Vector2f &cornerURxy,
                                             sf::Vector2f &cornerDL,
                                             sf::Vector2f &cornerDR,
                                             sf::Vector2f &cornerUL,
                                             sf::Vector2f &cornerUR);
void getCornerBeforeMovement(gameMap &map,
                                        sf::Vector2i &cornerType,
                                        sf::Vector2f &corner,
                                        sf::Vector2f &cornerxy,
                                        Corner C1);
void checkIfInASlope(gameMap &map);
};

#endif
