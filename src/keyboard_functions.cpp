
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "character.h"
#include "tile.h"
#include <fstream>
#include <iostream>
#include "keyboard_functions.h"
void checkKeyboard(Character & char1)
{

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        char1.setLeft(true);
    }else
    {
        char1.setLeft(false);
    }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        char1.setRight(true);
    }else{
    char1.setRight(false);
    }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        char1.setUp(true);
    }else
    {
        char1.setUp(false);
    }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        char1.setDown(true);
    }else{
    char1.setDown(false);
    }
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        char1.setSpace(true);
    }else{
    char1.setSpace(false);
    }
         if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        char1.setRun(true);
    }else{
    char1.setRun(false);
    }
}

