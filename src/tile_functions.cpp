
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include <fstream>
#include <iostream>
#include "object_type.h"
#include "tile_functions.h"
int stringToInt(std::string convert)
{
    std::stringstream convertss(convert);
    int result;
    if (!(convertss >> result) )
    {
        result = 0;
    }
    return result;

}

float stringToFloat(std::string convert)
{
    std::stringstream convertss(convert);
    float result;
    if (!(convertss >> result) )
    {
        result = 0;
    }
    return result;

}
void loadSS(const int &numOfSpriteSheets,std::vector<sf::Texture> &spriteSheet)
{
    std::ifstream tileProps("gameproperties/tileproperties.txt");
    std::string inputs,spriteLoc;
    for(int ssNum = 0;ssNum<numOfSpriteSheets;ssNum++)
    {
    tileProps >> inputs;
    tileProps >> spriteLoc;
    if(!spriteSheet[ssNum].loadFromFile(spriteLoc))
    {
        std::cout << "ERROR LOADING SPRITE SHEET" << std::endl;
    }
    tileProps >> inputs;
    tileProps >> inputs;
    //Tile Width
    tileProps >> inputs;
    tileProps >> inputs;
    }
}
void loadEverything(const int &numOfSpriteSheets, const int &numOfTiles,
                    const int &numOfTileTypes, const int &numOfLadderTypes,
                    std::vector<Tile> &tileSet,
                    std::vector<TileType> &tileTypeSet,
                    std::vector<LadderType> &ladderTypeSet)
{
    //Open a file stream to get tile information from a txt file
    std::ifstream tileProps("gameproperties/tileproperties.txt");
	std::vector<int> tileWidth(numOfSpriteSheets), tileHeight(numOfSpriteSheets);

//If we were able ro open the properties files, then get the data fro mit
         if(tileProps.is_open())
    {

int tempx, tempy;

//Inputs stores the current string of the properties files

    std::string inputs,spriteLoc;
    for(int ssNum = 0;ssNum<numOfSpriteSheets;ssNum++)
    {

        tileProps >> inputs;

    tileProps >> inputs;
        tileProps >> inputs;
    tileProps >> inputs;
    tileHeight[ssNum] = stringToInt(inputs);
    //Tile Width
    tileProps >> inputs;
    tileProps >> inputs;
    tileWidth[ssNum] = stringToInt(inputs);
    }

    //Tile Height

tileProps >> inputs;
    //Create all the tiles with their properties
    for(int tileNum =0;tileNum<numOfTiles;tileNum++)
    {
        tileProps >> inputs;
    tileProps >> inputs;
        //Get the ID of the tile
        tileProps >> inputs;
    tileProps >> inputs;
    int id = stringToInt(inputs);
    //Get the type of tile
    tileProps >> inputs;
    tileProps >> inputs;
    tileSet[id].setTileType(stringToInt(inputs));
    //Get the Sprite Sheet
        tileProps >> inputs;
    tileProps >> inputs;
    tileSet[id].setSSNum(stringToInt(inputs));
    tileSet[id].setTileDims(sf:: Vector2u(tileWidth[stringToInt(inputs)], tileHeight[stringToInt(inputs)]));
    //Create the sprite for the tile
        tileProps >> inputs;

    tileProps >> tempx;
    tileProps >> tempy;
    tileSet[id].setSpriteLoc(sf::Vector2u(tempx, tempy));
    }
    tileProps >> inputs;
    for(int tileTypeNum = 0;tileTypeNum<numOfTileTypes;tileTypeNum++)
    {
        tileProps >> inputs;
    tileProps >> inputs;
        tileProps >> inputs;
    tileProps >> inputs;
    int id = stringToInt(inputs);
     tileProps >> inputs;
    tileProps >> inputs;
    if(inputs=="0")
    {
    tileTypeSet[id].setWalkable(false);
    }else
    {
        tileTypeSet[id].setWalkable(true);
    }
     tileProps >> inputs;
    tileProps >> inputs;
    tileTypeSet[id].setLadder(stringToInt(inputs));
       tileProps >> inputs;
    tileProps >> inputs;
    tileTypeSet[id].setCloud(stringToInt(inputs));
           tileProps >> inputs;
    tileProps >> inputs;
        if(inputs=="0")
    {
    tileTypeSet[id].setDoor(false);
    }else{
    tileTypeSet[id].setDoor(true);
    }
    tileProps >> inputs;
    tileProps >> inputs;
        if(inputs=="0")
    {
    tileTypeSet[id].setWater(false);
    }else{
    tileTypeSet[id].setWater(true);
    }
    tileProps >> inputs;
    tileProps >> inputs;
        if(inputs=="0")
    {
    tileTypeSet[id].setDeadly(false);
    }else{
    tileTypeSet[id].setDeadly(true);
    }
    tileProps >> inputs;
    tileProps >> inputs;
    tileTypeSet[id].setSlanted(inputs);
    }


      tileProps >> inputs;
      for(int ladderTypeNum = 0;ladderTypeNum < numOfLadderTypes;ladderTypeNum++)
      {
          tileProps >> inputs;
    tileProps >> inputs;
                  tileProps >> inputs;
    tileProps >> inputs;
    int id = stringToInt(inputs);
     tileProps >> inputs;
    tileProps >> inputs;
  ladderTypeSet[id].setWidth(stringToInt(inputs));
  tileProps >> inputs;
    tileProps >> inputs;
  ladderTypeSet[id].setCenter(stringToInt(inputs));
      }
      }
}
void loadSS_Tile_Nums(int &p_numSS, int &p_tileNum, int &p_tileTypeNum, int &p_ladderTypeNum)
{
    std::string inputs;
    std::string ssFlag = "spriteSheet:";
    std::string ssEndFlag = "endSS";
    std::string tileFlag = "tileid:";
    std::string tileEndFlag = "endTiles";
    std::string tileTypeFlag = "tiletype:";
    std::string tileTypeEndFlag = "endTileTypes";
    std::string ladderTypeFlag = "ladderType:";
    std::string ladderTypeEndFlag = "endLadderTypes";
     std::ifstream tileProps("gameproperties/tileproperties.txt");
     p_numSS =0;
     p_tileNum = 0;
     p_tileTypeNum = 0;
     p_ladderTypeNum = 0;
     if(tileProps.is_open())
    {
        tileProps >> inputs;
        while(inputs!=ssEndFlag)
              {
if(inputs == ssFlag)
{

        p_numSS++;
}
        tileProps >> inputs;
              }

        tileProps >> inputs;
             while(inputs!=tileEndFlag)
              {
        if(inputs == tileFlag)
        {
        p_tileNum++;
        }
                tileProps >> inputs;
              }
                           while(inputs!=tileTypeEndFlag)
              {
        if(inputs == tileTypeFlag)
        {
        p_tileTypeNum++;
        }
                tileProps >> inputs;
              }
        while(inputs!=ladderTypeEndFlag)
              {
        if(inputs == ladderTypeFlag)
        {
        p_ladderTypeNum++;
        }
                tileProps >> inputs;
              }
    }
}
void getGameProperties(int &p_windowWidth, int &p_windowHeight, bool &p_vSync)
{
    std::string inputs;
    std::string sWFlag = "ScreenWidth:";
    std::string sHFlag = "ScreenHeight:";
    std::string vSFlag = "Vsync:";
    std::ifstream gameProps("gameproperties/gameproperties.txt");
    if(gameProps.is_open())
    {
        gameProps >> inputs;
        while(!gameProps.eof())
        {
            if(inputs==sWFlag)
            {


                gameProps >> inputs;
                p_windowWidth = stringToInt(inputs);
            }else if(inputs==sHFlag)
                     {


                gameProps >> inputs;
                p_windowHeight= stringToInt(inputs);
                     }else if(inputs==vSFlag)
                     {


                gameProps >> inputs;
                if(inputs == "On")
                {
                    p_vSync = true;
                }else
                {
                    p_vSync = false;
                }
                     }else{
                     std::cout << "Error in Loading Window Properties" << std::endl;
                     gameProps >> inputs;
                     }
                     gameProps >> inputs;
            }
        }
    }


