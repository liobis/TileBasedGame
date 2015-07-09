
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include "character.h"
#include "tile.h"
#include "tileType.h"
#include "ladderType.h"
#include "door.h"
#include <vector>
#include "keyboard_functions.h"
#include "joystick_functions.h"
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"
#include "moving_block.h"
#include "tile_functions.h"
#include "falling_block.h"
#include "loadMap.h"
#include "view_function.h"
#include "procedural_generation.h"
#include "update_objects.h"
#include "destroy_functions.h"
int main()
{
    //Clock used in determining FPS
    sf::Clock clock;
    clock.restart();
    int fps=0;

    //Initilize everything
   int  windowWidth, windowHeight, numSS,temp;
    bool vSync;
    loadSS_Tile_Nums(numSS,temp,temp,temp);

    //load the window and game properites
    getGameProperties(windowWidth, windowHeight, vSync);

    //create the windows
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tile Based Game");

    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(vSync);
    window.setPosition(sf::Vector2i(400,400));

    //create out first character (maybe load fron txt file later?)
   std::vector<Character> playerSet;
   playerSet.push_back( Character(sf::Vector2f(100,359),sf::Vector2f(30,50), 6.0f,10.0f,3.0f, 1,sf::Vector2u(0,0), sf::Vector2f(15,15),sf::Vector2f(6,15),sf::Vector2f(6,15)));
   playerSet.push_back( Character(sf::Vector2f(400,600),sf::Vector2f(30,50), 6.0f,10.0f,3.0f, 1,sf::Vector2u(0,0), sf::Vector2f(15,15),sf::Vector2f(6,15),sf::Vector2f(6,15)));
    std::string map1 ="maps/map2.txt";
    std::vector<gameMap> allMaps;
	loadGameMap(map1, allMaps);

    //Get textures
    std::vector<sf::Texture> textureSheet(numSS);
    loadSS(numSS, textureSheet);


       //create it's sprite and texture it
       {
   sf::Sprite player1_Sprite;
   player1_Sprite.setTexture(textureSheet[playerSet[0].getSSNum()]);
   player1_Sprite.setTextureRect(sf::IntRect(playerSet[0].getSpriteLoc().x*playerSet[0].getDimensions().x,playerSet[0].getSpriteLoc().y*playerSet[0].getDimensions().y,playerSet[0].getDimensions().x,playerSet[0].getDimensions().y));
   player1_Sprite.setOrigin((playerSet[0].getDimensions().x)/2,(playerSet[0].getDimensions().y)/2);
    playerSet[0].setSprite(player1_Sprite);
    playerSet[1].setSprite(player1_Sprite);
    playerSet[1].setPushingSpeed(100);
    playerSet[0].setPushingSpeed(100);
    playerSet[0].setLiftable(true);
   playerSet[1].setLiftable(true);
       }
    //Create all the sprites
    std::vector< std::vector< std::vector < sf::Sprite > > > spritesToDraw;
    spritesToDraw.push_back(updateMapSprites(allMaps[0],textureSheet));

    //Create player 1's view
    sf::View player1_view;
    player1_view.setSize(windowWidth,windowHeight);
    //Start the game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //check to see if any keys are press
        if(sf::Joystick::isConnected(0))
        {
            checkJoystick(0, playerSet[0]);
        }else{
        checkKeyboard(playerSet[0]);
        }
        checkKeyboard(playerSet[1]);

        //clear the window
        window.clear(sf::Color::White);

        //update the player based on movements and gravity
         updateAllObjects(window,allMaps,playerSet[0].getCurrentMap(),playerSet);

             playerSet[0].update(allMaps,spritesToDraw, textureSheet, playerSet);
              playerSet[1].update(allMaps,spritesToDraw, textureSheet, playerSet);
        if(allMaps[playerSet[0].getCurrentMap()].getKillList().size()>0)
        {
            grim_reaper(allMaps[playerSet[0].getCurrentMap()], playerSet);
        }
        setView_Scroll(player1_view, playerSet[0], allMaps[playerSet[0].getCurrentMap()]);
        window.setView(player1_view);
        //Draws only sprites on screen
        drawInViewSprites(window, player1_view,allMaps[playerSet[0].getCurrentMap()],spritesToDraw[playerSet[0].getCurrentMap()]);
      //Scrolls the view based on the player's locations
       drawAllObjects(window,allMaps,playerSet[0].getCurrentMap(),playerSet);
        window.draw(playerSet[0].getSprite());
       window.draw(playerSet[1].getSprite());

        window.display();
        //FPS counrter
        fps++;
        if(clock.getElapsedTime().asSeconds()>=1)
        {
            clock.restart();
            std::cout << fps << std::endl;
            fps=0;
        }

    }
    return 0;

}

