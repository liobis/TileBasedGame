#ifndef LADDERTYPE_H
#define LADDERTYPE_H
class LadderType
{
private:
    int width;
    int center;
public:
    LadderType();
    LadderType(int p_width, int p_center);
    void setWidth(int p_width);
    void setCenter(int p_center);
    int getWidth();
    int getCenter();

};
#endif