void loadObjectTypes(std::vector<Object_Type> & objectTypeSet)
{
    std::ifstream objectProps("gameproperties/objectproperties.txt");
    std::string objectTypeEndFlag = "endObjectTypes";
          if(objectProps.is_open())
    {
std::string inputs;
int tempx, tempy;
objectProps >> inputs;
while(inputs != objectTypeEndFlag)
{
    Object_Type tempObject;
    objectProps >> inputs;
    objectProps >> inputs;
    objectProps >> inputs;
    objectProps >> inputs;
    objectProps >> inputs;
    tempx = stringToInt(inputs);
    objectProps >> inputs;
    tempy = stringToInt(inputs);
    tempObject.setDimensions(sf::Vector2f(tempx,tempy));
        objectProps >> inputs;
        objectProps >> inputs;
    tempx = stringToInt(inputs);
    objectProps >> inputs;
    tempy = stringToInt(inputs);
    tempObject.setSpriteDimensions(sf::Vector2f(tempx,tempy));
        objectProps >> inputs;
    objectProps >> inputs;
    tempObject.setSSnum(stringToInt(inputs));

        objectProps >> inputs;
    objectProps >> inputs;
    tempx = stringToInt(inputs);
    objectProps >> inputs;
    tempy = stringToInt(inputs);
    tempObject.setSpriteLoc(sf::Vector2u(tempx,tempy));
    objectProps >> inputs;
            objectProps >> inputs;
            if(inputs == "0")
            {
                tempObject.setCharMovable(false);
            }else
            {
                tempObject.setCharMovable(true);
            }
                    objectProps >> inputs;
    objectProps >> inputs;
    tempObject.setPushingSpeed(stringToInt(inputs));
        objectProps >> inputs;
            objectProps >> inputs;
            if(inputs == "0")
            {
                tempObject.setLiftable(false);
            }else
            {
                tempObject.setLiftable(true);
            }
    objectProps >> inputs;
    objectTypeSet.push_back(tempObject);

}
    }
}

