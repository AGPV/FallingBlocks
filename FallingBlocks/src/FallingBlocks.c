/*
 ============================================================================
 Name        : SDLFirstSteps.c
 Author      : Agpv
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_WIDTH = 160;
const int BLOCK_HEIGHT = 64;

typedef struct node {
    SDL_Rect block;
    struct node * next;
} tower;



int main(int argc, char *argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL couldn't init! SDL ERROR: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("Hello SDL!", SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL){
            printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL,
                    SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));





            tower * head = NULL;
            head = malloc(sizeof(tower));
            if (head == NULL){
                return 1;
            }
            head->block.h = BLOCK_HEIGHT;
            head->block.w = BLOCK_WIDTH;
            head->block.x = SCREEN_WIDTH/2 - BLOCK_WIDTH/2;
            head->block.y = 0;
            head->next = NULL;

            int quit = 0;
            char motionFlag = 'r';
            SDL_Event event;
            while(quit == 0){
                while (SDL_PollEvent(&event) != 0) {
                                if (event.type == SDL_QUIT){
                                    quit = 1;
                                }
                                else if (event.type == SDL_KEYDOWN) {
                                    switch (event.key.keysym.sym) {
                                    case SDLK_SPACE:
                                        while(head->block.y+head->block.h < SCREEN_HEIGHT){
                                            motionFlag = 'd';
                                            head->block.y += 10;
                                            SDL_FillRect(screenSurface, NULL,
                                                    SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                                            SDL_FillRect(screenSurface, &head->block,
                                                                        SDL_MapRGB(screenSurface->format, 0, 0, 0));
                                            SDL_Delay(5);
                                            SDL_UpdateWindowSurface(window);
                                        }
                                        motionFlag = 's';
                                        break;
                                    case SDLK_ESCAPE:
                                        quit = 1;
                                        break;
                                    }
                                }
                            }
                    if(head->block.x + head->block.w <SCREEN_WIDTH && motionFlag =='r'){
                    SDL_FillRect(screenSurface, NULL,
                            SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                    SDL_FillRect(screenSurface, &head->block,
                            SDL_MapRGB(screenSurface->format, 0, 0, 0));
                    head->block.x ++;
                    SDL_Delay(1);
                    SDL_UpdateWindowSurface(window);
                    } else if(motionFlag!='s'){
                        motionFlag = 'l';
                        if (head->block.x>=0 && motionFlag == 'l'){
                            SDL_FillRect(screenSurface, NULL,
                                    SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                            SDL_FillRect(screenSurface, &head->block,
                                    SDL_MapRGB(screenSurface->format, 0, 0, 0));
                            head->block.x --;
                            SDL_Delay(1);
                            SDL_UpdateWindowSurface(window);
                            if (head->block.x == 0) {
                            motionFlag = 'r';
                    }
                    }
                }
            }
        }
    }
    SDL_DestroyWindow( window );
    SDL_Quit();
	return EXIT_SUCCESS;
}
