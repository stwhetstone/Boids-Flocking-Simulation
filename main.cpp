#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

int *gFrameBuffer;
SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Texture *gTexture;
static int gDone;

const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;


void init() {
    // make whole screen black
    for(int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
        gFrameBuffer[i] = 0xff000000;
    }

    for(int i = 0; i < WINDOW_WIDTH; i++) {
        // height 
        int p = (int)((sin((i + 3247) * 0.02) * 0.3 +
            sin((i + 2347) * 0.04) * 0.1 +
            sin((i + 4378) * 0.01) * 0.6) * 100 + (WINDOW_HEIGHT * 2 / 3));
        // x pixel relate to height
        int pos = p * WINDOW_WIDTH + i;
        // make every pixel in pos column g from p to height
        for(int j = p; j < WINDOW_HEIGHT; j++) {
            gFrameBuffer[pos] = 0xff007f00;
            pos += WINDOW_WIDTH;
        }
    }
}

bool update() {
    SDL_Event e;

    // get input
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }

        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) {
            return false;
        }
    }

    char *pix;
    // length of 1 row of pixels
    int pitch;

    // make texture write only
    // pix points to array of locked pixels from texture
    SDL_LockTexture(gTexture, NULL, (void **)&pix, &pitch);
    // copy frame buffer to pix 1 line of pixels at a time
    for (int i = 0, pixOffset = 0, bufOffset = 0; i < WINDOW_HEIGHT; i++, bufOffset += WINDOW_WIDTH, pixOffset += pitch) {
        // frame buffer because the entire display might not be updated every frame
        memcpy(pix + pixOffset, gFrameBuffer + bufOffset, WINDOW_WIDTH * 4);
    }

    // copy data to video memory
    SDL_UnlockTexture(gTexture);
    // copy texture to renderer
    SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);
    // update screen with renderer
    SDL_RenderPresent(gRenderer);

    return true;
}

// set 8 random pixels at top of the screen to white
void newSnow() {
    for(int i = 0; i < 8; i++) {
        gFrameBuffer[rand() % (WINDOW_WIDTH - 2) + 1] = 0xffffffff;
    }
}

void snowfall() {
    for(int j = WINDOW_HEIGHT - 2; j >= 0; j--) {
        int ypos = j * WINDOW_WIDTH;
        for(int i = 1; i < WINDOW_WIDTH - 1; i++) {
            // if current pixel is white
            if(gFrameBuffer[ypos + i] == 0xffffffff) {
                // if pixel below is black move white pixel down
                if(gFrameBuffer[ypos + i + WINDOW_WIDTH] == 0xff000000) {
                    gFrameBuffer[ypos + i + WINDOW_WIDTH] = 0xffffffff;
                    gFrameBuffer[ypos + i] = 0xff000000;
                // if pixel to the left is black move white pixel left
                } else if(gFrameBuffer[ypos + i + WINDOW_WIDTH - 1] == 0xff000000) { 
                    gFrameBuffer[ypos + i + WINDOW_WIDTH - 1] = 0xffffffff;
                    gFrameBuffer[ypos + i] = 0xff000000;
                // if pixel to the right is black move white pixel right
                } else if(gFrameBuffer[ypos + i + WINDOW_WIDTH + 1] == 0xff000000) {
                    gFrameBuffer[ypos + i + WINDOW_WIDTH + 1] = 0xffffffff;
                    gFrameBuffer[ypos + i] = 0xff000000;
                }
            }
        }
    }
}

void render() {
    newSnow();
    snowfall();
}

void loop() {
    if(!update()) {
        gDone = 1;
    } else {
        render();
    }
}

int main() {
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        return -1;
    }

    gFrameBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
    gWindow = SDL_CreateWindow("CH4", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, NULL);
    gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if(!gFrameBuffer || !gWindow || !gRenderer || !gTexture) {
        return -1;
    }

    init();
    gDone = 0;
    while(!gDone) {
        loop();
    }

    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    delete[] gFrameBuffer;

    return 0;
}
