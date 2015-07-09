#ifndef VIEWFUNCTION_H
#define VIEWFUNCTION_H
void setView_Scroll(sf::View &view, Character &character, gameMap &map);
void drawInViewSprites(sf::RenderWindow &window,
                       sf::View &view,gameMap &map,
                       std::vector< std::vector < sf::Sprite > > &sprites );
#endif
