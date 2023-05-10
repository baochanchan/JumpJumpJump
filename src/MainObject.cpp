#include "MainObject.h"

MainObject::MainObject()
{
    frame = 0;
    x_pos = y_pos = 0;
    x_val = y_val = 0;
    width_frame = height_frame = 0;
    status = -1;
    input_type.left = input_type.right = 0;
    input_type.down = input_type.up = input_type.jump = 0;
    on_ground = false;
    map_x = map_y = 0;
    revive_time = 0;
    money_count = 0;
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if(ret){
        //1 frame trong buc anh 8 frame
        width_frame = rect_.w / 6;
        height_frame = rect_.h;
    }
    return ret;
}

SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;//lay vi tri obj
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame;
    rect.h = height_frame;
    return rect;
}

void MainObject::set_clips()
{
    if(width_frame > 0 && height_frame > 0){
        for(int i = 0; i < 6; i++){
            frame_clip[i] = {width_frame * i, 0, width_frame, height_frame};
        }
    }
}

void MainObject::IncreaseMoney()
{
    money_count++;
}

void MainObject::Show(SDL_Renderer* des)
{
    if(status == WALK_LEFT){
        LoadImg("img/Walk2.png", des);
    }
    else{
        LoadImg("img/Walk1.png", des);
    }

    if(input_type.left == 1 || input_type.right == 1){
        frame++;
    }
    else frame = 0;

    if(frame >= 6) frame = 0;

    rect_.x = x_pos - map_x;
    rect_.y = y_pos - map_y;

    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame, height_frame};

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void MainObject::HandleInputAction(SDL_Event event, SDL_Renderer* screen)
{
    if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
        case SDLK_RIGHT:
            status = WALK_RIGHT;
            input_type.right = 1;
            input_type.left = 0;
            break;
        case SDLK_LEFT:
            status = WALK_LEFT;
            input_type.left = 1;
            input_type.right = 0;
            break;
        case SDLK_UP:
            input_type.jump = 1;
            break;
        }
    }
    else if(event.type == SDL_KEYUP){
        switch(event.key.keysym.sym){
        case SDLK_RIGHT:
            input_type.right = 0;
            break;
        case SDLK_LEFT:
            input_type.left = 0;
            break;
        case SDLK_SPACE:
            input_type.jump = 0;
            break;
        }
    }


    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(event.button.button == SDL_BUTTON_RIGHT)
        {
            input_type.jump = 1;
        }
        else if(event.button.button == SDL_BUTTON_LEFT)
    {
        BulletObject* p_bullet = new BulletObject();
        p_bullet -> set_bullet_type(BulletObject::LAZE_BULLET);
        p_bullet -> LoadImgBullet(screen);

        if(status == WALK_LEFT)
        {
            p_bullet -> set_bullet_dir(BulletObject::DIR_LEFT);
            p_bullet -> SetRect(this->rect_.x, rect_.y + height_frame*0.25);
        }
        else
        {
            p_bullet -> set_bullet_dir(BulletObject::DIR_RIGHT);
            p_bullet -> SetRect(this->rect_.x + width_frame -20, rect_.y + height_frame*0.25);
        }



        p_bullet -> set_x_val(20);
        p_bullet -> set_y_val(20);
        p_bullet -> set_is_move(true);

        p_bullet_list.push_back(p_bullet);
    }
    }
}
void MainObject::HandleBullet(SDL_Renderer* des)
{
    for(int i = 0; i < p_bullet_list.size(); i++)
    {
        BulletObject* p_bullet = p_bullet_list.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet -> get_is_move() == true)
            {
                p_bullet -> HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet -> Render(des);
            }
            else
            {
                p_bullet_list.erase(p_bullet_list.begin() + i);
                if(p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void MainObject::RemoveBullet(const int& idx)//ham xoa dan sau khi dc ban
{
    int size = p_bullet_list.size();
    if( size > 0 && idx < size )//check xem co dan k
    {
        BulletObject* p_bullet = p_bullet_list.at(idx);
        p_bullet_list.erase(p_bullet_list.begin() + idx);//xoa dan

        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;//tiep tuc xoa dan ben main
        }
    }
}

void MainObject::DoPlayer(Map& map_data)
{
    if(revive_time == 0)
    {
      x_val = 0;
    y_val += GRAVITY_SPEED;

    if(y_val >= MAX_FALL_SPEED){ y_val = MAX_FALL_SPEED;}

    if(input_type.left == 1){
        x_val -= PLAYER_SPEED;
    }
    else if(input_type.right == 1){
        x_val += PLAYER_SPEED;
    }

    if(input_type.jump == 1)
    {
        if(on_ground == true)
        {
            y_val = - PLAYER_JUMP;
        }
        on_ground = false;
        input_type.jump = 0;
    }
    CheckToMap(map_data); //kiem tra va chama
    CenterEntityOnMap(map_data);

    }
    if(revive_time > 0 )
    {
        revive_time --;
        if(revive_time == 0)
        {
            if(x_pos > 256)
            {
                x_pos -= 256;
                map_x -= 256;
            }
            else{
                x_pos = 0;
            }

            y_pos = 0;
            x_val = 0;
            y_val = 0;
        }

    }


}

void MainObject::CenterEntityOnMap(Map& map_data)
{
    map_data.start_x = x_pos - (SCREEN_WIDTH / 2);
    if(map_data.start_x < 0) map_data.start_x = 0;
    else if(map_data.start_x + SCREEN_WIDTH >= map_data.max_x)
        map_data.start_x = map_data.max_x - SCREEN_WIDTH;

    map_data.start_y = y_pos - (SCREEN_HEIGHT / 2);
    if(map_data.start_y < 0) map_data.start_y = 0;
    else if(map_data.start_y + SCREEN_HEIGHT >= map_data.max_y)
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
}

void MainObject::CheckToMap(Map& map_data)
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

            if(val1 == STATE_MONEY || val2 == STATE_MONEY)
            {
                val1 = map_data.tile[y1][x2] = 0;
                val2 = map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else
            {
                if(val1 != 0 || val2 != 0){
                x_pos = x2 * TILE_SIZE - (width_frame + 1);
                x_val = 0;
            }
            }

        }
        else if(x_val < 0)   //main object dang di chuyen ve ben phai
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            if( val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                IncreaseMoney();
            }
            else
            {
                if( val1 != 0 || val2 != 0){
                x_pos = (x1 + 1) * TILE_SIZE;
                x_val = 0;
            }
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
            if(val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else
            {
                if(val1 != 0 || val2 != 0)
                {
                    y_pos = y2 * TILE_SIZE;
                y_pos -= (height_frame);
                y_val = 0;
                on_ground = true;
                if(status == 0)
                {
                    status = WALK_RIGHT;
                }
                }
            }
        }
        else if(y_val < 0)   //main object dang di chuyen ve ben phai
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if(val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                IncreaseMoney();
            }
            else
            {
              if(val1 != 0 || val2 != 0)
              {
                  y_pos =(y1 + 1)*TILE_SIZE;
                  y_val = 0;
              }
            }
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

MainObject::~MainObject()
{

}
