#pragma once

#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "ComonFunc.h"
#include "BaseObject.h"

#define MAX_TILE 20
//doi tuong luu hinh anh map
class TileMat : public BaseObject
{
public:
    TileMat(){;}
    ~TileMat(){;}
};

class GameMap
{
public:
    GameMap(){;}
    ~GameMap(){;}

    void LoadMap(char* name);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);
    Map getMap() {return game_map;}
    void SetMap(Map& map_data) {game_map = map_data;}
private:
    Map game_map;
    //so luong hinh anh phai dung den (dat, nuoc,...)
    TileMat tile_mat[MAX_TILE];
};

#endif // GAME_MAP_H
