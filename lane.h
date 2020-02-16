#pragma once
#include "drawable.h"
#include "note.h"
#include "note_image.h"
#include "const.h"
#include "music.h"
#include <SDL2/SDL.h>
#include <deque>

class Lane : public Drawable {
    static const int SPEED = 350; //pixels per second
    static const int BOTTOM_PADDING = 100; //target's offset from bottom of screen
public:
    int x = 0;
    Music *toTrack = NULL;
    SDL_Texture *arrowImage = NULL;
    std::deque<Note *> futureNotes; //sorted
    std::deque<NoteImage *> viewableNotes; //sorted

    Lane(SDL_Renderer *r, Music *music, SDL_Texture *arrowImage, int x) : 
        Drawable(r), toTrack(music), arrowImage(arrowImage), x(x) { };

    virtual void render();

    static int calculateNoteY(double noteTime, double now);

    void updateViewable();

    void hit();
};