#ifndef PRECEDURALGENERATION_H
#define PRECEDURALGENERATION_H
void addAnotherTile(std::vector <std::vector<std::vector <bool> > > & mapConnections,std::vector <std::vector<int> > & tempMap,std::vector <std::vector<int> > & adjacentFreeSpaces, const int freeSpace);
int generateAdjacentFreeSpaces(std::vector <std::vector<int> > & tempMap,std::vector <std::vector<int> > & adjacentFreeSpaces);
std::vector <std::vector<int> > generateMap(int width, int height, int direction, int minEndRooms, int seed);
std::vector <std::vector<int> > makeTheMetaMap(const int direction,
                    std::vector <std::vector<std::vector <bool> > > & mapConnections,
                    std::vector <std::vector<int> > & tempMap);
std::vector <std::vector<std::vector<std::vector<int> > > > getMapSegments();
std::vector<std::vector<int> > generateMainFromMeta(std::vector<std::vector<int> > metaMap, std::vector <std::vector<std::vector<std::vector<int> > > > mapSegments);
#endif
