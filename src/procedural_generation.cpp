#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "procedural_generation.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "tile.h"
#include "character.h"
#include "ladderType.h"
#include "tileType.h"
#include "door.h"
#include "object_type.h"
#include "gameMap.h"
#include "movable.h"
#include "movable_object.h"
#include "tile_functions.h"
//Directions,   1 - left to right
//              2 - up to down
//              3 - right to left
//              4 - down to up

std::vector <std::vector<int> > generateMap(int width, int height, int direction, int minEndRooms, int seed)
{
    srand(seed);
    //Generate the map left to right then flip/rotate afterwards!
    int tempHeight,tempWidth;
    if(direction == 1 || direction == 3)
    {
        tempHeight = height;
        tempWidth = width;
    }else
    {
        tempWidth = height;
        tempHeight = width;
    }
    //Create the

    std::cout << tempHeight << "  " << tempWidth << std::endl;
    std::vector <std::vector<int> > tempMap, adjacentFreeSpaces;
    std::vector <std::vector<std::vector <bool> > > mapConnections;
    for(int y = 0; y<tempHeight;y++)
    {
        std::vector<int> temp;
        std::vector<std::vector<bool> > temp2;
        for(int x = 0;x<tempWidth;x++)
        {
            std::vector<bool> temp3;
            temp.push_back(0);
            for(int dir=0; dir<4; dir++)
            {
                temp3.push_back(false);
            }
            temp2.push_back(temp3);
        }
        mapConnections.push_back(temp2);
        tempMap.push_back(temp);
        adjacentFreeSpaces.push_back(temp);
    }
    //Choose the starting point
    int currentX = 0;
    int currentY = rand() % tempHeight;
    //up = 0
    //right = 1
    //down = 2
    int lastMove = 1;

    while(currentX < tempWidth)
    {
        //allowable directions
        bool directions[3] = {true, true ,true};

        //can't go back
        if(lastMove==0)
        {
            directions[2] = false;
        }else if(lastMove == 2)
        {
            directions[0] = false;
        }

        //can't go up or down if at top or bottom
        if(currentY==0)
        {
            directions[0] = false;
        }else if(currentY == (tempHeight-1))
        {
            directions[2] = false;
        }
        int counter = 0;
        for(int i=0;i<3;i++)
        {
            if(directions[i])
            {
                counter++;
            }
        }
        int nextDir = rand()%counter;
        counter = 0;
        for(int i=0;i<3;i++)
        {
            if(directions[i])
            {
                if(counter == nextDir)
                {
                    nextDir = i;
                    break;
                }
                counter++;
            }
        }
        lastMove = nextDir;

        //Link the current tile to the next
        mapConnections[currentY][currentX][lastMove+1] = true;
        tempMap[currentY][currentX] = 1;

        //move
        if(lastMove ==0)
        {
            currentY--;
            mapConnections[currentY][currentX][3] = true;
        }else if(lastMove == 1)
        {
            currentX++;
            if(currentX < tempWidth)
            {
                mapConnections[currentY][currentX][0] = true;
            }else
            {
                mapConnections[currentY][currentX-1][lastMove+1] = false;
            }
        }else if(lastMove == 2)
        {
            currentY++;
            mapConnections[currentY][currentX][1] = true;
        }
    }
    int totalPos = generateAdjacentFreeSpaces(tempMap, adjacentFreeSpaces);
    while(totalPos!=0)
    {
        int freeSpace = rand()%totalPos;
        addAnotherTile(mapConnections,tempMap,adjacentFreeSpaces,freeSpace);
        totalPos = generateAdjacentFreeSpaces(tempMap, adjacentFreeSpaces);
    }

    std::vector <std::vector<int> > metaMap = makeTheMetaMap(direction,mapConnections,tempMap);

    std::vector <std::vector<std::vector<std::vector<int> > > > mapSegments = getMapSegments();

    return generateMainFromMeta(metaMap, mapSegments);
}
void addAnotherTile(std::vector <std::vector<std::vector <bool> > > & mapConnections,std::vector <std::vector<int> > & tempMap,std::vector <std::vector<int> > & adjacentFreeSpaces, const int freeSpace)
{
    int height = tempMap.size();
    int width = tempMap[0].size();
    int counter = 0;
    int x_rand, y_rand, numDir;
    for(int x = 0;x<width;x++)
    {
        for(int y = 0; y<height;y++)
        {
            if(counter <= freeSpace )
            {
                numDir = freeSpace-counter;
                counter += adjacentFreeSpaces[y][x];
                x_rand = x; y_rand = y;
            }
        }
    }
   int dir = 0;
    counter = 0;
    if(x_rand!=0)
    {
        if(tempMap[y_rand][x_rand-1] ==0)
        {
            if(numDir == counter)
            {
                dir = 0;
            }
            counter++;
        }
    }
    if(x_rand!=(width-1))
    {
        if(tempMap[y_rand][x_rand+1] ==0)
        {
            if(numDir == counter)
            {
                dir = 2;
            }
            counter++;
        }
    }
    if(y_rand!=0)
    {
        if(tempMap[y_rand-1][x_rand] ==0)
        {
            if(numDir == counter)
            {
                dir = 1;
            }
            counter++;
        }
    }
    if(y_rand!=(height-1))
    {
        if(tempMap[y_rand+1][x_rand] ==0)
        {
            if(numDir == counter)
            {
                dir = 3;
            }
            counter++;
        }
    }
    int x_new,y_new;
    if(dir ==0)
    {
        x_new = x_rand-1;
        y_new = y_rand;
        tempMap[y_new][x_new] = 1;
        mapConnections[y_rand][x_rand][0] = true;
        mapConnections[y_new][x_new][2] = true;
    }
    if(dir ==2)
    {
        x_new = x_rand+1;
        y_new = y_rand;
        tempMap[y_new][x_new] = 1;
        mapConnections[y_rand][x_rand][2] = true;
        mapConnections[y_new][x_new][0] = true;
    }
    if(dir ==1)
    {
        x_new = x_rand;
        y_new = y_rand-1;
        tempMap[y_new][x_new] = 1;
        mapConnections[y_rand][x_rand][1] = true;
        mapConnections[y_new][x_new][3] = true;
    }
    if(dir ==3)
    {
        x_new = x_rand;
        y_new = y_rand+1;
        tempMap[y_new][x_new] = 1;
        mapConnections[y_rand][x_rand][3] = true;
        mapConnections[y_new][x_new][1] = true;
    }
}
int generateAdjacentFreeSpaces(std::vector <std::vector<int> > & tempMap,std::vector <std::vector<int> > & adjacentFreeSpaces)
{
    int height = tempMap.size();
    int width = tempMap[0].size();
    int counter;
        for(int y = 0; y<height;y++)
    {
        std::vector<int> temp;
        for(int x = 0;x<width;x++)
        {
            temp.push_back(0);
        }
        adjacentFreeSpaces.push_back(temp);
    }
    int totalPos = 0;
    for(int x = 0;x<width;x++)
    {
        for(int y = 0; y<height;y++)
        {
            counter = 0;
            if(tempMap[y][x] == 1)
            {
                //check all directions
                if(x!=0)
                {
                    if(tempMap[y][x-1] ==0)
                    {
                        counter++;
                    }
                }
                if(x!=(width-1))
                {
                    if(tempMap[y][x+1] ==0)
                    {
                        counter++;
                    }
                }
                if(y!=0)
                {
                    if(tempMap[y-1][x] ==0)
                    {
                        counter++;
                    }
                }
                if(y!=(height-1))
                {
                    if(tempMap[y+1][x] ==0)
                    {
                        counter++;
                    }
                }

            }
            totalPos += counter;
            adjacentFreeSpaces[y][x] = counter;
        }
    }
    return totalPos;
}

