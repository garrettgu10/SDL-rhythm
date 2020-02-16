#include "lane.h"
#include "const.h"
#include <SDL2/SDL.h>

void Lane::render() {
    for(NoteImage *note : viewableNotes) {
        note->render();
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT - BOTTOM_PADDING, 
        SCREEN_WIDTH, SCREEN_HEIGHT - BOTTOM_PADDING);
}

int Lane::calculateNoteY(double noteTime, double now) {
    double timeFromImpact = noteTime - now;
    
    double hitLineY = SCREEN_HEIGHT - BOTTOM_PADDING;
    
    return hitLineY - timeFromImpact * SPEED;
}

constexpr int RED = 0xff0000;

//promotes non-viewable notes, moves viewable notes, and removes notes which have moved off screen
void Lane::updateViewable() {
    double now = toTrack-> getSeconds();
    
    //iterate thru futureNotes until one is not visible
    while(!futureNotes.empty()) {
        Note *nextFuture = futureNotes.front();
        futureNotes.pop_front();
        int noteY = calculateNoteY(nextFuture->time, now);
        if(noteY > -NoteImage::HEIGHT) {
            NoteImage *newNoteImg = new NoteImage(renderer, x, noteY, RED, nextFuture);
            viewableNotes.push_back(newNoteImg);
        }else{
            //i'm not visible, add me back and stop looking
            futureNotes.push_front(nextFuture);
            break;
        }
    }

    //iterate thru viewable notes, updating y
    int numViewable = viewableNotes.size();
    for(int i = 0; i < numViewable; i++) {
        NoteImage *nextViewable = viewableNotes.front();
        viewableNotes.pop_front();
        
        nextViewable->y = calculateNoteY(nextViewable->note->time, now);
        
        if(nextViewable->y > SCREEN_HEIGHT + NoteImage::HEIGHT) {
            //don't add me back
        }else{
            //add me back
            viewableNotes.push_back(nextViewable);
        }
    }
}

void Lane::hit() {
    if(viewableNotes.size() == 0) return;
    NoteImage *nextNote = viewableNotes.front();
    double now = toTrack->getSeconds();
    double error = fabs(nextNote->note->time - toTrack->getSeconds());

    if(error < 0.1) {
        delete nextNote;
        viewableNotes.pop_front();
        printf("woohoo!\n");
    }else if(error < 0.3){
        printf("it's ok i guess\n");
    }
}