#ifndef THREATSOBJECT_H
#define THREATSOBJECT_H

#include "ComonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_FRAME_NUM 7
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3

class ThreatsObject : public BaseObject
{
    public:
        ThreatsObject();
        ~ThreatsObject();

        enum TypeMove
        {
            STATIC_THREAT = 0,
            MOVE_IN_SPACE_THREAT = 1, //di chuyen trong pham vi
        };
        void set_x_val(const float& xVal){x_val = xVal;}
        void set_y_val(const float& yVal){y_val = yVal;}

        void set_x_pos(const float& xp) {x_pos = xp;}
        void set_y_pos(const float& yp) {y_pos = yp;}
        float get_x_pos() const {return x_pos;}
        float get_y_pos() const {return y_pos;}
        void SetMapXY(const int& mp_x, const int& mp_y) {map_x = mp_x; map_y = mp_y;}

        void set_clips();
        bool LoadImg(std::string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des);
         int get_width_frame() const {return width_frame;}
         int get_height_frame() const {return height_frame;}
        void DoPlayer(Map& gMap);
        void CheckToMap(Map& gMap);
        void InitThreats();

        void set_type_move(const int typeMove) {type_move = typeMove;}
        void SetAnimationsPos(const int& pos_a, const int& pos_b) { animation_a = pos_a, animation_b = pos_b;}
        void ImpMoveType(SDL_Renderer* screen);//xu ly pham vi di chuyen
        void set_input_left(const int& ipLeft) {input_type.left = ipLeft;};
        SDL_Rect GetRectFrame();
        std::vector<BulletObject*> get_bullet_list() const {return bullet_list;}//ham lu ly dan cho quai
        void set_bullet_list(const std::vector<BulletObject*>& bl_list) {bullet_list = bl_list;}

        void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
        void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
        void RemoveBullet(const int& idx);//ham xoa dan

    protected:

    private:
        int map_x;
        int map_y;
        float y_val;
        float x_val;
        float x_pos;
        float y_pos;
        bool on_ground;
        int revive_time;
        SDL_Rect frame_clip[THREAT_FRAME_NUM];
        int width_frame;
        int height_frame;
        int frame;
        int type_move;
        int animation_a;//a tay trai
        int animation_b;//b tay phai
        Input input_type;//bien di chuyen

        std::vector<BulletObject*> bullet_list;
};

#endif // THREATSOBJECT_H

