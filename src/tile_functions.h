#ifndef TILEFUNCTIONS_H
#define TILEFUNCTIONS_H
int stringToInt(std::string convert);
float stringToFloat(std::string convert);
void loadEverything(const int &numOfSpriteSheets, const int &numOfTiles,
                    const int &numOfTileTypes, const int &numOfLadderTypes,
                    std::vector<Tile> &tileSet,
                    std::vector<TileType> &tileTypeSet,
                    std::vector<LadderType> &ladderTypeSet);
void loadSS_Tile_Nums(int &p_numSS, int&p_tile_num, int &p_tileTypeNum,
                      int &p_ladderTypeNum);
void getGameProperties(int &p_windowWidth, int &p_windowHeight,
                       bool &p_vSync);
void loadSS(const int &numOfSpriteSheets,std::vector<sf::Texture> &spriteSheet);
void loadObjectTypes(std::vector<Object_Type> & objectTypeSet);
#endif
