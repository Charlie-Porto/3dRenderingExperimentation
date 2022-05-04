#ifndef virtual_keyboard_cpp
#define virtual_keyboard_cpp

#include <vector>

/* SDL Libraries */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* Other dependencies */
#include "../../sdl_core/sim_manager.cpp"      // for events 

class VirtualKeyboard 
{
    /* virtual keyboard. 'nuff said. */
    public:
        VirtualKeyboard();
        ~VirtualKeyboard(){};

        std::vector<bool> check_buttons();

        /* key movement */ 
        bool R_pressed;
        bool L_pressed;
        bool Down_pressed;
        bool Up_pressed;

}; 

VirtualKeyboard::VirtualKeyboard() 
{
    R_pressed = false;
    L_pressed = false;
    Down_pressed = false;
    Up_pressed = false;
}

std::vector<bool> VirtualKeyboard::check_buttons()
{
    /* poll all events */
    for (int i = 0; i < Simulation::frame_events.size(); i++)
    {
        /* check specifically for key press event */
        if ( Simulation::frame_events[i].type == SDL_KEYDOWN )
        {
            switch ( Simulation::frame_events[i].key.keysym.sym )
            {
                case SDLK_RIGHT:
                    std::cout<<"PRESS: R" <<'\n';
                    R_pressed = true;
                    break;
                case SDLK_LEFT:
                    std::cout<<"PRESS: L" <<'\n';
                    L_pressed = true;
                    break;
                case SDLK_UP:
                    std::cout<<"PRESS: UP" <<'\n';
                    Up_pressed = true;
                    break;
                case SDLK_DOWN:
                    std::cout<<"PRESS: DOWN" <<'\n';
                    Down_pressed = true;
                    break;
                default:
                    break;
            } 
        }

        else if ( Simulation::frame_events[i].type == SDL_KEYUP )
        {
            switch ( Simulation::frame_events[i].key.keysym.sym )
            {
                case SDLK_RIGHT:
                    std::cout<<"LIFT: R" <<'\n';
                    R_pressed = false;
                    break;
                case SDLK_LEFT:
                    std::cout<<"LIFT: L" <<'\n';
                    L_pressed = false;
                    break;
                case SDLK_UP:
                    std::cout<<"LIFT: UP" <<'\n';
                    Up_pressed = false;
                    break;
                case SDLK_DOWN:
                    std::cout<<"LIFT: DOWN" <<'\n';
                    Down_pressed = false;
                    break;
                default:
                    break;
            }
        }
    }
    return {R_pressed, L_pressed, Up_pressed, Down_pressed};
}



#endif /* virtual_keyboard_cpp */
