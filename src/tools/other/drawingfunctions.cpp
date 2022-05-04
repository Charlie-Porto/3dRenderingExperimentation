#ifndef drawingfunctions_cpp
#define drawingfunctions_cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cmath>
#include <vector>
#include <iostream>

/* vector functions */
std::vector<std::vector<int>> append_vect(std::vector<std::vector<int>> a, std::vector<std::vector<int>> b)
{
    for (int i = 0; i < b.size(); i++)
    {   
        a.push_back(b[i]);
    }
    return a;
}

/*^^^^^^^^^^^^^^^^^^^^^^^^*/



/* DRAWING PIXELS */ 
void drawPixelList (std::vector<std::vector<int>> pixels, std::vector<int> color, SDL_Renderer* renderer) {
    
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);

    for (int i = 0; i < pixels.size(); i++) {
        SDL_RenderDrawPoint(renderer, pixels[i][0], pixels[i][1]);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}

void drawPixel (std::vector<int> pixel, int color[4], SDL_Renderer* renderer) {

    SDL_RenderDrawPoint(renderer, pixel[0], pixel[1]);
}



/* SQUARE DRAWING FUNCTION */
void drawSquare (int x_corner, int y_corner, int width, std::vector<int> color, SDL_Renderer* renderer) {
    
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_x = x_corner + j;
            int pixel_y = y_corner + i;
            SDL_RenderDrawPoint(renderer, pixel_x, pixel_y);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
/*^^^^^^^^^^^^^^^^*/







/* for Circle Drawing: */

std::vector<std::vector<int>> get_c_oct(int xc, int yc, int x, int y)
{
    std::vector<std::vector<int>> points;

    points.push_back({xc+x, yc+y});
    points.push_back({xc-x, yc+y});
    points.push_back({xc+x, yc-y});
    points.push_back({xc-x, yc-y});
    points.push_back({xc+y, yc+x});
    points.push_back({xc-y, yc+x});
    points.push_back({xc+y, yc-x});
    points.push_back({xc-y, yc-x});

    return points;
}

std::vector<std::vector<int>> rast_circle(int xc, int yc, int r) 
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    std::vector<std::vector<int>> points_list = get_c_oct(xc, yc, x, y);

    while (y >= x)
    {
        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        
        std::vector<std::vector<int>> new_points = get_c_oct(xc, yc, x, y);
        points_list = append_vect(points_list, new_points);
    }

    return points_list;
}

void DrawCircle(int xc, int yc, int r, std::vector<int> color, SDL_Renderer* renderer) {
  const std::vector<std::vector<int>> points = rast_circle(xc, yc, r);
  drawPixelList(points, color, renderer);
}

void DrawRectangle(int x, int y, int w, std::vector<int> color, SDL_Renderer* renderer) {
  SDL_Rect platform;
  platform.x = x;
  platform.y = y;
  platform.h = 10;
  platform.w = w;

  SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);

  // draw
  SDL_RenderDrawRect(renderer, &platform);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 250);
}


std::vector<std::vector<int>> fill_circle (int rad, int xc, int yc, std::vector<int> color, SDL_Renderer* renderer) // just get points
{
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
     std::vector<std::vector<int>> inner_pixels;

    int x = xc - rad;
    int y = yc - rad;

    int y_end = yc + rad;
    int x_end = xc + rad;


    for (int i = x; i < x_end; i++)
    {
        for (int j = y; j < y_end; j++)
        {
            if ((i - xc)*(i - xc) + (j - yc)*(j - yc) < (rad * rad))
            {   

                int i_sq = (i - xc)*(i - xc);
                int j_sq = (j - yc)*(j - yc);

                inner_pixels.push_back({i,j});
            }
        }
    }

    return inner_pixels;
}




#endif /* drawingfunctions_cpp */ 