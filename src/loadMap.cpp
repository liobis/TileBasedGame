
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "character.h"
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"
#include "moving_block.h"
#include "falling_block.h"
#include "tile_functions.h"

#include "procedural_generation.h"
#include "loadMap.h"

//Load the map from the location
std::vector< std::vector<int> > loadMap(std::string const &loc)
{
    //create the empty map
    std::vector< std::vector<int> > currentMap;

    //if we need to load it from a file
    if(loc != "random")
       {
    std::ifstream loadedMap(loc.c_str());

    //Flags for the beginning and end of the map
    std::string startMapFlag = "StartMap";
    std::string endMapFlag = "EndMap";

    //Counters for the height and width and of the map
    int counter_height = 0;
    int counter_width = 0;

    //holdr string for text inputs
    std::string holder;

    //First determine height and width of map
    if(loadedMap.is_open())
    {
        //get lines until we get to the startmap flag
        getline(loadedMap, holder);

        while(holder!=startMapFlag)
        {
        getline(loadedMap, holder);

        }

    getline(loadedMap, holder);

    //keep getting lines until we get to the end map flag
    while (holder!= endMapFlag)
    {
        counter_height++;
        getline(loadedMap, holder);
    }

    //once we get the height, reset to get the wdith
    loadedMap.clear();
    loadedMap.seekg(0);
    getline(loadedMap, holder);
    while(holder!=startMapFlag)
    {
        getline(loadedMap, holder);

    }
    loadedMap >> holder;
    while (holder!= endMapFlag)
    {
        counter_width++;
        loadedMap >> holder;
    }
    }

    //We've counted up all the tiles, divided by the height to get the width
    counter_width /= counter_height;


    //Now we have the height and width, let's finally get the damn map
    loadedMap.clear();
    loadedMap.seekg(0);
    getline(loadedMap, holder);
    while(holder!=startMapFlag)
    {
        getline(loadedMap, holder);

    }
    for(int y=0; y<counter_height;y++)
    {
        //temp holds the current row we are on
        std::vector<int> temp;
        for(int x=0;x<counter_width;x++)
        {
            loadedMap >> holder;
            temp.push_back(stringToInt(holder));
        }

        //Add the current row to the map
        currentMap.push_back(temp);
    }
       }else{
           //create a random map
       currentMap = generateMap(8,5,1,1,3);
       }
    return currentMap;

}

//;load all the doors from the map
std::map< int ,std::map < int,Door > > loadDoors(std::string const &loc,
                                                 std::vector<Door> &p_fullDoorSet)
{
    //Load the map
    std::ifstream loadedMap(loc.c_str());
    std::map< int ,std::map < int,Door > >  doors;

    //Flags for the beginning and end of the doors
    std::string startDoorFlag = "StartDoors";
    std::string endDoorFlag = "EndDoors";

    std::string holder;

    //First determine height and width of map
    if(loadedMap.is_open())
    {
        //get lines until we get to the startmap flag
        getline(loadedMap, holder);

        //Start at the start door flag
        while(holder!=startDoorFlag)
        {
            getline(loadedMap, holder);
        }

        loadedMap >> holder;

        //while we are not at the end
        while(holder!=endDoorFlag)
        {
            int holderx, holdery;

            //create the door
            Door holderDoor;

            //First get the x,y position of the door
            loadedMap >> holder;
            holderx = stringToInt(holder);
            loadedMap >> holder;
            holdery = stringToInt(holder);
            holderDoor.setDoorLoc(sf::Vector2i(holderx, holdery));

            //Get the map to load when we enter the door
            loadedMap >> holder;
            holderDoor.setMapType(holder);

            //Get the x,y, position of where to load into the new map
            loadedMap >> holder;
            holderx = stringToInt(holder);
            loadedMap >> holder;
            holdery = stringToInt(holder);
            holderDoor.setNewDoorLoc(sf::Vector2i(holderx, holdery));
            doors[holderDoor.getDoorLoc().x][holderDoor.getDoorLoc().y] = holderDoor;

            //add the door to the list
            p_fullDoorSet.push_back(holderDoor);
            loadedMap >> holder;
        }
    }
    return doors;
}