std::vector <std::vector<int> > makeTheMetaMap(const int direction,
                    std::vector <std::vector<std::vector <bool> > > & mapConnections,
                    std::vector <std::vector<int> > & tempMap)
{
    int l,r,u,d,lu,ur,rd,dl,dlu,lur,urd,rdl,lr,ud,lurd;
    std::vector <std::vector<int> >metaMap;
    if(direction==1)
    {
        l = 1;
        u = 2;
        r = 3;
        d = 4;
        lu = 5;
        ur = 6;
        rd = 7;
        dl = 8;
        dlu = 9;
        lur = 10;
        urd = 11;
        rdl = 12;
        lr = 13;
        ud = 14;
        lurd = 15;
    }else if(direction == 3)
    {
        l = 3;
        u = 2;
        r = 1;
        d = 4;
        lu = 6;
        ur = 5;
        rd = 8;
        dl = 7;
        dlu = 11;
        lur = 10;
        urd = 9;
        rdl = 12;
        lr = 13;
        ud = 14;
        lurd = 15;
    }else if(direction == 2)
    {
        l = 2;
        u = 1;
        r = 4;
        d = 3;
        lu = 5;
        ur = 8;
        rd = 7;
        dl = 6;
        dlu = 10;
        lur = 9;
        urd = 12;
        rdl = 11;
        lr = 14;
        ud = 13;
        lurd = 15;
    }else if(direction == 4)
    {
        l = 4;
        u = 3;
        r = 2;
        d = 1;
        lu = 7;
        ur = 6;
        rd = 5;
        dl = 8;
        dlu = 12;
        lur = 11;
        urd = 10;
        rdl = 9;
        lr = 14;
        ud = 13;
        lurd = 15;
    }
    for(int y=0;y<tempMap.size();y++)
    {
        for(int x = 0;x<tempMap[0].size();x++)
        {
            bool left = mapConnections[y][x][0];
            bool right = mapConnections[y][x][2];
            bool up =  mapConnections[y][x][1];
            bool down = mapConnections[y][x][3];
            if(left)
            {
                if(up)
                {
                    if(right)
                    {
                        if(down)
                        {
                            tempMap[y][x] = lurd;
                        }else
                        {
                            tempMap[y][x] = lur;
                        }

                    }else
                    {
                        if(down)
                        {
                            tempMap[y][x] = dlu;
                        }else
                        {
                            tempMap[y][x] = lu;
                        }
                    }

                }else
                {
                    if(right)
                    {
                        if(down)
                        {
                            tempMap[y][x]= rdl;
                        }else
                        {
                            tempMap[y][x] = lr;
                        }

                    }else
                    {
                        if(down)
                        {
                            tempMap[y][x] = dl;
                        }else
                        {
                            tempMap[y][x] = l;
                        }
                    }
                }

            }else
            {
                if(up)
                {
                    if(right)
                    {
                        if(down)
                        {
                            tempMap[y][x] = urd;
                        }else
                        {
                            tempMap[y][x] = ur;
                        }

                    }else
                    {
                        if(down)
                        {
                            tempMap[y][x] = ud;
                        }else
                        {
                            tempMap[y][x] = u;
                        }
                    }

                }else
                {
                    if(right)
                    {
                        if(down)
                        {
                            tempMap[y][x] = rd;
                        }else
                        {
                            tempMap[y][x] = r;
                        }

                    }else
                    {
                        if(down)
                        {
                            tempMap[y][x] = d;
                        }else{
                        std::cout<< "ERROR, BLANK ROOM" << std::endl;
                        }
                    }
                }
            }
        }
    }
    if(direction==1)
    {
        metaMap = tempMap;
    }else if(direction == 3)
    {
         for(int y=0;y<tempMap.size();y++)
        {
            std::vector<int> temp;
            for(int x =0;x<tempMap[0].size();x++)
            {
                temp.push_back(tempMap[y][tempMap[0].size()-1-x]);
                std::cout << tempMap.size()-1-x << std::endl;
            }
            metaMap.push_back(temp);
        }
    }else if(direction == 2)
    {
        for(int x =0;x<tempMap[0].size();x++)
        {
            std::vector<int> temp;
            for(int y=0;y<tempMap.size();y++)
            {
                temp.push_back(tempMap[y][x]);
            }
            metaMap.push_back(temp);
        }
    }else if(direction == 4)
    {
        for(int x =0;x<tempMap[0].size();x++)
        {
            std::vector<int> temp;
            for(int y=0;y<tempMap.size();y++)
            {
                temp.push_back(tempMap[tempMap.size() -1 - y][tempMap[0].size()-1-x]);
            }
            metaMap.push_back(temp);
        }
    }

    return metaMap;
}
std::vector <std::vector<std::vector<std::vector<int> > > > getMapSegments()
{
    std::vector <std::vector<std::vector<std::vector<int> > > > allSegments;
    std::ifstream loadedSeg("maps/mapSegments/segProps.txt");
    std::string endSegsFlag = "endSegments";
    int width, height;
    std::string inputs;
    loadedSeg >> inputs;
    loadedSeg >> inputs;
    width = stringToInt(inputs);
    loadedSeg >> inputs;
    loadedSeg >> inputs;
    height = stringToInt(inputs);
    for(int seg=0;seg<16;seg++)
    {

        std::vector <std::vector<std::vector<int> > > currentSegments;
        std::stringstream ss;
        ss << seg;
        std::string num = ss.str();
        std::string loc = "maps/mapSegments/"+num+".txt";
        std::ifstream loadedSeg(loc.c_str());
        loadedSeg >> inputs;
        while(inputs != endSegsFlag)
        {
            std::vector <std::vector<int > > currentSeg;
        for(int y = 0;y<height;y++)
        {
            std::vector <int> currentRow;
            for(int x = 0; x<width;x++)
            {
                currentRow.push_back(stringToInt(inputs));
                loadedSeg >> inputs;
            }
            currentSeg.push_back(currentRow);
        }
        loadedSeg >> inputs;
        currentSegments.push_back(currentSeg);
        }
        allSegments.push_back(currentSegments);
    }
    return allSegments;
}
std::vector<std::vector<int> > generateMainFromMeta(std::vector<std::vector<int> > metaMap, std::vector <std::vector<std::vector<std::vector<int> > > > mapSegments)
{
    std::vector<std::vector<int> >  finalMap;
    std::vector<std::vector<int> >  segmentsChoosen;
    int metaHeight = metaMap.size();
    int metaWidth = metaMap[0].size();
    int segHeight = mapSegments[0][0].size();
    int segWidth = mapSegments[0][0][0].size();
    //choose a random segment
        for(int metaY = 0;metaY<metaHeight;metaY++)
    {
                std::vector<int>  row;
        for(int metaX = 0; metaX<metaWidth;metaX++)
        {
            int segC = rand()%mapSegments[metaMap[metaY][metaX]].size();
            std::cout << segC << " ";
                        row.push_back(segC);
        }
        std::cout <<std::endl;
        segmentsChoosen.push_back(row);
    }
    for(int metaY = 0;metaY<metaHeight;metaY++)
    {
        for(int segY = 0;segY < segHeight;segY++)
{
        std::vector<int>  row;
        for(int metaX = 0; metaX<metaWidth;metaX++)
        {
            for(int segX = 0;segX<segWidth; segX++)
            {
                row.push_back(mapSegments[metaMap[metaY][metaX]][segmentsChoosen[metaY][metaX]][segY][segX]);
            }
        }
        finalMap.push_back(row);
        }
    }
    return finalMap;
}

