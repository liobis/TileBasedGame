#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.h"
void checkJoystick(const int joystickID, Character & char1)
{
    int jumpButton = 0;
    int limitJS = 60;
    int limitDpad = 20;
    int limitTrigger = 60;
    if(sf::Joystick::isButtonPressed(joystickID,jumpButton))
    {
        char1.setSpace(true);
    }else
    {

        char1.setSpace(false);
    }
    if(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::PovY)>limitDpad||
       sf::Joystick::getAxisPosition(joystickID, sf::Joystick::X)>limitJS)
    {
            char1.setRight(true);

    }else{
    char1.setRight(false);
    }
        if(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::PovY)<-limitDpad||
       sf::Joystick::getAxisPosition(joystickID, sf::Joystick::X)<-limitJS)
    {
            char1.setLeft(true);

    }else{
    char1.setLeft(false);
    }
    if(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::PovX)>limitDpad||
       sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Y)<-limitJS)
    {
        char1.setUp(true);
    }else{
    char1.setUp(false);
    }
        if(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::PovX)<-limitDpad||
       sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Y)>limitJS)
    {
        char1.setDown(true);
    }else{
    char1.setDown(false);
    }

    if(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Z)<-limitTrigger||
       sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Z)>limitTrigger)
    {
        char1.setRun(true);
    }else{
    char1.setRun(false);
    }

}
