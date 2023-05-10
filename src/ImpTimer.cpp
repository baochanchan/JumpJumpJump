#include "ImpTimer.h"

ImpTimer::ImpTimer()
{
    start_tick = 0;
    paused_tick = 0;
    is_paused_ = false;
    is_started_ = false;

}

ImpTimer::~ImpTimer()
{
    //dtor
}

void ImpTimer::start()
{
    is_started_ = true;
    is_paused_ = false;
    start_tick = SDL_GetTicks();
}

void ImpTimer::stop()
{
    is_paused_ = false;
    is_started_ = false;
}

void ImpTimer::paused()
{
    if(is_started_ && !is_paused_)
    {
        is_paused_ = true;
        paused_tick = SDL_GetTicks() - start_tick;
    }
}


void ImpTimer::unpaused()
{
    if(is_paused_)
    {
        is_paused_ = false;
        start_tick = SDL_GetTicks() - paused_tick;
        paused_tick = 0;
    }
}

int ImpTimer::get_ticks()
{
    if(is_started_)
    {
        if(is_paused_) return paused_tick;
        else return SDL_GetTicks() - start_tick;
    }
}

bool ImpTimer::is_started()
{
    return is_started_;
}

bool ImpTimer::is_paused()
{
    return is_paused_;
}
