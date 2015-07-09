#ifndef DOOR_H
#define DOOR_H
class Door
{
private:
    bool alreadyLoaded;
    sf::Vector2i doorLoc;
    sf::Vector2i newDoorLoc;
    int newDoorMapTypeNum;
    std::string mapType;
public:
    Door();
    Door(sf::Vector2i p_doorLoc, sf::Vector2i p_newDoorLoc,
         std::string p_mapType);
    bool getAlreadyLoaded();
    void setAlreadyLoaded(bool p_alreadyLoaded);
    sf::Vector2i getDoorLoc();
    void setDoorLoc(sf::Vector2i p_doorLoc);
    sf::Vector2i getNewDoorLoc();
    void setNewDoorLoc(sf::Vector2i p_newDoorLoc);
    int getNewDoorMapTypeNum();
    void setNewDoorMapTypeNum(int p_newDoorMapTypeNum);
    std::string getMapType();
    void setMapType(std::string p_mapType);
};
#endif
