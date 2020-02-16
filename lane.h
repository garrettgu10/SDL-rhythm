#pragma once
#include "drawable.h"
#include "note.h"
#include "note_image.h"
#include "const.h"
#include "music.h"
#include <SDL2/SDL.h>
#include <deque>

class Lane : public Drawable {
    static const int SPEED = 1000; //pixels per second
    static constexpr double EXPLOSION_SPEED = 4; // 1/second
    static const int BOTTOM_PADDING = 50; //target's offset from bottom of screen
public:
    int x = 0;
    Music *toTrack = NULL;
    ArrowImage arrowImage = {NULL, 0};
    std::deque<Note *> futureNotes; //sorted
    std::deque<NoteImage *> viewableNotes; //sorted
    std::deque<NoteImage *> explodingNotes;

    Lane(SDL_Renderer *r, Music *music, ArrowImage arrowImage, int x) : 
        Drawable(r), toTrack(music), arrowImage(arrowImage), x(x) { };

    virtual void render();

    static int calculateNoteY(double noteTime, double now);

    void updateViewable();

    void hit();
};