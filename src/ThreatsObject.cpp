#include "ThreatsObject.h"
#include "ComonFunc.h"


ThreatsObject::ThreatsObject()
{
    width_frame = 0;
    height_frame = 0;
    x_val = 0;
    y_val = 0;
    x_pos = 0;
    y_pos = 0;
    on_ground = 0;
    revive_time = 0;
    frame = 0;
    animation_a = 0;
    animation_b = 0;
    input_type.left = 0;//ben trai
    type_move = STATIC_THREAT;
    //ctor
}

ThreatsObject::~ThreatsObject()
{

    //dtor
}

bool ThreatsObject::LoadImg(std::string path , SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret)
    {
        width_frame = rect_.w/THREAT_FRAME_NUM;
        height_frame = rect_.h;
    }
    return ret;
}

SDL_Rect ThreatsObject::GetRectFrame()
{
    SDL_Rect rect;//lay vi tri obj
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame;
    rect.h = height_frame;
    return rect;
}

void ThreatsObject::set_clips()
{
    if(width_frame > 0 && height_frame > 0)
    {
        for(int i = 0; i < THREAT_FRAME_NUM; i++){
            frame_clip[i] = {width_frame * i, 0, width_frame, height_frame};
        }
    }
}

void ThreatsObject::Show(SDL_Renderer* des)
{
    if(revive_time == 0)
    {
        rect_.x = x_pos - map_x;
        rect_.y = y_pos - map_y;
        frame ++;
        if(frame >= THREAT_FRAME_NUM)
        {
            frame = 0;
        }

        SDL_Rect* currentClip = &frame_clip[frame];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame, height_frame};
        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
    }
}

void ThreatsObject::DoPlayer(Map& gMap)
{
    if(revive_time == 0)
    {
        x_val = 0;
        y_val += THREAT_GRAVITY_SPEED;
        if(y_val >= THREAT_MAX_FALL_SPEED)
        {
            y_val = THREAT_MAX_FALL_SPEED;
        }

        if(input_type.left == 1)
        {
            x_val -= THREAT_SPEED;
        }
        else if(input_type.right == 1)
        {
            x_val += THREAT_SPEED;
        }

        CheckToMap(gMap);
    }
    else if(revive_time > 0)
    {
        revive_time --;
        if(revive_time == 0)
        {
            InitThreats();
        }
    }
}

void ThreatsObject::InitThreats()
{
            x_val = 0;
            y_val = 0;
            if(x_pos > 256)
            {
                x_pos -= 256;
                animation_a -= 256;
                animation_b -= 256;
            }
            else
            {
                x_pos = 0;
            }
            y_pos = 0;
            revive_time = 0;
            input_type.left = 1;
}

void ThreatsObject::RemoveBullet(const int& idx)//ham xoa dan sau khi dc ban
{
    int size = bullet_list.size();
    if( size > 0 && idx < size )//check xem co dan k
    {
        BulletObject* p_bullet = bullet_list.at(idx);
        bullet_list.erase(bullet_list.begin() + idx);//xoa dan

        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;//tiep tuc xoa dan ben main
        }
    }
}

void ThreatsObject::CheckToMap(Map& map_data)
{
    /* (y1, x1) ******** (y2, x1)
        *                   *
        *                   *
       (y1, x2) ******** (y2, x2) */
    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;

    //check horizontal
    int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;
    //o thu bao nhieu
    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;

    y1 = (y_pos) / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
        if(x_val > 0)   //main object dang di chuyen ve ben phai
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];



                if((val1 != 0 && val1 != STATE_MONEY) ||( val2 != 0 && val2 != STATE_MONEY))
            {
                x_pos = x2 * TILE_SIZE - (width_frame + 1);
                x_val = 0;
            }
}


        else if(x_val < 0)   //main object dang di chuyen ve ben phai
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];



                if(( val1 != 0 && val1 != STATE_MONEY) || (val2 != 0 && val2 != STATE_MONEY)){
                x_pos = (x1 + 1) * TILE_SIZE;
                x_val = 0;
            }

        }

    }

    //check vertical
    int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
    x1 = (x_pos) / TILE_SIZE;
    x2 = (x_pos + x_val + width_min) / TILE_SIZE;

    y1 = (y_pos + y_val) / TILE_SIZE;
    y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
        if(y_val > 0)   //main object dang di chuyen ve xuong
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];


                if((val1 != 0 && val1 != STATE_MONEY) || (val2 != 0 && val2 != STATE_MONEY))
                {
                    y_pos = y2 * TILE_SIZE;
                y_pos -= (height_frame);
                y_val = 0;
                on_ground = true;

                }
            }
        }
        else if(y_val < 0)   //main object dang di chuyen ve ben phai
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];



              if((val1 != 0 && val1 != STATE_MONEY) || (val2 != 0 && val2 != STATE_MONEY))
              {
                  y_pos =(y1 + 1)*TILE_SIZE;
                  y_val = 0;
              }


        }

        x_pos += x_val;
    y_pos += y_val;

    if(x_pos < 0) x_pos = 0;
    else if(x_pos + width_frame > map_data.max_x) x_pos = map_data.max_x - width_frame - 1;
    if(y_pos < 0) y_pos = 0;
    if(y_pos > map_data.max_y)
    {
        revive_time = 10;
    }

    }

void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
    if (type_move == STATIC_THREAT)
    {
        ;//dung yen
    }
    else
    {
        if(on_ground = true)//dung tren mat dat moi di chuyen
        {
            if(x_pos > animation_b)
            {
                input_type.left = 1;
                input_type.right = 0;
                LoadImg("img//Threat_Walk.png", screen);
            }
            else if(x_pos < animation_a)
            {
                input_type.left = 0;
                input_type.right = 1;
                LoadImg("img//Threat_Walk 2.png", screen);
            }
        }
        else
        {
            if(input_type.left == 1)
            {
                LoadImg("img//Threat Walk.png", screen);
            }
        }
    }


}

void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
    if(p_bullet != NULL)
    {
        p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
        bool ret = p_bullet->LoadImgBullet(screen);
        if(ret)
        {
        p_bullet->set_is_move(true);//di chuyen
        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        p_bullet->SetRect(x_pos + 20, y_pos + 10);
        p_bullet->set_x_val(15);
        bullet_list.push_back(p_bullet);
        }
    }
}

void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
    for(int i=0;i<bullet_list.size();i++)
    {
        BulletObject* p_bullet = bullet_list.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move())
            {
                p_bullet->HandleMove(x_limit, y_limit);
                p_bullet->Render(screen);
            }
            else
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(x_pos + 20, y_pos + 10);
            }
        }
    }
}



