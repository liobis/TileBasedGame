#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "tile.h"
#include "character.h"
#include "ladderType.h"
#include "tileType.h"
#include "door.h"
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"
#include "falling_block.h"
void grim_reaper(gameMap & map,std::vector<Character> & playerSet)
{
    //Go through all the objects to kill
    for(int obj=0;obj <map.getKillList().size();obj++)
    {
        int currentId = map.getKillList()[obj];
        //We gotta find out what kind of object this is...AND KILL IT!
        bool done = false;
        for(int movableObj = 0;movableObj <  map.getAllObjects().size();movableObj ++)
        {

            if(currentId == map.getAllObjects()[movableObj].getId())
            {
                map.getAllObjects().erase(map.getAllObjects().begin()+movableObj);
                done = true;
                break;
            }
        }
        if(done)
        {
            continue;
        }
        for(int charObj = 0;charObj < playerSet.size();charObj++)
        {
            if(currentId == playerSet[charObj].getId())
            {
                playerSet[charObj].death();
                done = true;
                break;

            }
        }
             if(done)
        {
            continue;
        }
        for(int fallingObj = 0;fallingObj <  map.getFallingBlocks().size();fallingObj ++)
        {

            if(currentId == map.getFallingBlocks()[fallingObj].getId())
            {
                map.getFallingBlocks().erase(map.getFallingBlocks().begin()+fallingObj);
                break;
            }
        }
    }
    map.getKillList().clear();
}
