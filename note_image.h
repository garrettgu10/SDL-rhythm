#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include "drawable.h"
#include "note.h"

class NoteImage : public Drawable{
public:
    static const int WIDTH = 100;
    static const int HEIGHT = 5;

    Note *note;
    int x;
    int y;
    NoteImage(SDL_Renderer *r, int x, int y, int rgb, Note *note) : Drawable(r), x(x), y(y), note(note) {
        
    };

    virtual void render();
};