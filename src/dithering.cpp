#include "../include/dithering.h"


int clamp(int n, int max, int min)
{
    if (n>max)
    {
        return max;
    }
    if(n<min)
    {
        return min;
    }
    return n;
}

Dithering::Dithering(int h, int w, SDL_Surface* surf) : 
            height(h), width(w), surface(surf)
{
}

Dithering::~Dithering()
{
    delete surface;
}
void Dithering::setWidth(int w)
{
    width = w;
}
void Dithering::setHeight(int h)
{
    height = h;
}
void Dithering::setSurface(SDL_Surface* surf)
{
    surface = surf;
}

void Dithering::putPixel(SDL_Surface* surf, int x, int y, SDL_Color color)
{
    int* pixels = static_cast<int*>(surf->pixels);
    pixels[y*width + x] = SDL_MapRGB(surf->format, 
                                    color.r, color.g, color.b);
}

Uint32 Dithering::getpixel(SDL_Surface* surf, int x, int y)
{
    int bpp = surf->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

switch (bpp)
{
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {

            return p[0] << 16 | p[1] << 8 | p[2];
            break;
        }
        else
        {
            return p[0] | p[1] << 8 | p[2] << 16;
            break;
        }

    case 4:

        return *(Uint32 *)p;
        break;

    default:
        return 0;
      }
}

SDL_Surface* Dithering::grey(SDL_Surface* surf)
{
    SDL_Surface* resImage = SDL_CreateRGBSurface(0, surf->w, surf->h, 32, 0xff000000,0x00ff0000,0x0000ff00, 0x000000ff);
    SDL_BlitSurface(surf, NULL, resImage, NULL);
    SDL_Color currentColor;
     for(int y = 0; y<resImage->h-1; y++)
    {
        for(int x = 1; x<resImage->w-1; x++)
        {
            double colorValue = getpixel(resImage, x, y);
            SDL_GetRGB(colorValue, resImage->format, 
                        &currentColor.r, &currentColor.g, &currentColor.b);
            double greyscale = (currentColor.r + currentColor.g + currentColor.b)/3;
            SDL_Color newColor{
                        (Uint8)(greyscale),
                        (Uint8)(greyscale),
                        (Uint8)(greyscale)
            };
            putPixel(resImage, x, y, newColor);
        }
    }
    IMG_SaveJPG(resImage, "./images/negrini.jpg", 100);
    return resImage;
}
void Dithering::apply()
{
    int format;
    SDL_Surface* resImage = grey(surface);
    // SDL_Surface* resImage = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0xff000000,0x00ff0000,0x0000ff00, 0x000000ff);
    // SDL_BlitSurface(surface, NULL, resImage, NULL);
    SDL_Color currentColor;
    SDL_Color diffusion;
    SDL_Color newColor;
    for(int y = 0; y<height-1; y++)
    {
        for(int x = 1; x<width-1; x++)
        {
            double colorValue = getpixel(resImage, x, y);
            SDL_GetRGB(colorValue, resImage->format, 
                        &currentColor.r, &currentColor.g, &currentColor.b);
            format = 1;
            newColor = SDL_Color{
                        (Uint8)(std::round(format * currentColor.r/255.0)*(255/format)),
                        (Uint8)(std::round(format * currentColor.g/255.0)*(255/format)),
                        (Uint8)(std::round(format * currentColor.b/255.0)*(255/format))
            };
            putPixel(resImage, x, y, newColor);


            double errR = currentColor.r - newColor.r;
            double errG = currentColor.g - newColor.g;
            double errB = currentColor.b - newColor.b;


            colorValue = getpixel(resImage, x+1, y  );
            SDL_GetRGB(colorValue, resImage->format, 
                            &diffusion.r,&diffusion.g,&diffusion.b);
            diffusion.r = clamp(diffusion.r + errR * 7/16.0, 255, 0);
            diffusion.g = clamp(diffusion.g + errG * 7/16.0, 255, 0);
            diffusion.b = clamp(diffusion.b + errB * 7/16.0, 255, 0);
            putPixel(resImage, x+1, y, diffusion);

            colorValue = getpixel(resImage, x-1, y+1);
            SDL_GetRGB(colorValue, resImage->format, 
                &diffusion.r,&diffusion.g,&diffusion.b);
            diffusion.r = clamp(diffusion.r + errR * 3/16.0, 255, 0);
            diffusion.g = clamp(diffusion.g + errG * 3/16.0, 255, 0);
            diffusion.b = clamp(diffusion.b + errB * 3/16.0, 255, 0);
            putPixel(resImage, x-1, y+1, diffusion);

            colorValue = getpixel(resImage, x  , y+1);
            SDL_GetRGB(colorValue, resImage->format, 
                &diffusion.r,&diffusion.g,&diffusion.b);
            diffusion.r = clamp(diffusion.r + errR * 5/16.0, 255, 0);
            diffusion.g = clamp(diffusion.g + errG * 5/16.0, 255, 0);
            diffusion.b = clamp(diffusion.b + errB * 5/16.0, 255, 0);
            putPixel(resImage, x, y+1, diffusion);

            colorValue = getpixel(resImage, x+1, y+1);
            SDL_GetRGB(colorValue, resImage->format, 
                &diffusion.r,&diffusion.g,&diffusion.b);
            diffusion.r = clamp(diffusion.r + errR * 1/16.0, 255, 0);
            diffusion.g = clamp(diffusion.g + errG * 1/16.0, 255, 0);
            diffusion.b = clamp(diffusion.b + errB * 1/16.0, 255, 0);
            putPixel(resImage, x+1, y+1, diffusion);
            
        }
    }
    IMG_SaveJPG(resImage, "./images/negroid.jpg", 100);
}