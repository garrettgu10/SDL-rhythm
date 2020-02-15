#include "music.h"

void mix_handler(void *udata, Uint8 *, int len) {
    Music *music = (Music *)udata;

    music->samples += len;

    double timerTime = music->timer.getTime();
    double musicTime = (double)music->samples / music->frequency;

    printf("%lf\t%lf\n", timerTime, musicTime);
    printf("%lf\n", (timerTime + musicTime) / 2);
    
    music->timer.sync((timerTime + musicTime) / 2);
}

Music::Music(const char *path)  {
    music = Mix_LoadMUS(path);
    if(!music) {
        printf("loading music: %s\n", Mix_GetError());
    }

    Mix_SetPostMix(mix_handler, this);
}