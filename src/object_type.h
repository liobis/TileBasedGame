#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H
class Object_Type
{
private:
    sf::Vector2f dimensions;
    int ssNum;
    sf::Vector2u spriteLoc;
    sf::Vector2f spriteDims;
    bool char_movable;
    float pushingSpeed;
    bool liftable;
public:
    Object_Type();
    sf::Vector2f getDimensions();
    int getSSnum();
    sf::Vector2u getSpriteLoc();
    void setDimensions(sf::Vector2f p_dimensions);
    void setSSnum(int p_ssNum);
    void setSpriteLoc(sf::Vector2u spriteLoc);
    bool getCharMovable();
    void setCharMovable(bool p_charMovable);
    float getPushingSpeed();
void setPushingSpeed(float p_pushingSpeed);
bool getLiftable();
void setLiftable(bool p_liftable);
void setSpriteDimensions(sf::Vector2f p_spriteDims);
sf::Vector2f getSpriteDimensions();
};
#endif
