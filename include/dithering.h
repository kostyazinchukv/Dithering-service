#ifndef DITHERING_H
#define DITHREING_H
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Dithering{
    public:
        Dithering() = default;
        Dithering(int h, int w, SDL_Surface* surf);
        ~Dithering();

        void setWidth(int w);
        void setHeight(int h);
        void setSurface(SDL_Surface* surf);

        void putPixel(SDL_Surface* surf, int x, int y, SDL_Color color);
        Uint32 getpixel(SDL_Surface* surf, int x, int y);

        SDL_Surface* grey(SDL_Surface* surf);
        void apply();
    private:
    int height;
    int width;
    SDL_Surface* surface;
};
#endif