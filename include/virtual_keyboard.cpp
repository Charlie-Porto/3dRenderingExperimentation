#ifndef virtual_keyboard_cpp
#define virtual_keyboard_cpp

#include <vector>

/* SDL Libraries */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* Other dependencies */
#include "../src/sdl_core/sim_manager.cpp"      // for events 

struct JoystickReport{
  bool R_pressed;
  bool L_pressed;
  bool Down_pressed;
  bool Up_pressed;
};

class VirtualKeyboard 
{
/* virtual keyboard. 'nuff said. */
public:
    VirtualKeyboard();
    ~VirtualKeyboard(){};

    JoystickReport check_buttons();

    /* key movement */ 
private:

  JoystickReport joystick_; 

}; 


VirtualKeyboard::VirtualKeyboard() {
    joystick_.R_pressed = false;
    joystick_.L_pressed = false;
    joystick_.Down_pressed = false;
    joystick_.Up_pressed = false;
}

JoystickReport VirtualKeyboard::check_buttons()
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
                    joystick_.R_pressed = true;
                    break;
                case SDLK_LEFT:
                    std::cout<<"PRESS: L" <<'\n';
                    joystick_.L_pressed = true;
                    break;
                case SDLK_UP:
                    std::cout<<"PRESS: UP" <<'\n';
                    joystick_.Up_pressed = true;
                    break;
                case SDLK_DOWN:
                    std::cout<<"PRESS: DOWN" <<'\n';
                    joystick_.Down_pressed = true;
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
                    joystick_.R_pressed = false;
                    break;
                case SDLK_LEFT:
                    std::cout<<"LIFT: L" <<'\n';
                    joystick_.L_pressed = false;
                    break;
                case SDLK_UP:
                    std::cout<<"LIFT: UP" <<'\n';
                    joystick_.Up_pressed = false;
                    break;
                case SDLK_DOWN:
                    std::cout<<"LIFT: DOWN" <<'\n';
                    joystick_.Down_pressed = false;
                    break;
                default:
                    break;
            }
        }
    }
    return joystick_; 
}



#endif /* virtual_keyboard_cpp */