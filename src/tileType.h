
#ifndef TILETYPE_H
#define TILETYPE_H
class TileType
{
private:
    bool walkable;
    int ladder;
    bool cloud;
    bool door;
    bool water;
    bool deadly;
    std::string slanted;
public:
    TileType();
    TileType(bool p_walk, int p_ladder,bool p_cloud,
             bool p_door, bool p_water, bool p_deadly);
    void setWalkable(bool walk);
    bool getWalkable();
    void setLadder(int p_ladder);
    int getLadder();
    bool getCloud();
    void setCloud(bool p_cloud);
    bool getDoor();
    void setDoor(bool p_door);
    bool getWater();
    void setWater(bool p_water);
    bool getDeadly();
    void setDeadly(bool p_deadly);
    std::string getSlanted();
    void setSlanted(std::string p_slanted);
};
#endif
