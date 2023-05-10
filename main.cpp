#include "ComonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "TextObject.h"
#include "Health.h"

BaseObject g_background;
SDL_Surface* bg1;
SDL_Surface* bg2;
TTF_Font* font_time = NULL;


bool Init()
{
    bool sc = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return false;
    }

    //tao window
    g_window = SDL_CreateWindow("GunAndJump", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(g_window == NULL) sc = false;
    else{
        //tao xuat
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL) sc = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            //cong khai anh
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags)) sc = false;
        }

        if(TTF_Init() == -1)//loi
        {
            sc = false;
        }

        font_time = TTF_OpenFont("font//dlxfont.ttf", 15);//ham tra ra la con tro
        if(font_time == NULL)
        {
            sc = false;
        }
    }
    return sc;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img/1.png", g_screen);
    return ret;
}

void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen); g_screen = NULL;
    SDL_DestroyWindow(g_window); g_window = NULL;

    IMG_Quit();
    SDL_Quit();

}

std::vector<ThreatsObject*> MakeThreadList()
{
    std::vector<ThreatsObject*> list_threats;

    ThreatsObject* dynamic_threats = new ThreatsObject[20];//20 con quai di chuyen
    for(int i=0;i<20;i++)
    {
        ThreatsObject* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("img//Threat_Walk.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i*500);
            p_threat->set_y_pos(200);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(1);//ham xu ly bug ket vao tuong

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationsPos(pos1, pos2);

            list_threats.push_back(p_threat);//cho quai vao vi tri
        }
    }



    ThreatsObject* threats_objs = new ThreatsObject[20];//20 con tinh~

    for(int i=0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threats_objs + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("img//Threat_Walk.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(700 + i*1200);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(0);//dung yen, xoa ham nay la chayj vo 1 cho trung tuong

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);//xuat


        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{
    int num_die = 0;//khoi tao so mang
    ImpTimer fps_timer;
    if(!Init()) return -1;
    if(!LoadBackground()) return -1;

    bg1 = IMG_Load("img/3he.png");
    bg2 = IMG_Load("img/3he.png");

    SDL_Texture* tex1 = SDL_CreateTextureFromSurface(g_screen, bg1);
    SDL_Texture* tex2 = SDL_CreateTextureFromSurface(g_screen, bg2);

    GameMap game_map;
    game_map.LoadMap("map/map02.txt");
    game_map.LoadTiles(g_screen);

    MainObject player;
    player.LoadImg("img/Walk1.png", g_screen);
    player.set_clips();



    SDL_Rect rec1 = {0, -150, bg1->w, bg1->h};
    SDL_Rect rec2 = {SCREEN_WIDTH, -150, bg2->w, bg2->h};

    std::vector<ThreatsObject*> threats_list = MakeThreadList();


    bool is_quit = false;
    while(!is_quit)
    {
        fps_timer.start();
        rec1.x -= LAYER1_SPEED;
        rec2.x -= LAYER1_SPEED;
        if(rec1.x <= - SCREEN_WIDTH) rec1.x = SCREEN_WIDTH;
        if(rec2.x <= - SCREEN_WIDTH) rec1.x = SCREEN_WIDTH;

        while(SDL_PollEvent(&g_event) != 0)
        {
            if(g_event.type == SDL_QUIT){
                is_quit = true;
            }
            player.HandleInputAction(g_event, g_screen);
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        SDL_RenderCopy(g_screen, tex1, NULL, &rec1);
        SDL_RenderCopy(g_screen, tex2, NULL, &rec2);
        Map map_data = game_map.getMap();

        player.HandleBullet(g_screen);
        player.SetMapXY(map_data.start_x, map_data.start_y);
        player.DoPlayer(map_data);
        player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        //time text
        TextObject time_game;
        time_game.SetColor(TextObject::WHITE_TEXT);

        TextObject mark_game;//dem tien va dem quai
        mark_game.SetColor(TextObject::WHITE_TEXT);
        Uint8 mark_value = 0;

        TextObject money_game;
        money_game.SetColor(TextObject::WHITE_TEXT);
        //show game time
        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 60 - time_val;

        if (val_time <= 0)
            {
                    is_quit = true;
                    break;
            }
        else
            {
                std::string str_val = std::to_string(val_time);//tg chuyen dong
                str_time += str_val;

                time_game.SetText(str_time);//hien thi tg game
                time_game.LoadFromRenderText(font_time, g_screen);//truyen font, text sang sur
                time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);//xuat sang goc ben phai
            }




        for (int i=0; i< threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x, map_data.start_y);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Show(g_screen);

                SDL_Rect rect_player = player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();//check dan quai
                for(int jj=0;jj<tBullet_list.size();++jj)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if(pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);//kiem tra xem nhan vat game trung dan k
                        if(bCol1)//bCOl laf quai vat de check collision
                        {
                            p_threat->RemoveBullet(jj);//xoa dan jj
                            break;
                        }
                    }
                }

                SDL_Rect rect_threat = p_threat->GetRectFrame();//lay vi tri threat
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);//lay vi tri
                if( bCol1 || bCol2 )
                {
                        num_die++;
                        if(num_die <= 1)
                        {
                            player.SetRect(0, 0);
                            player.set_revive_time(5);
                            SDL_Delay(1000);//theo milis
                            continue;
                        }
                        else
                        {
                            p_threat->Free();
                        close();
                        SDL_Quit();
                        return 0;
                        }

                }
            }
        }

        std::vector<BulletObject*> bullet_arr = player.get_bullet_list();//lay dan ra trong list
        for(int j=0;j<bullet_arr.size();++j)//kiem tra tung vien dan vca cham voi threats
        {
            BulletObject* p_bullet = bullet_arr.at(j);
            if(p_bullet != NULL)
            {
                for(int t=0;t<threats_list.size();++t)
                {
                    ThreatsObject* obj_threat = threats_list.at(t);
                    if(obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;//vi tri chieu ngang threats
                        tRect.y = obj_threat->GetRect().y;//doc
                        tRect.w = obj_threat->get_width_frame();//su dung frame load hieu ung dong(1 chieu)
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();
                        bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);//check va cham va remove dan
                        if(bCol)
                        {

                            player.RemoveBullet(j);
                            obj_threat->Free();//xoa obj trung dan
                            threats_list.erase(threats_list.begin() + t);//done
                         }

                    }
                }
            }
        }
        SDL_RenderPresent(g_screen);
        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;
        if(real_imp_time < time_one_frame){
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0) SDL_Delay(delay_time);
        }
    }

    for(int i=0;i<threats_list.size();i++)//huy bo cac con quai da chet
    {
        ThreatsObject* p_threat = threats_list.at(i);
        if(p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }

    threats_list.clear();
    close();
    return 0;
}
