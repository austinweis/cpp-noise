#include <iostream>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "FastNoiseLite.h"

using namespace std;

#define WIDTH 720
#define HEIGHT 720

bool running, fullscreen;

SDL_Renderer* renderer;
SDL_Window* window;

FastNoiseLite fastNoise;

void draw();
void update();
void input();
int ran = rand() % 999999;

double noise(float x, float y){
    return (fastNoise.GetNoise(x+ran, y+ran)+1)/2 ;
}

int main(int argc, char * argv[])
{
    fastNoise.SetSeed(time(NULL)); // set noise seed
    fastNoise.SetFractalType(fastNoise.FractalType_FBm);
    fastNoise.SetFractalOctaves(8);
    //printGrid(100, 50);

    // sdl window
    running=true;
    fullscreen=false;
    static int lastTime=0;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Failed to initialize sdl2" << endl;
    if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) cout << "Failed to create window" << endl;
    SDL_SetWindowTitle(window, "SDL2 Window");

    draw();
    while(running){
        update();
        input(); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void update(){
    if(fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    if(!fullscreen) SDL_SetWindowFullscreen(window, 0);
}
void input(){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type==SDL_QUIT) running=false;
        else if(e.type==SDL_KEYDOWN){
            ran = rand() % 999999;
            draw();
        }      
    }
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    if(keystates[SDL_SCANCODE_ESCAPE]) running=false;
    if(keystates[SDL_SCANCODE_F11]) fullscreen=!fullscreen;
}
void draw(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    fastNoise.SetNoiseType(fastNoise.NoiseType_Perlin);
    for (int y = 0; y < HEIGHT/2; y++){
        for (int x = 0; x < WIDTH; x++){
            double val = noise(x, y);

            int color = 255 - (255 * val);
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    fastNoise.SetNoiseType(fastNoise.NoiseType_OpenSimplex2S);
    for (int y = HEIGHT/2; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            double val = noise(x, y);

            int color = 255 - (255 * val);
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);
}