//Update all the sprites used in the current map
std::vector< std::vector < sf::Sprite > > updateMapSprites(gameMap &map, std::vector<sf::Texture> &textureSheet)
{
    //Similar to before, we get all the sprites
    std::vector <sf::Sprite> sprite;
    std::vector< std::vector < sf::Sprite > > output;

    //Go through all the tiles
    for(int y = 0;y<map.getHeight();y++)
    {
        for(int x = 0;x<map.getWidth();x++)
        {


 sf::Sprite currentTile;
            //First, get the number of tile
            int tileType = map.tileNum(x,y);

            //Get it's dimensions
            sf::Vector2u dims = map.getTile(tileType).getTileDims();

            //Get where on the spritesheet it is
            sf::Vector2u spriteDims = map.getTile(tileType).getSpriteLoc();

            //Create the sprite and push it to the output
            currentTile.setTexture(textureSheet[map.getTile(tileType).getSSNum()]);
            currentTile.setTextureRect(sf::IntRect(spriteDims.x*dims.x,spriteDims.y*dims.y,dims.x,dims.y));
            currentTile.setPosition(x*dims.x,y*dims.y);
            sprite.push_back(currentTile);
        }
        output.push_back(sprite);
        sprite.clear();
    }
    for(int i = 0;i<map.getAllObjects().size();i++)
    {

        sf::Sprite currentObject;
        sf::Vector2f dims = map.getAllObjects()[i].getSpriteDimensions();
        sf::Vector2u spriteDims = map.getAllObjects()[i].getSpriteLoc();
        sf::Vector2f spriteSize = map.getAllObjects()[i].getDimensions();
        currentObject.setTexture(textureSheet[map.getAllObjects()[i].getSSNum()]);
        currentObject.setTextureRect(sf::IntRect(spriteDims.x*map.getTile(0).getTileDims().x,spriteDims.y*map.getTile(0).getTileDims().y,spriteSize.x,spriteSize.y));
        currentObject.setOrigin((spriteSize.x)/2,(spriteSize.y)/2);
        map.getAllObjects()[i].setSprite(currentObject);
    }
        for(int i = 0;i<map.getMovingBlocks().size();i++)
    {

        sf::Sprite currentObject;
        sf::Vector2f dims = map.getMovingBlocks()[i].getSpriteDimensions();
        sf::Vector2u spriteDims = map.getMovingBlocks()[i].getSpriteLoc();
        sf::Vector2f spriteSize = map.getMovingBlocks()[i].getDimensions();
        currentObject.setTexture(textureSheet[map.getMovingBlocks()[i].getSSNum()]);

   currentObject.setTextureRect(sf::IntRect(spriteDims.x*map.getTile(0).getTileDims().x,spriteDims.y*map.getTile(0).getTileDims().y,spriteSize.x,spriteSize.y));
        currentObject.setOrigin((spriteSize.x)/2,(spriteSize.y)/2);
        map.getMovingBlocks()[i].setSprite(currentObject);
    }
            for(int i = 0;i<map.getFallingBlocks().size();i++)
    {

        sf::Sprite currentObject;
        sf::Vector2f dims = map.getFallingBlocks()[i].getSpriteDimensions();
        sf::Vector2u spriteDims = map.getFallingBlocks()[i].getSpriteLoc();
        sf::Vector2f spriteSize = map.getFallingBlocks()[i].getDimensions();
        currentObject.setTexture(textureSheet[map.getFallingBlocks()[i].getSSNum()]);

   currentObject.setTextureRect(sf::IntRect(spriteDims.x*map.getTile(0).getTileDims().x,spriteDims.y*map.getTile(0).getTileDims().y,spriteSize.x,spriteSize.y));
        currentObject.setOrigin((spriteSize.x)/2,(spriteSize.y)/2);
        map.getFallingBlocks()[i].setSprite(currentObject);
    }
    return output;
}

