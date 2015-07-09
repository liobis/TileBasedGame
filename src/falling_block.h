
#ifndef FALLINGBLOCK_H
#define FALLINGBLOCK_H
class Falling_Block : public Movable_Object
{
private:
    bool cloud;
    int framesTilFall;
    bool collision;
    int currentFrame;
    bool falling;
public:
    Falling_Block();
    void update(std::vector<gameMap> &allMaps, int currentMap,std::vector<Character> & playerSet);
        bool getCollision();
        void setCollision(bool p_col);
        void setFramesTilFall(int p_framesTilFall);
        bool getCloud();
        void setCloud(bool p_cloud);
};
#endif
