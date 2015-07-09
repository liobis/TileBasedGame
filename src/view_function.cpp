
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "character.h"
#include "door.h"
#include <math.h>
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"

void setView_Scroll(sf::View &view, Character &character, gameMap &map)
{
    float height = view.getSize().y;
    float width  = view.getSize().x;
    float maxW = map.getWidth()*(map.getTile(0).getTileDims().x);
    float maxH = map.getHeight()*(map.getTile(0).getTileDims().y);
    float newX = 0;
    float newY = 0;

    if(character.getPosition().x < width/2)
    {
        newX = width/2;
    }else if(character.getPosition().x > maxW - width/2)
    {
        newX  = maxW - width/2;
    }else
    {
        newX = round(character.getPosition().x);
    }
        if(character.getPosition().y < height/2)
    {
        newY = height/2;
    }else if(character.getPosition().y > maxH - height/2)
    {
        newY  = maxH - height/2;
    }else
    {
        newY = round(character.getPosition().y);
    }
    view.setCenter(newX, newY);
}
void drawInViewSprites(sf::RenderWindow &window, sf::View &view,gameMap &map, std::vector< std::vector < sf::Sprite > > &sprites )
{
    int leftX = floor((view.getCenter().x - view.getSize().x/2)/(map.getTile(0).getTileDims().x));
    int rightX = ceil((view.getCenter().x + view.getSize().x/2)/(map.getTile(0).getTileDims().x));
    int upY = floor((view.getCenter().y - view.getSize().y/2)/(map.getTile(0).getTileDims().y));
    int downY = ceil((view.getCenter().y + view.getSize().y/2)/(map.getTile(0).getTileDims().y));
for(int y = upY; y<downY; y++)
{
    for(int x = leftX; x<rightX; x++)
    {
        window.draw(sprites[y][x]);
    }
}
}
