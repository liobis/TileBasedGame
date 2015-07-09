#ifndef MOVINGBLOCK_H
#define MOVINGBLOCK_H
class Moving_Block : public Movable_Object
{
private:
    std::vector<sf::Vector2f> path;
    bool cloud;
    int currentPath, nextOne;
    sf::Vector2f speedAndBuffer;
    int frameBuffer;
    float unAccel;
    int numFrames;
    int maxFrames;
    std::string dir;
public:
    Moving_Block();
    void setCloud(bool p_cloud);
    bool getCloud();
    void setCurrentPath(int p_path);
    int getCurrentPath();
    std::vector<sf::Vector2f> getPath();
    void setPath(std::vector<sf::Vector2f> p_path);
    sf::Vector2f getSpeedAndBuffer();
    void setSpeedAndBuffer(sf::Vector2f p_SpeedAndBuffer);
    void update(std::vector<gameMap> &allMaps,
                            std::vector<Character> & playerSet,
                             int &currentMap,
                             std::vector<int> & alreadyCheckedx,
                             std::vector<int> & alreadyCheckedy);
void update(std::vector<gameMap> &allMaps, std::vector<Character> & playerSet,
             int &currentMap,
             std::vector<int> & alreadyCheckedx,
             std::vector<int> & alreadyCheckedy, bool withGrav,
            bool checkLeft, bool checkRight,
            bool checkUp, bool checkDown);
            };
#endif
