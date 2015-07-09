#include <SFML/Graphics.hpp>
#include <string>
#include "door.h"

//Assume the first door's map has not already been loaded
    Door::Door():alreadyLoaded(false), newDoorMapTypeNum(0){}

    Door::Door(sf::Vector2i p_doorLoc, sf::Vector2i p_newDoorLoc,
          std::string p_mapType): alreadyLoaded(false),
          newDoorMapTypeNum(0)
         {
             setDoorLoc(p_doorLoc);
             setNewDoorLoc(p_newDoorLoc);
             setMapType(p_mapType);
         }

//Checks if the map it is linked to has been loaded
    bool Door::getAlreadyLoaded()
    {
        return alreadyLoaded;
    }
    //Set's the door to already loaded
    void Door::setAlreadyLoaded(bool p_alreadyLoaded)
    {
        alreadyLoaded = p_alreadyLoaded;
    }

    //get the x,y position of the door
    sf::Vector2i Door::getDoorLoc()
    {
        return doorLoc;
    }

    //set the x,y position of the door
    void Door::setDoorLoc(sf::Vector2i p_doorLoc)
    {
        doorLoc = p_doorLoc;
    }

    //get the location where it will spawn you in the new map
    sf::Vector2i Door::getNewDoorLoc()
    {
        return newDoorLoc;
    }

    //set the location where it wil spawn you in the new map
    void Door::setNewDoorLoc(sf::Vector2i p_newDoorLoc)
    {
        newDoorLoc = p_newDoorLoc;
    }

    //Set's the map we link to
    int Door::getNewDoorMapTypeNum()
    {
        return newDoorMapTypeNum;
    }

    //Set's the map we link to
    void Door::setNewDoorMapTypeNum(int p_newDoorMapTypeNum)
    {
        newDoorMapTypeNum = p_newDoorMapTypeNum;
    }

    //get's the location we loaded the map from
    std::string Door::getMapType()
    {
        return mapType;
    }

    //set's the location to load the map from
    void Door::setMapType(std::string p_mapType)
    {
        mapType = p_mapType;
    }
