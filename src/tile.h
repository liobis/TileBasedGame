
#ifndef TILE_H
#define TILE_H
class Tile
{
private:
    //tiletype
    int tileType;
    sf::Vector2u spriteLoc;
    int ssNum;
    sf::Vector2u tileDims;

public:
Tile();
    Tile(int p_tileType, sf::Vector2u p_spriteLoc,int ssNum, sf::Vector2u p_tileDims);
    void setSSNum(int p_ssNum);
    void setSpriteLoc(sf::Vector2u p_spriteLoc);
    void setTileType (int p_tileType);
    int getTileType ();
    sf::Vector2u getSpriteLoc();
    int getSSNum();
    void setTileDims(sf::Vector2u p_tileDims);
    sf::Vector2u getTileDims();
};

#endif
