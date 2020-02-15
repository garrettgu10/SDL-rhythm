#include "note_image.h"

void NoteImage::render() {
    SDL_Rect rect = {x - WIDTH / 2, y - HEIGHT / 2, WIDTH, HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}