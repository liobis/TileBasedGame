#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
int Movable::idCounter = 0;
int Movable::getId()
{
    return id;
}
void Movable::setSprite(sf::Sprite p_sprite)
{
    sprite = p_sprite;
}
sf::Sprite & Movable::getSprite()
{
    return sprite;
}

sf::Vector2f Movable::getSpriteDimensions()
{
    return spriteDims;
}

void Movable::setSpriteDimensions(sf::Vector2f p_spriteDims)
{
    spriteDims = p_spriteDims;
}
//Sets the character's position in the map
void Movable::setPosition(sf::Vector2f p_position)
{
    position = p_position;
}

//Sets the dimensions of the character
void Movable::setDimensions(sf::Vector2f p_dimensions)
{
    dimensions = p_dimensions ;
}
//Gets the character's positions
sf::Vector2f Movable::getPosition()
{
    return position;
}

//Gets the character's dimensions
sf::Vector2f Movable::getDimensions()
{
    return dimensions;
}

//Set the spriteSheet our character uses
void Movable::setSSNum(int p_ssNum)
{
    ssNum = p_ssNum;
}

//Get the spriteSheet number our character uses
int Movable::getSSNum()
{
    return ssNum;
}

//Get location on the spriteSheet our character's sprite is at
sf::Vector2u Movable::getSpriteLoc()
{
    return spriteLoc;
}

//Set the location on the spriteSheet where out character is at
void Movable::setSpriteLoc(sf::Vector2u p_spriteLoc)
{
    spriteLoc = p_spriteLoc;
}

//Return the current velocity of the character
sf::Vector2f Movable::getVelocity()
{
    return velocity;
}

//Sets the characters x velocity
void Movable::setVelocityX(float x)
{
    velocity.x = x;
}

//Sets the character's y velocity
void Movable::setVelocityY(float y)
{
    velocity.y = y;
}

//and the position of one of then (used in placement)
sf::Vector2i Movable::getCornerType(Corner C1, Corner C2,
                                    gameMap &map, sf::Vector2f &posc1,
                                    sf::Vector2f &posc2)
{
    posc1 = getCorner(C1);
    posc2 = getCorner(C2);

    //Get the x and y positions on the map of the tiles the corners are touching
    posc1.x = floor(posc1.x/map.getTile(0).getTileDims().x);
    posc1.y = floor(posc1.y/map.getTile(0).getTileDims().y);
    posc2.x = floor(posc2.x/map.getTile(0).getTileDims().x);
    posc2.y = floor(posc2.y/map.getTile(0).getTileDims().y);

    //Return the tileType
    sf::Vector2i output;
    output.x = map.getTile(map.tileNum((static_cast<int>(posc1.x)),(static_cast<int>(posc1.y)))).getTileType();
    output.y = map.getTile(map.tileNum((static_cast<int>(posc2.x)),(static_cast<int>(posc2.y)))).getTileType();
    return output;
}



//Takes in a corner and returns it's coordinated
sf::Vector2f Movable::getCorner(Corner p_corner)
{
    sf::Vector2f corner_Location = getPosition();
    //The -1's in the formulas are correction factors to account
    //for the fact the center is a pixel and not a coordinate
    switch (p_corner)
    {
        case CORNER_UPLEFT:
            corner_Location.x -= (getDimensions().x)/2;
            corner_Location.y -= (getDimensions().y)/2;
            break;
        case CORNER_DOWNLEFT:
            corner_Location.x -= (getDimensions().x)/2;
            corner_Location.y += getDimensions().y/2-0.01;
            break;
        case CORNER_UPRIGHT:
            corner_Location.x += getDimensions().x/2-0.01;
            corner_Location.y -= (getDimensions().y)/2;
            break;
        case CORNER_DOWNRIGHT:
            corner_Location.x += getDimensions().x/2-0.01;
            corner_Location.y += getDimensions().y/2-0.01;
            break;
    }
    return corner_Location;
}
bool Movable::checkForCollision(Movable obj1)
{

    if(round((getPosition().x + getDimensions().x/2)*100) > round((obj1.getPosition().x - obj1.getDimensions().x/2)*100) &&
       round((getPosition().x - getDimensions().x/2)*100) < round((obj1.getPosition().x + obj1.getDimensions().x/2)*100) &&
       round((getPosition().y + getDimensions().y/2)*100) > round((obj1.getPosition().y - obj1.getDimensions().y/2)*100) &&
       round((getPosition().y - getDimensions().y/2)*100) < round((obj1.getPosition().y + obj1.getDimensions().y/2)*100) &&
       obj1.getId() != id)
    {
        return true;
    }else
    {
        return false;
    }
}
Movable::Movable(){
    id = ++idCounter;
}
void Movable::death()
{
}
void Movable::setYBeforeMovement()
{
    yBeforeMovement = getPosition().y;
}
void Movable::setXBeforeMovement()
{
    xBeforeMovement = getPosition().x;
}
