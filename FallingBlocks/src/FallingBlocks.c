/*
 ============================================================================
 Name        : FalllingBlocks.c
 Author      : Agpv
 Version     : 0.1
 Description : SDL analog of mobile classic game "Tower Bloxx".
               space — drop the block
               on keypad — color change:
               ---------------
               4, 1 | red +/-
               5, 2 | green +/-
               6, 3 | blue +/-
               ---------------


               color is saved in the settings file
               ----------------
               128 0 128       |settings.txt example
               ----------------

               the highest score for the session is saved in a highscores.txt
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "queue.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_HEIGHT = 64;
const int BLOCK_WIDTH_CONST = 160;
int BLOCK_WIDTH = BLOCK_WIDTH_CONST;
int def = 0;

Uint8 r = 0;
Uint8 g = 0;
Uint8 b = 0;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;


void draw();
void restart();


int main(int argc, char *argv[]) {
    int quit = 0;
    char motionFlag = 'r';
    int score = 0, highScore = 0;

    //load color from file
    FILE *settings;
    settings = fopen("settings.txt", "r");
    fscanf(settings, "%i ", &r);
    fscanf(settings, "%i ", &g);
    fscanf(settings, "%i", &b);
    fclose(settings);

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL couldn't init! SDL ERROR: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("Falling Blocks!", SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL){
            printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_Rect * newElem = NULL;
            newElem = malloc(sizeof(SDL_Rect));
            if (newElem == NULL){
                return 1;
            }
            newElem->h = BLOCK_HEIGHT;
            newElem->w = BLOCK_WIDTH_CONST;
            newElem->x = SCREEN_WIDTH/2 - BLOCK_WIDTH_CONST/2;
            newElem->y = SCREEN_HEIGHT - BLOCK_HEIGHT;
            push(&tower, *newElem);
            newElem->y = SCREEN_HEIGHT - BLOCK_HEIGHT * 2;
            push(&tower, *newElem);
            newElem->y = 0;
            push(&tower, *newElem);
            SDL_Event event;
            while(quit == 0){
                while (SDL_PollEvent(&event) != 0) {
                    if (event.type == SDL_QUIT){
                        quit = 1;
                    } else if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:                    //the fall
                                    while(tower.tail->block.y+tower.tail->block.h < tower.head->next->block.y){
                                        motionFlag = 'd';
                                        tower.tail->block.y += 10;          //falling speed
                                        draw();
                                        SDL_Delay(5);
                                        SDL_UpdateWindowSurface(window);
                                        if (score%2 == 0){                  //alternating direction
                                            motionFlag = 'l';
                                        } else {
                                            motionFlag = 'r';
                                        }
                                    }
                                    if(tower.tail->block.x+BLOCK_WIDTH < tower.head->next->block.x ||
                                            tower.tail->block.x > tower.head->next->block.x + BLOCK_WIDTH){
                                        //miss
                                        SDL_FillRect(screenSurface, NULL,
                                                SDL_MapRGB(screenSurface->format, 0xFF, 0, 0));
                                        SDL_UpdateWindowSurface(window);
                                        SDL_Delay(1000);
                                        SDL_FillRect(screenSurface, NULL,
                                                SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                                        SDL_UpdateWindowSurface(window);
                                        if (score > highScore){
                                            highScore = score;
                                        }
                                        score = 0;
                                        motionFlag = 's';
                                    } else {
                                        //shortening blocks
                                        if (tower.head->next->block.x > tower.tail->block.x){
                                            def = tower.head->next->block.x - tower.tail->block.x;
                                            tower.tail->block.x += def;
                                        } else {
                                            def = tower.tail->block.x - tower.head->next->block.x;
                                        }
                                        BLOCK_WIDTH -= def;
                                        newElem->w = BLOCK_WIDTH;
                                        score++;
                                    }
                                    //push the queue
                                    if (motionFlag != 's'){
                                        push(&tower, *newElem);
                                        pop(&tower);
                                        tower.head->next->block.w = BLOCK_WIDTH;
                                        tower.head->block.y += BLOCK_HEIGHT;
                                        tower.head->next->block.y += BLOCK_HEIGHT;
                                    } else {
                                        //restart
                                        restart();
                                        draw();
                                        motionFlag = 'r';
                                    }
                                break;
                             case SDLK_ESCAPE:
                                //exit and saving colors
                                quit = 1;
                                break;
                             case SDLK_KP_1:
                                     r-= 10;
                                 break;
                             case SDLK_KP_4:
                                     r+= 10;
                                 break;
                             case SDLK_KP_2:
                                     g-= 10;
                                 break;
                             case SDLK_KP_5:
                                     g+= 10;
                                 break;
                             case SDLK_KP_3:
                                     b-= 10;
                                 break;
                             case SDLK_KP_6:
                                     b+= 10;
                                 break;
                                    }
                                }
                 }
                    if(tower.tail->block.x + tower.tail->block.w <= SCREEN_WIDTH && motionFlag =='r'){
                    //move right
                    draw();
                    tower.tail->block.x++;
                    SDL_Delay(1);
                    SDL_UpdateWindowSurface(window);
                    } else if(motionFlag != 's'){
                        //move left
                        motionFlag = 'l';
                        if (tower.tail->block.x>=0 && motionFlag == 'l'){
                            draw();
                            tower.tail->block.x--;
                            SDL_Delay(1);
                            SDL_UpdateWindowSurface(window);
                            if (tower.tail->block.x == 0) {
                            motionFlag = 'r';
                    }
                    }
                }
            }
        }
    }
    //saving color in the settings file
    settings = fopen("settings.txt", "w");
    fprintf(settings, "%i ", r);
    fprintf(settings, "%i ", g);
    fprintf(settings, "%i", b);
    fclose(settings);

    //saving a highscore in a file
    FILE *highScores;
    highScores = fopen("highscores.txt", "a+");
    fprintf(highScores, "%i \n", highScore);
    fclose(highScores);

    SDL_DestroyWindow( window );
    SDL_Quit();
	return EXIT_SUCCESS;
}

//draws 3 blocks
void draw(){
    SDL_FillRect(screenSurface, NULL,
            SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_FillRect(screenSurface, &tower.tail->block,
        SDL_MapRGB(screenSurface->format, r, g, b));
    SDL_FillRect(screenSurface, &tower.head->block,
        SDL_MapRGB(screenSurface->format, r, g, b));
    SDL_FillRect(screenSurface, &tower.head->next->block,
        SDL_MapRGB(screenSurface->format, r, g, b));
}


//changes the blocks size to the starting size
void restart(){
    BLOCK_WIDTH = BLOCK_WIDTH_CONST;
    tower.tail->block.w = tower.head->block.w = tower.head->next->block.w = BLOCK_WIDTH;
    tower.tail->block.x = SCREEN_WIDTH/2 - BLOCK_WIDTH/2;
    tower.tail->block.y = 0;
    tower.head->next->block.x = SCREEN_WIDTH/2 - BLOCK_WIDTH/2;
    tower.head->next->block.y = SCREEN_HEIGHT - BLOCK_HEIGHT * 2;
    tower.head->block.x = SCREEN_WIDTH/2 - BLOCK_WIDTH/2;
    tower.head->block.y = SCREEN_HEIGHT - BLOCK_HEIGHT;
}
