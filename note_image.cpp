#include "note_image.h"

void NoteImage::render() {
    SDL_Rect rect = {x - WIDTH / 2, y - HEIGHT / 2, WIDTH, HEIGHT};
    
    int rgb = arrowImage.rgb;
    SDL_SetTextureColorMod(arrowImage.image, (rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff);

    SDL_RenderCopyEx(renderer, arrowImage.image, NULL, &rect, arrowImage.rotation, NULL, SDL_FLIP_NONE);
}