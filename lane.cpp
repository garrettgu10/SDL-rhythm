#include "lane.h"
#include "const.h"
#include "score.h"
#include <SDL2/SDL.h>

void Lane::render() {
    for(NoteImage *note : explodingNotes) {
        note->render();
    }
    for(NoteImage *note : viewableNotes) {
        note->render();
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
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
            NoteImage *newNoteImg = new NoteImage(renderer, x, noteY, RED, nextFuture, arrowImage);
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
        
        if(nextViewable->y > SCREEN_HEIGHT + NoteImage::HEIGHT / 2) {
            //we missed it
            score->previousHit = BAD;
            score->counts[BAD]++;
            score->pointVal -= 100;
            score->combo = 0;
            delete nextViewable;
        }else{
            //add me back
            viewableNotes.push_back(nextViewable);
        }
    }

    int numExploding = explodingNotes.size();
    for(int i = 0; i < numExploding; i++){
        NoteImage *nextExploding = explodingNotes.front();
        explodingNotes.pop_front();

        nextExploding->explosionFactor = (now - nextExploding->explosionStart) * EXPLOSION_SPEED;
        if(nextExploding->explosionFactor < 1){
            //add me back
            explodingNotes.push_back(nextExploding);
        }
    }

    if(futureNotes.size() + viewableNotes.size() + explodingNotes.size() == 0){
        finished = true;
    }
}

const int POINT_VAL[4] = {300, 200, 50, -100};

inline void Lane::handleVerdict(int verdict) {
    if(verdict < 0 || verdict >= 4) printf("error 11\n");
    score->previousHit = verdict;
    score->counts[verdict]++;
    score->pointVal += POINT_VAL[verdict];
    if(verdict == GREAT || verdict == BAD){
        score->combo = 0;
    }else{
        score->combo++;
        if(score->combo > score->maxCombo) {
            score->maxCombo = score->combo;
        }
    }
}

void Lane::hit() {
    if(viewableNotes.size() == 0) {
        if(!CHEAT){
            handleVerdict(BAD);
        }
        return;
    }
    NoteImage *nextNote = viewableNotes.front();
    double now = toTrack->getSeconds();
    double error = fabs(nextNote->note->time - toTrack->getSeconds());

    if(error < (CHEAT? 0.05 : 0.15)){
        viewableNotes.pop_front();
        nextNote->explosionStart = now;
        nextNote->y = SCREEN_HEIGHT - BOTTOM_PADDING;
        explodingNotes.push_back(nextNote);
        if(error < 0.03 || CHEAT){
            handleVerdict(PERFECT);
        }else if(error < 0.07){
            handleVerdict(AMAZING);
        }else{
            handleVerdict(GREAT);
        }
    }else if(!CHEAT) {
        handleVerdict(BAD);
    }
}