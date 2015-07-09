#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H
class Character;
class Movable_Object : public Movable
{
protected:

    float pushingSpeed;
    bool liftable;
    bool charMovable;
public:
     Movable_Object(sf::Vector2f p_position,
                                   sf::Vector2f p_dimensions,
                                   int p_ssNum,
                                   sf::Vector2u p_spriteLoc);
    Movable_Object();


void update(std::vector<gameMap> &allMaps, std::vector<Character> & playerSet,
             int &currentMap,
             std::vector<int> & alreadyCheckedx,
             std::vector<int> & alreadyCheckedy, bool withGrav,
            bool checkLeft, bool checkRight,
            bool checkUp, bool checkDown);
bool getCharMovable();
void setCharMovable(bool p_charMovable);
float getPushingSpeed();
void setPushingSpeed(float p_pushingSpeed);
bool getLiftable();
void setLiftable(bool p_liftable);
void checkForDeath(gameMap &map, std::vector<int> & alreadyChecked, bool pushedByBlock);
void checkSlopedCorners(gameMap &map, bool checkLR, float &afterY,
                        std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
bool getClimbing();
bool getCloud();
void checkIfPushedByMovingBlock(gameMap &map,
                                 std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
void checkLR(gameMap &map,const int dir, const bool &check, const bool &withGrav,
                               std::vector<int> & alreadyCheckedx, bool pushedByMovingBlockx,
                               float & yAfter,
                             std::vector<int> & alreadyCheckedy,
                             bool & pushedByMovingBlocky);
template <typename T>
void checkXCollisionsWithMovableObject(gameMap &map, std::vector<gameMap> &allMaps,
                                                       int currentMap,
                                                        std::vector<Character> & playerSet,
                                                       std::vector<int> & alreadyCheckedx,
                                                       std::vector<int> & alreadyCheckedy,
                                                       bool & pushedByMovingBlockx,
                                                       std::vector<T> & ObjectSet,
                                                        bool check);
void gravityEffect(gameMap &map,std::vector<Character> & playerSet,
                   float gravEffects, float gravForce);
void checkUD(gameMap &map, const int dir, const bool &check, const bool &withGrav,
             std::vector<int> & alreadyCheckedy, bool pushedByMovingBlocky, float gravEffects);
template <typename T>
void checkYCollisionsWithMovableObject(gameMap &map, std::vector<gameMap> &allMaps,
                                                       int currentMap,
                                                        std::vector<Character> & playerSet,
                                                       std::vector<int> & alreadyCheckedx,
                                                       std::vector<int> & alreadyCheckedy,
                                                       std::vector<T> & ObjectSet);
void checkYCollisionsWithFallingBlocks(gameMap &map);
void checkYCollisionsWithMovingBlocks(gameMap &map,
                                                      std::vector<gameMap> &allMaps,
                                                       int currentMap,
                                                        std::vector<Character> & playerSet,
                                                       std::vector<int> & alreadyCheckedx,
                                                       std::vector<int> & alreadyCheckedy,
                                                       bool withGrav);
void getAllCorners(gameMap & map, sf::Vector2f &cornerDL, sf::Vector2f &cornerDR,
                   sf::Vector2f &cornerUL, sf::Vector2f &cornerUR,
                   sf::Vector2f &cornerDLxy, sf::Vector2f &cornerDRxy,
                   sf::Vector2f & cornerULxy, sf::Vector2f &cornerURxy,
                   sf::Vector2i &downCorner, sf::Vector2i & upCorner);
void checkIfMovedATileForSloped(gameMap & map, std::string &dir,
                                            int dirToCheck, float & yOutput,
                             std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
void placeObjectOnSlope(gameMap &map, std::string &dir, std::string C1dir,
                                   std::string C2dir, sf::Vector2i corners,
                                   sf::Vector2f cornerC1, sf::Vector2f cornerC2,
                                   sf::Vector2f cornerC1xy, sf::Vector2f cornerC2xy,
                                   float &yOutput,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
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
                                             sf::Vector2f &cornerUR,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky );
void checkIfMovedToTopOfSlope(gameMap &map, sf::Vector2i corners,
                                         int dir, sf::Vector2f cornerC1xy,
                                         sf::Vector2f cornerC2xy);
void checkSidesOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                        sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                        sf::Vector2f cornerDRxy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
void checkIfHitSideEdgeOfSlopedBlock(gameMap &map, sf::Vector2i downCorner,
                                                sf::Vector2i upCorner, sf::Vector2f cornerDLxy,
                                                sf::Vector2f cornerURxy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
void checkIfHitTopBotEdgeOfSlopedBlock(gameMap &map, int dir,
                                                  sf::Vector2i corners,
                                                  sf::Vector2f cornerC1xy,
                                                  sf::Vector2f cornerC2xy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
void checkIfHitTopBotOfSlopedBlock(gameMap &map, int dir,
                                              sf::Vector2i corners,
                                              sf::Vector2f cornerC1xy,
                                              sf::Vector2f cornerC2xy,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
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
                                             sf::Vector2f &cornerUR,
                                   std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy,
                             bool &pushedByMovingBlockx,
                             bool & pushedByMovingBlocky);
void getCornerBeforeMovement(gameMap &map,
                                        sf::Vector2i &cornerType,
                                        sf::Vector2f &corner,
                                        sf::Vector2f &cornerxy,
                                        Corner C1);
void checkIfInASlope(gameMap &map);
};
#endif
