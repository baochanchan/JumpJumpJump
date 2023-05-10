#include "BulletObject.h"


BulletObject::BulletObject()
{
    x_val = 0;
    y_val = 0;
    is_move = false;
    bullet_type = SPHERE_BULLET;

}

BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;
    if(bullet_type == LAZE_BULLET)
    {
        ret =  LoadImg("img//laze_bullet.png", des);
    }
    else
    {
        ret = LoadImg("img//sphere_bullet.png", des);
    }

    return ret;
}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
    if(bullet_dir == DIR_RIGHT)
    {
        rect_.x += x_val;
        if(rect_.x > x_border)
        {
            is_move = false;
        }
    }
    else if(bullet_dir == DIR_LEFT)
    {
        rect_.x -= x_val;
        if(rect_.x < 0)
        {
            is_move = false;
        }
    }
    else if(bullet_dir == DIR_UP)
    {
        rect_.y -=y_val;
        if(rect_.y < 0)
        {
            is_move = false;
        }
    }
    else if(bullet_dir == DIR_UP_LEFT)
    {
        rect_.x -= x_val;
        if(rect_.x < 0)
        {
            is_move = false;
        }
        rect_.y -= y_val;
        if(rect_.y < 0)
        {
            is_move = false;
        }
    }
    else if(bullet_dir == DIR_UP_RIGHT)
    {
        rect_.x += x_val;
        if(rect_.x > x_border)
        {
            is_move = false;
        }
        rect_.y -= y_val;
        if(rect_.y < 0)
        {
            is_move = false;
        }
    }
}
