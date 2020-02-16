#include "note_image.h"

void NoteImage::render() {
    SDL_Rect rect = {x - WIDTH / 2, y - HEIGHT / 2, WIDTH, HEIGHT};
    SDL_RenderCopy(renderer, arrowImage, NULL, &rect);
}