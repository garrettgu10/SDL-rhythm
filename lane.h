#include "drawable.h"
#include "note.h"
#include "note_image.h"
#include "const.h"
#include "music.h"
#include <SDL2/SDL.h>
#include <deque>

class Lane : public Drawable {
    static const int SPEED = 100; //pixels per second
    static const int BOTTOM_PADDING = 100; //target's offset from bottom of screen
public:
    int x = 0;
    Music *toTrack = NULL;
    std::deque<Note *> futureNotes; //sorted
    std::deque<NoteImage *> viewableNotes; //sorted

    Lane(SDL_Renderer *r, Music *music, int x) : Drawable(r), toTrack(music), x(x) {
        for(int i = 0; i < 100; i++){
            futureNotes.push_back(new Note(i * 0.10));
        }
    };

    virtual void render();

    static int calculateNoteY(double noteTime, double now);

    void updateViewable();
};