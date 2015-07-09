#include <SFML/Graphics.hpp>
#include "object_type.h"
Object_Type::Object_Type()
{

}
    sf::Vector2f Object_Type::getDimensions()
    {
        return dimensions;
    }
    int Object_Type::getSSnum()
    {
        return ssNum;
    }
    sf::Vector2u Object_Type::getSpriteLoc()
    {
        return spriteLoc;
    }
    void Object_Type::setDimensions(sf::Vector2f p_dimensions)
    {
        dimensions = p_dimensions;
    }
    void Object_Type::setSSnum(int p_ssNum)
    {
        ssNum = p_ssNum;
    }
    void Object_Type::setSpriteLoc(sf::Vector2u p_spriteLoc)
    {
        spriteLoc = p_spriteLoc;
    }
    bool Object_Type::getCharMovable()
    {
        return char_movable;
    }
    void Object_Type::setCharMovable(bool p_charMovable)
    {
        char_movable = p_charMovable;
    }
    float Object_Type::getPushingSpeed()
{
    return pushingSpeed;
}
void Object_Type::setPushingSpeed(float p_pushingSpeed)
{
    pushingSpeed = p_pushingSpeed;
}
bool Object_Type::getLiftable()
{
    return liftable;
}
void Object_Type::setLiftable(bool p_liftable)
{
    liftable = p_liftable;
}
void Object_Type::setSpriteDimensions(sf::Vector2f p_spriteDims)
{
    spriteDims = p_spriteDims;
}
sf::Vector2f Object_Type::getSpriteDimensions()
{
    return spriteDims;
}