//create the gameMap
void loadGameMap(std::string loc,std::vector<gameMap> &allMaps)
{

    int numOfSpriteSheets, numOfTiles,numOfTileTypes,numOfLadderTypes;
    //Find out how many tiles and spritesheets we have
    loadSS_Tile_Nums(numOfSpriteSheets, numOfTiles,numOfTileTypes, numOfLadderTypes);

    //Create the texture and tile vectors
    std::vector<Tile> tileSet(numOfTiles);
    std::vector<TileType> tileTypeSet(numOfTileTypes);
    std::vector<LadderType> ladderTypeSet(numOfLadderTypes);
    std::vector<Door> fullDoorSet;
    std::vector<Movable_Object> allObjects;
    std::vector<Object_Type> objectTypeSet;
std::vector<Moving_Block> allMovingBlocks;
std::vector<Falling_Block> allFallingBlocks;
    //Load all the doors
    std::map< int ,std::map < int,Door > > doorSet = loadDoors(loc, fullDoorSet);

    //load all the tiles and spritesheets
    loadEverything(numOfSpriteSheets, numOfTiles, numOfTileTypes, numOfLadderTypes, tileSet, tileTypeSet, ladderTypeSet);
    loadObjectTypes(objectTypeSet);
    loadMovableObjects(loc, allObjects, allMovingBlocks, allFallingBlocks, objectTypeSet,tileSet);

    //Load the starting map and create the gameMap
    std::vector< std::vector<int> > currentMap = loadMap(loc);
    allMaps.push_back(gameMap(currentMap, tileSet,tileTypeSet, ladderTypeSet,
                               doorSet, fullDoorSet, loc, allObjects,
                               objectTypeSet, allMovingBlocks, allFallingBlocks, 0.8,true));

}

