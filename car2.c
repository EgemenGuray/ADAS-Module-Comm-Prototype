
#include "car2.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// DO NOT MODIFY THIS STRUCTURE
struct car {
    int CUR_POS;
};

// DO NOT MODIFY THIS STRUCTURE

// random car structure yaratio random off set variable assign edio
struct car *car_create(){
    struct car *c = malloc(sizeof(struct car));
    srand(time(NULL));
    int number_of_offset = rand() % 100;
    int random_val = 0;
    while(true){
        if(number_of_offset == 0) break;
        int OFFSET_VAL = rand() % 8191;
        random_val += OFFSET_VAL;
        number_of_offset -= 1;
    }

    c->CUR_POS = random_val;
    return c;

}

int pos_car(struct car *c){
    return c->CUR_POS;
}

void car_destroy(struct car *c) {
    free(c);
}

