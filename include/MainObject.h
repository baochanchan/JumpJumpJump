#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <vector>
#include "ComonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"


#define GRAVITY_SPEED 0.7
#define MAX_FALL_SPEED 15
#define PLAYER_SPEED 10
#define PLAYER_JUMP 14

class MainObject : public BaseObject
{
    public:
        MainObject();
        ~MainObject();

        enum WalkType{
            WALK_RIGHT = 0,
            WALK_LEFT = 1,
        };
        bool LoadImg(std::string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des);
        void HandleInputAction(SDL_Event event, SDL_Renderer* screen);
        void set_clips();

        void DoPlayer(Map& map_data);
        void CheckToMap(Map& map_data);
        void SetMapXY(const int map_x, const int map_y)
        {this->map_x = map_x; this->map_y = map_y;}
        void CenterEntityOnMap(Map& map_data);
        void UpdateImagePlayer(SDL_Renderer* des);
        SDL_Rect GetRectFrame();

        void set_bullet_list(std::vector<BulletObject*> bullet_list)
        {
            p_bullet_list = bullet_list;
        }
        std::vector<BulletObject*> get_bullet_list() const {return p_bullet_list;}
        void HandleBullet(SDL_Renderer* des);
        void IncreaseMoney();
        void RemoveBullet(const int& idx);//ham xoa dan
        void set_revive_time(const int& rv_time){revive_time = rv_time;}
        int GetMoneyCount() const {return money_count;}

        int get_x() {return x_pos - map_x;}
        int get_y() {return y_pos - map_y;}
    private:


        int money_count;
        std::vector<BulletObject*> p_bullet_list;
        float x_val;
        float y_val;

        float x_pos;
        float y_pos;

        int width_frame;
        int height_frame;

        SDL_Rect frame_clip[8];
        Input input_type;
        int frame;
        int status;
        bool on_ground;
        int map_x;
        int map_y;
        int revive_time;
};

#endif // MAINOBJECT_H
