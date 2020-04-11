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
const int BLOCK_HEIGHT = 64;
int BLOCK_WIDTH = 160;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

typedef struct node_t{
    SDL_Rect block;
    struct node_t* next;
}   TNode;

typedef struct queue_t{
    TNode* head;
    TNode* tail;
}   TQueue;

TQueue tower = {NULL, NULL};

TQueue* push(TQueue* queue, SDL_Rect block){
    // Создание нового узла
    TNode* node = malloc(sizeof(TNode));
    // Присваиваем значение новому узлу
    node->block = block;
    // Т.к. элемент добавляется в хвост, то указатель
    // на следующий элемент ни на что не ссылается
    node->next = NULL;

    // Если очередь не пуста, то ...
    if (queue->head && queue->tail){
        // Хвостовой элемент теперь ссылается на
        // созданный узел
        queue->tail->next = node;
        // Созданный узел становится хвостом
        queue->tail = node;
    }
    else{
        // О-о-о... Первый элемент, теперь он и
        // вершина и хвост
        queue->head = queue->tail = node;
    }
    return queue;
}

// Извлечение элемента из очереди
SDL_Rect* pop(TQueue* queue){
    SDL_Rect block = {0, 0, 0, 0};
    TNode* node;

    // Пока есть элементы в очереди
    if (queue->head){
        // Сохраняем указатель на удаляемый элемент
        node = queue->head;
        // Сохраняем значение удаляемого элемента
        block = node->block;
        // Вершиной очереди становится следующий за удаляемым элемент
        queue->head = queue->head->next;
        // Окончательно удаляем элемент
        free(node);
    }
    return &block;
}
void clear(TQueue* queue){
    // Пока есть хотя бы один элемент
    while (queue->head){
        // Извлекаем очередной элемент
        pop(queue);
    }
    queue->head = queue->tail = NULL;
}

void draw(){
    SDL_FillRect(screenSurface, NULL,
            SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_FillRect(screenSurface, &tower.tail->block,
        SDL_MapRGB(screenSurface->format, 0, 0, 0));
    SDL_FillRect(screenSurface, &tower.head->block,
        SDL_MapRGB(screenSurface->format, 0, 0, 0));
    SDL_FillRect(screenSurface, &tower.head->next->block,
        SDL_MapRGB(screenSurface->format, 0, 0, 0));
}

int main(int argc, char *argv[]) {
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
            newElem->w = BLOCK_WIDTH;
            newElem->x = SCREEN_WIDTH/2 - BLOCK_WIDTH/2;
            newElem->y = SCREEN_HEIGHT - BLOCK_HEIGHT;
            push(&tower, *newElem);
            newElem->y = SCREEN_HEIGHT - BLOCK_HEIGHT * 2;
            push(&tower, *newElem);
            newElem->y = 0;
            push(&tower, *newElem);
            int quit = 0;
            char motionFlag = 'r';
            int score = 0;
            SDL_Event event;
            while(quit == 0){
                while (SDL_PollEvent(&event) != 0) {
                    if (event.type == SDL_QUIT){
                        quit = 1;
                    } else if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                if(motionFlag!='s'){
                                    while(tower.tail->block.y+tower.tail->block.h < tower.head->next->block.y){
                                        motionFlag = 'd';
                                        tower.tail->block.y += 10;
                                        draw();
                                        SDL_Delay(5);
                                        SDL_UpdateWindowSurface(window);
                                    }
                                    if(tower.tail->block.x+BLOCK_WIDTH < tower.head->next->block.x ||
                                            tower.tail->block.x > tower.head->next->block.x + BLOCK_WIDTH){//miss
                                        SDL_FillRect(screenSurface, NULL,
                                                SDL_MapRGB(screenSurface->format, 0xFF, 0, 0));
                                        SDL_UpdateWindowSurface(window);
                                        SDL_Delay(200);
                                        motionFlag = 's';
                                    } else {
                                        //shortening blocks
                                        /*if (tower.head->next->block.x > tower.tail->block.x){
                                            BLOCK_WIDTH -= tower.head->next->block.x - tower.tail->block.x;
                                        } else {
                                            BLOCK_WIDTH -= tower.tail->block.x - (tower.head->next->block.x + BLOCK_WIDTH);
                                        }*/
                                        score++;
                                    }
                                    push(&tower, *newElem);
                                    pop(&tower);
                                    tower.head->block.y += BLOCK_HEIGHT;
                                    tower.head->next->block.y += BLOCK_HEIGHT;
                                } else {
                                    //restart
                                }
                                break;
                             case SDLK_ESCAPE:
                                quit = 1;
                                break;
                                    }
                                }
                 }
                    if(tower.tail->block.x + tower.tail->block.w <SCREEN_WIDTH && motionFlag =='r'){
                    draw();
                    tower.tail->block.x ++;
                    SDL_Delay(1);
                    SDL_UpdateWindowSurface(window);
                    } else if(motionFlag != 's'){
                        motionFlag = 'l';
                        if (tower.tail->block.x>=0 && motionFlag == 'l'){
                            draw();
                            tower.tail->block.x --;
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
    SDL_DestroyWindow( window );
    SDL_Quit();
	return EXIT_SUCCESS;
}
