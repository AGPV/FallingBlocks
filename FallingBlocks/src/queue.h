#ifndef QUEUE_
#define QUEUE_

//working with a queue of SDL_Rect

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
    TNode* node = malloc(sizeof(TNode));    //create new node
    node->block = block;                    //assign a values
    node->next = NULL;
    if (queue->head && queue->tail){        //if queue doesn't empty
        queue->tail->next = node;
        queue->tail = node;                 //created node becomes tail
    }
    else{                                   //first node
        queue->head = queue->tail = node;
    }
    return queue;
}

SDL_Rect* pop(TQueue* queue){
    SDL_Rect block = {0, 0, 0, 0};          //x,y,w,h
    TNode* node;
    if (queue->head){
        node = queue->head;
        block = node->block;
        queue->head = queue->head->next;
        free(node);
    }
    return &block;              //idk how to improve warning
}

#endif /* QUEUE_ */
