//Ladders have a center and a width

#include "ladderType.h"
LadderType::LadderType(){}
LadderType::LadderType(int p_width, int p_center)
{
    setWidth(p_width);
    setCenter(p_center);
}

//sets the width of the ladder
    void LadderType::setWidth(int p_width)
    {
        width = p_width;
    }

//sets the center of the ladder
    void LadderType::setCenter(int p_center)
    {
        center = p_center;
    }

//gets the width of the ladder
    int LadderType::getWidth()
    {
        return width;
    }

//gets the center of the ladder
    int LadderType::getCenter()
    {
        return center;
    }
