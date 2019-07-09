#include "car2.h"
#include <time.h>
#include "receiver.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// DO NOT MODIFY THIS STRUCTURE
struct package {
  int VAL_1;
  int VAL_2;
  int OFF_1;
  int OFF_2;
};

// DO NOT MODIFY THIS STRUCTURE
struct target_array {
  bool loop;
  int VAL[400];
  int OFF[400];
  int OFF_ADDED[400];
  int prev_off;
  int front_index;
  int last_index;
  int MAX_ELEMENT_INDEX;
};

struct package *package_create(int val1, int val2, int off1, int off2){
    struct package *p = malloc(sizeof(struct package));
    p->VAL_1 = val1;
    p->VAL_2 = val2;
    p->OFF_1 = off1;
    p->OFF_2 = off2;
    return p;

}

void package_destroy(struct package *p) {
    free(p);
}

struct target_array *target_create(void) {
  struct target_array *ta = malloc(sizeof(struct target_array));
  ta->front_index = 0;
  ta->prev_off = 0;
  ta->last_index = 0;//last available index
  ta->MAX_ELEMENT_INDEX = 399;
  ta->loop = false;
  return ta;
}


void add_back(struct package *p, struct target_array *ta){
 if(p->OFF_1 != 8192){
   add_back_help(p->VAL_1, p->OFF_1, ta);
 }
 if(p->OFF_2 != 8192){
   add_back_help(p->VAL_2, p->OFF_2, ta);
 }
}

void add_back_help(int value, int offset, struct target_array *ta){
    if(ta->last_index == ta->MAX_ELEMENT_INDEX && ta->front_index == 0){
        //printf("1111111\n");
        ta->loop = true;

        ta->VAL[ta->last_index] = value;

        ta->OFF[ta->last_index] = offset;
        ta->OFF_ADDED[ta->last_index] = offset + ta->prev_off;
        ta->prev_off += offset;

        struct car *c = car_create();// car pos requested
        int CUR_POS = pos_car(c);
        int new_front = find_index(CUR_POS, ta);


        printf("!!!!!!!CAR POSITION OFFSET: %d, %d ELEMENTS WILL BE DELETED, NF: %d, OF: %d!!!!!!!!!!!\n", CUR_POS, new_front-ta->front_index, new_front, ta->front_index);
        if(new_front == ta->front_index) {
            printf("======WARNING DATA OVERRIDE!!!!!========\n");
            ta->front_index = new_front;
        }else if(new_front >= ta->last_index){
            printf("======WARNING CAR TRAVELLED ALL OFFSETS DATA WILL BE DELETED!!!!!========\n");
            ta->loop = false;
            ta->front_index = 0;
        }else{
            ta->front_index = new_front;
        }
        car_destroy(c);
        ta->last_index = 0;


    }else if(ta->last_index + 1 == ta->front_index){
        //printf("222222\n");
        ta->VAL[ta->last_index] = value;
        ta->OFF[ta->last_index] = offset;
        ta->OFF_ADDED[ta->last_index] = offset + ta->prev_off;
        ta->prev_off += offset;
        ta->last_index += 1;// last item added

        struct car *c = car_create();// car pos requested
        int CUR_POS = pos_car(c);
        int new_front = find_index(CUR_POS, ta);
        printf("!!!!!!!CAR POSITION OFFSET: %d, %d ELEMENTS WILL BE DELETED, NF: %d, OF: %d!!!!!!!!!!!\n", CUR_POS, new_front-ta->front_index, new_front, ta->front_index);

        if(new_front == ta->front_index) {
            printf("======WARNING DATA OVERRIDE!!!!!========\n");
            ta->front_index = new_front;
        }else if(new_front >= ta->last_index){
            printf("======WARNING CAR TRAVELLED ALL OFFSETS DATA WILL BE DELETED!!!!!========\n");
            ta->last_index = 0;
            ta->loop = false;
            ta->front_index = ta->last_index;
        }else{
            ta->front_index = new_front;
        }
        car_destroy(c);
    }else{
        //printf("33333\n");
        ta->VAL[ta->last_index] = value;
        ta->OFF[ta->last_index] = offset;
        ta->OFF_ADDED[ta->last_index] = offset + ta->prev_off;
        ta->prev_off += offset;
        ta->last_index += 1;
    }
}


int front_offset(struct target_array *ta){
  if(ta->front_index == ta->last_index){
    return 8192;
  }else{
    return ta->OFF[ta->front_index];
  }
  
}

void remove_front_help(struct target_array *ta){
  if(ta->front_index == ta->last_index){
    ta->loop = false;
    ta->last_index = 0;
    ta->front_index = 0;
  }else{
    ta->front_index += 1;
    if(ta->front_index == ta->MAX_ELEMENT_INDEX + 1) ta->front_index = 0;
  }
}


// dequeue_destroy(q) frees all memory for q
// requires: q must be from a previous malloc
// effects: q is no longer valid
void ta_destroy(struct target_array *ta) {
  free(ta);
}

// DO NOT MODIFY THIS FUNCTION
// dequeue_print(q) prints the contents of q
// effects: prints to output
void ta_print(const struct target_array *ta) {
  int a = ta->front_index;
  int b = ta->last_index;
  if(b < a || (b == a && ta->loop)){
    for(int x = a; x<ta->MAX_ELEMENT_INDEX + 1; x++){
      printf("[OFFSET: %d, SPEED: %d, OFFSET_ADDED: %d]\n", ta->OFF[x], ta->VAL[x], ta->OFF_ADDED[x]);
    }
    for(int x = 0; x<ta->last_index; x++){
      printf("[OFFSET: %d, SPEED: %d, OFFSET_ADDED: %d]\n", ta->OFF[x], ta->VAL[x], ta->OFF_ADDED[x]);
    }
  }else{
    for(int x = a; x<ta->last_index; x++){
      printf("[OFFSET: %d, SPEED: %d, OFFSET_ADDED: %d]\n", ta->OFF[x], ta->VAL[x], ta->OFF_ADDED[x]);
    }
    
  }
}

int binarySearch(int *a[], int l, int r, int x, int possible){
    if(r >= 1){
        int mid = l+(r-1)/2;
        if(a[mid] == x) return mid + 1;
        if(a[mid] > x ) return binarySearch(a, l, mid-1, x, mid);
        return binarySearch(a, mid+1, r, x, mid + 1);
    }
    return possible;



}

int sequentialSearch(int *a[], int l, int r, int x, int possible){
    int pos = -1;
    if(l <= r){
        for(int i = l; i < r; i++){
            if(a[i] < x){
                pos = i;
            }
            printf("a[index]: %d carpos: %d index: % d curpos: %d\n", a[i], x, i, pos);
        }
    }else{
        for(int i = l; i < 399; i++){
            if(a[i] < x && a[i]!= 0){
                pos = i;
            }
            printf("a[index]: %d carpos: %d index: % d curpos: %d\n", a[i], x, i, pos);
        }
        for(int i = 0; i < r; i++){
            if(a[i] < x && a[i]!= 0){
                pos = i;
            }
            printf("a[index]: %d carpos: %d index: % d curpos: %d\n", a[i], x, i, pos);
        }

    }

    printf("a[index]: %d, a[index + 1]: %d, car offset: %d POS:%d\n", a[pos],a[pos + 1], x, pos);
    return pos;


}

int find_index(int offset, struct target_array *ta){
    return sequentialSearch(ta->OFF_ADDED, ta->front_index, ta->last_index, offset, ta->front_index);
}

