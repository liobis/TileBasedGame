#include <string>
#include "tileType.h"
TileType::TileType(){}
TileType::TileType(bool p_walk, int p_ladder, bool p_cloud,
                   bool p_door, bool p_water, bool p_deadly)
{
    setWalkable(p_walk);
    setLadder(p_ladder);
    setCloud(p_cloud);
    setDoor(p_door);
    setWater(p_water);
    setDeadly(p_deadly);
}
void TileType::setWalkable(bool walk)
{
    walkable = walk;
}
bool TileType::getWalkable()
{
    return walkable;
}
void TileType::setLadder(int p_ladder)
{
    ladder = p_ladder;
}
int TileType::getLadder()
{
    return ladder;
}
bool TileType::getCloud()
{
    return cloud;
}
void TileType::setCloud(bool p_cloud)
{
    cloud = p_cloud;
}
    bool TileType::getDoor()
    {
        return door;
    }
    void TileType::setDoor(bool p_door)
    {
        door = p_door;
    }
bool TileType::getWater()
{
    return water;
}
void TileType::setWater(bool p_water)
{
    water = p_water;
}
bool TileType::getDeadly()
{
    return deadly;
}
void TileType::setDeadly(bool p_deadly)
{
    deadly = p_deadly;
}
std::string TileType::getSlanted()
{
    return slanted;
}
void TileType::setSlanted(std::string p_slanted)
{
    slanted = p_slanted;
}
