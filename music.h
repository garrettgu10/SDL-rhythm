#pragma once
#include <SDL2/SDL_mixer.h>
#include "timer.h"

class Music {
public:
    Mix_Music *music;
    int samples = 0;
    int frequency = 163840;
    Timer timer;

    Music(const char *path);

    void play() {
        Mix_PlayMusic(music, -1);
        timer.start();
    }

    bool isPlaying() {
        return !Mix_PausedMusic();
    }

    int getSamples() {
        return samples;
    }

    double getSeconds() {
        return timer.getTime();
    }

    ~Music() {
        Mix_FreeMusic(music);
    }

};