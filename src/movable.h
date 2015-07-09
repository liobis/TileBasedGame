#ifndef MOVABLE_H
#define MOVABLE_H
class Movable
{
protected:
            //Center Position of Chacter
    sf::Vector2f position;
    sf::Vector2f dimensions;
    int ssNum;
    sf::Vector2u spriteLoc;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    int id;
    static int idCounter;
    float yBeforeMovement,xBeforeMovement;
    sf::Vector2f spriteDims;
public:
    Movable();
         enum Corner {CORNER_UPLEFT,CORNER_UPRIGHT,CORNER_DOWNLEFT,CORNER_DOWNRIGHT};
void setPosition(sf::Vector2f p_position);
    void setDimensions(sf::Vector2f p_dimensions);
    void setSprite(sf::Sprite p_sprite);
    sf::Sprite & getSprite();
    sf::Vector2f getPosition();
    sf::Vector2f getDimensions();
    int getSSNum();
    void setSSNum(int p_ssNum);
    void setSpriteLoc(sf::Vector2u p_spriteLoc);
    sf::Vector2f getVelocity();
    void setVelocityX(float x);
    void setVelocityY(float y);
    sf::Vector2u getSpriteLoc();
    sf::Vector2f getCorner(Corner p_corner);
    int getId();
    bool checkForCollision(Movable obj1);
    sf::Vector2i getCornerType(Corner C1, Corner C2, gameMap &map, sf::Vector2f &posc1,sf::Vector2f &posc2);
    void death();
    void setYBeforeMovement();
    void setXBeforeMovement();
    sf::Vector2f getSpriteDimensions();
    void setSpriteDimensions(sf::Vector2f p_spriteDims);

};
#endif