void loadMovableObjects(std::string const &loc,
                         std::vector<Movable_Object> &p_allObjects,
                         std::vector<Moving_Block> &p_allMovingObjects,
                         std::vector<Falling_Block> &p_allFallingBlocks,
                         std::vector<Object_Type> & p_objectTypeSet,
                         std::vector<Tile> & tileSet)
{
    //Load the map
    std::ifstream loadedMap(loc.c_str());

    //Flags for the beginning and end of the doors
    std::string startObjectsFlag = "StartObjects";
    std::string endObjectsFlag = "EndObjects";
        std::string startMovingObjectsFlag = "StartMovingBlocks";
    std::string endMovingObjectsFlag = "EndMovingBlocks";
    std::string startFallingObjectsFlag = "StartFallingBlocks";
    std::string endFallingObjectsFlag = "EndFallingBlocks";

    std::string holder;

    //First determine height and width of map
    if(loadedMap.is_open())
    {

        //get lines until we get to the startmap flag
        getline(loadedMap, holder);

        //Start at the start door flag
        while(holder!=startObjectsFlag)
        {
            getline(loadedMap, holder);
        }

        loadedMap >> holder;

        //while we are not at the end
        while(holder!=endObjectsFlag)
        {

            int holderx, holdery, id;

            //create the door
            Movable_Object holderO;

            //First get the x,y position of the door
            loadedMap >> holder;
            id = stringToInt(holder);
            holderO.setSpriteDimensions(p_objectTypeSet[id].getDimensions());
            holderO.setSpriteLoc(p_objectTypeSet[id].getSpriteLoc());
            holderO.setSSNum(p_objectTypeSet[id].getSSnum());
            holderO.setCharMovable(p_objectTypeSet[id].getCharMovable());
            holderO.setPushingSpeed(p_objectTypeSet[id].getPushingSpeed());
            holderO.setLiftable(p_objectTypeSet[id].getLiftable());
            holderO.setDimensions(p_objectTypeSet[id].getSpriteDimensions());

            loadedMap >> holder;

            holderx = static_cast<int>((static_cast<float>(stringToFloat(holder))+.5)*static_cast<float>(tileSet[0].getTileDims().x));
            loadedMap >> holder;
            holdery = static_cast<int>((static_cast<float>(stringToFloat(holder))+.5)*static_cast<float>(tileSet[0].getTileDims().y));
            holderO.setPosition(sf::Vector2f(holderx, holdery));
            //add the door to the list
            p_allObjects.push_back(holderO);
            loadedMap >> holder;
        }
                while(holder!=startMovingObjectsFlag)
        {
            getline(loadedMap, holder);
        }
        loadedMap >> holder;
               while(holder!=endMovingObjectsFlag)
        {
            int holderx, holdery, id;
            Moving_Block holderMB;
            loadedMap >> holder;
            id = stringToInt(holder);
            holderMB.setSpriteDimensions(p_objectTypeSet[id].getDimensions());
            holderMB.setSpriteLoc(p_objectTypeSet[id].getSpriteLoc());
            holderMB.setSSNum(p_objectTypeSet[id].getSSnum());
            holderMB.setDimensions(p_objectTypeSet[id].getSpriteDimensions());

             loadedMap >> holder;
             holderx = stringToInt(holder);
             loadedMap >> holder;
             holdery = stringToInt(holder);
             holderMB.setSpeedAndBuffer(sf::Vector2f(holderx, holdery));
             loadedMap >> holder;
             if(holder == "0")
             {
                 holderMB.setCloud(false);
             }else
             {
                 holderMB.setCloud(true);
             }
             loadedMap >> holder;
             id = stringToInt(holder);
                std::vector<sf::Vector2f> path;
             for(int i =0; i<id ;i++)
             {
            loadedMap >> holder;
            holderx = static_cast<int>((static_cast<float>(stringToFloat(holder))+.5)*static_cast<float>(tileSet[0].getTileDims().x));
            loadedMap >> holder;
            holdery = static_cast<int>((static_cast<float>(stringToFloat(holder))+.5)*static_cast<float>(tileSet[0].getTileDims().y));
            if(i ==0 )
            {
                holderMB.setPosition(sf::Vector2f(holderx, holdery));
                holderMB.setVelocityX(0);
                holderMB.setVelocityY(0);
            }
            path.push_back(sf::Vector2f(holderx, holdery));
             }
             holderMB.setPath(path);
             p_allMovingObjects.push_back(holderMB);
             loadedMap >> holder;

        }
        while(holder != startFallingObjectsFlag)
        {
            getline(loadedMap, holder);
        }
        loadedMap >> holder;

        while(holder != endFallingObjectsFlag)
        {
             int holderx, holdery, id;
            loadedMap >> holder;
            Falling_Block holderFB;
            id = stringToInt(holder);
            holderFB.setSpriteDimensions(p_objectTypeSet[id].getDimensions());
            holderFB.setSpriteLoc(p_objectTypeSet[id].getSpriteLoc());
            holderFB.setSSNum(p_objectTypeSet[id].getSSnum());
            holderFB.setDimensions(p_objectTypeSet[id].getSpriteDimensions());
            loadedMap >> holder;
            holderx = static_cast<int>((static_cast<float>(stringToFloat(holder))+.5)*static_cast<float>(tileSet[0].getTileDims().x));
            loadedMap >> holder;
            holdery = static_cast<int>((static_cast<float>(stringToFloat(holder))+.5)*static_cast<float>(tileSet[0].getTileDims().y));
            loadedMap >> holder;
            holderFB.setPosition(sf::Vector2f(holderx, holdery));
            holderFB.setVelocityX(0);
            holderFB.setVelocityY(0);
            if(holder == "0")
             {
                 holderFB.setCloud(false);
             }else
             {
                 holderFB.setCloud(true);
             }
             loadedMap >> holder;
             id = stringToInt(holder);
             holderFB.setFramesTilFall(id);
             p_allFallingBlocks.push_back(holderFB);
             loadedMap >> holder;
        }

    }
}
