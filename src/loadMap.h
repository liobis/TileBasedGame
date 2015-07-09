
#ifndef LOADMAP_H
#define LOADMAP_H
std::vector< std::vector<int> > loadMap(std::string const &loc);
std::map< int ,std::map < int,Door > > loadDoors(std::string const &loc,
                                                 std::vector<Door> &p_fullDoorSet);
std::vector< std::vector < sf::Sprite > > updateMapSprites(gameMap &map,
                                                           std::vector<sf::Texture> &textureSheet);
void loadGameMap(std::string loc,std::vector<gameMap> &allMaps);
void loadMovableObjects(std::string const &loc,
                         std::vector<Movable_Object> &p_allObjects,
                         std::vector<Moving_Block> &p_allMovingObjects,
                         std::vector<Falling_Block> &p_allFallingBlocks,
                         std::vector<Object_Type> & p_objectTypeSet,
                         std::vector<Tile> & tileSet);
#endif
