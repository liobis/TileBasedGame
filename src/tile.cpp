
#include <SFML/Graphics.hpp>
#include "tile.h"
Tile::Tile()
{

}
    Tile::Tile(int p_tileType, sf::Vector2u p_spriteLoc,int p_ssNum, sf::Vector2u p_tileDims )
    {
        setSpriteLoc(p_spriteLoc);
        setTileType(p_tileType);
        setSSNum(p_ssNum);
        setTileDims(p_tileDims);
    }
    void Tile::setSpriteLoc(sf::Vector2u p_spriteLoc)
    {
        spriteLoc = p_spriteLoc;
    }
    void Tile::setTileType (int p_tileType)
    {
        tileType = p_tileType;
    }
    int Tile::getTileType ()
    {
        return tileType;
    }
    sf::Vector2u Tile::getSpriteLoc()
    {
        return spriteLoc;
    }
    void Tile::setSSNum(int p_ssNum)
    {
        ssNum = p_ssNum;
    }
    int Tile::getSSNum()
    {
        return ssNum;
    }
    void Tile::setTileDims(sf::Vector2u p_tileDims)
                     {
                         tileDims = p_tileDims;
                     }
    sf::Vector2u Tile::getTileDims()
    {
        return tileDims;
    }

