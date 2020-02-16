#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include "drawable.h"
#include "note.h"

class NoteImage : public Drawable{
public:
    static const int WIDTH = 100;
    static const int HEIGHT = 100;

    SDL_Texture *arrowImage;
    Note *note;
    int x;
    int y;
    NoteImage(SDL_Renderer *r, int x, int y, int rgb, Note *note, SDL_Texture *arrowImage) : 
        Drawable(r), x(x), y(y), note(note), arrowImage(arrowImage) {
        
    };

    virtual void render();
};