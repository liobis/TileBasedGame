#ifndef GAMEMAP_H
#define GAMEMAP_H
class Movable_Object;
class Moving_Block;
class Falling_Block;
class gameMap
{
private:
    std::vector< std::vector<int> > map;
    std::vector<Tile> tileSet;
    std::vector<TileType> tileTypeSet;
    std::vector<LadderType> ladderTypeSet;
    std::map< int ,std::map < int,Door > > doorSet;
    std::vector<Door> fullDoorSet;
    std::string loadedFrom;
    std::vector<Movable_Object> allObjects;
    std::vector<Object_Type> objectTypeSet;
    std::vector<Moving_Block> allMovingBlocks;
    std::vector<int> killList;
    std::vector<Falling_Block> allFallingBlocks;
    gameMap();
    float gravity;
    bool sideScroll;
public:
    gameMap(std::vector <std::vector<int> > p_map,
                   std::vector<Tile> p_tileSet,std::vector<TileType> p_tileTypeSet,
                   std::vector<LadderType> p_ladderTypeSet,
                   std::map< int ,std::map < int,Door > > p_doorSet,
                   std::vector<Door> p_fullDoorSet,
                   std::string p_loadedFrom,
                   std::vector<Movable_Object> p_allObjects,
                   std::vector<Object_Type> p_objectTypeSet,
                   std::vector<Moving_Block> p_allMovingBlocks,
                   std::vector<Falling_Block> p_allFallingBlocks,
                   float p_gravity, bool p_setSideScroll);
int getHeight();
int getWidth();
int tileNum(int x, int y);
Tile getTile(int num);
TileType getTileType(int num);
LadderType getLadderType(int num);
float getGravity();
void setGravity(float p_gravity);
bool getSideScroll();
void setSideScroll(bool p_setSideScroll);
Door & getDoor(int x, int y);
std::string getLoadedFrom();
std::vector<Door> & getFullDoorSet();
std::vector<Movable_Object>  & getAllObjects();
std::vector<Object_Type> getObjectTypeSet();
std::vector<Moving_Block> & getMovingBlocks();
std::vector<int> & getKillList();
std::vector<Falling_Block> & getFallingBlocks();
void setKill(int p_kill);
};
#endif
