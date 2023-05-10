#include "game_map.h"
#include <fstream>
#include <iostream>
#include <sstream>

void GameMap::LoadMap(char* name)
{
    std::ifstream fp;
    fp.open(name);
    if(!fp.is_open()){
        return;
    }
    game_map.max_x = 0;
    game_map.max_y = 0;
    for (int i = 0; i < MAX_MAP_Y; i++)
    {
        for (int j = 0; j < MAX_MAP_X; j++)
        {
            int x;
            fp >> x;
            game_map.tile[i][j] = x;
            //int val = game_map.tile[i][j];
            if(x > 0)
            {   //tim max_x va max_y chuan cho map
                if(j > game_map.max_x){
                    game_map.max_x = j;
                }
                if(i > game_map.max_y){
                    game_map.max_y = i;
                }
            }
        }
    }
    game_map.max_x = (game_map.max_x + 1) * TILE_SIZE;
    game_map.max_y = (game_map.max_y + 1) * TILE_SIZE;

    game_map.start_x = 0;
    game_map.start_y = 0;

    game_map.file_name = name;
    fp.close();
}

void GameMap::LoadTiles(SDL_Renderer* screen)
{
    for (int i = 0; i < MAX_TILE; i++)
    {
        std::stringstream ss;
        ss << "map/" << i << ".png";
        tile_mat[i].LoadImg(ss.str(), screen);
    }
}

void GameMap::DrawMap(SDL_Renderer* screen )
{
    int x1 = 0; //vi tri trai man hinh
    int x2 = 0; //vi tri phai man hinh
    int y1 = 0; //vi tri tren man hinh
    int y2 = 0; //vi tri cuoi man hinh
    //o thu bao nhieu
    int map_x = 0;
    int map_y = 0;

    x1 = (game_map.start_x % TILE_SIZE) * (-1);
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    y1 = (game_map.start_y % TILE_SIZE) * (-1);
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    //map_x = game_map.start_x / TILE_SIZE;
    //map_y = game_map.start_y / TILE_SIZE;

    for(int i = y1; i < y2; i += TILE_SIZE)
    {   //render theo hang
        map_x = game_map.start_x / TILE_SIZE;
        for(int j = x1; j < x2; j += TILE_SIZE)
        {   //render theo cot
            int val = game_map.tile[map_y][map_x];
            if (val > 0)
            {
                tile_mat[val].SetRect(j, i);
                tile_mat[val].Render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}
