#include <time.h>
#include "receiver.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "deqll.h"

int main(void) {
  struct dequeue *offset_deq_transmitter;
  offset_deq_transmitter = dequeue_create();
  
  struct dequeue *speed_deq_transmitter;
  speed_deq_transmitter = dequeue_create();
  
  
	srand(time(NULL));
  int x = 0;// counter for the loop
  int vehicle_speed = (rand() % 31) + 60; // vehicle speed between 60-90
  int OFFSET_VAL = rand() % 8191; // offset value between 0-8190
  int prev_sp = 0;
  //printf("===========[CC MANUAL OVERRIDE!! SPEED: %d OFFSET: 0]=======\n", vehicle_speed);
  dequeue_add_back(vehicle_speed, speed_deq_transmitter);
  dequeue_add_back(0, offset_deq_transmitter);

    while(true){
      if(x > 399) break;
      int r = rand() % 6; // offset 
      int sign = rand() % 2;// false or negative
      if(sign){
        OFFSET_VAL = rand() % 8191;
        prev_sp = vehicle_speed + r;
        //printf("[SPEED: %d, OFFSET: %d] \n", prev_sp, OFFSET_VAL);
        dequeue_add_back(prev_sp, speed_deq_transmitter);
        dequeue_add_back(OFFSET_VAL, offset_deq_transmitter);
      }else{
        OFFSET_VAL = rand() % 8191;
        prev_sp = vehicle_speed - r;
        //printf("[SPEED: %d, OFFSET: %d] \n", prev_sp, OFFSET_VAL);
        dequeue_add_back(prev_sp, speed_deq_transmitter);
        dequeue_add_back(OFFSET_VAL, offset_deq_transmitter);
      }
      int offset_time = rand() % 8; // offset 
      int sign_time = rand() % 2;// false or negative
      int manuel_speed_change_interval = 20;
      if(sign_time){
        manuel_speed_change_interval += offset_time;
      }else{
        manuel_speed_change_interval -= offset_time;
      }
      if(x % manuel_speed_change_interval == 0){
        int backup = vehicle_speed;
        vehicle_speed = (rand() % 31) + 60; // vehicle speed between 60-90 
        if(prev_sp - 5 <= vehicle_speed && vehicle_speed <= prev_sp + 5){
          vehicle_speed = backup;
        }else{
          OFFSET_VAL = rand() % 8191;
          //printf("===========[CC MANUAL OVERRIDE!! SPEED: %d OFFSET: %d]=======\n", vehicle_speed, OFFSET_VAL);
          dequeue_add_back(prev_sp, speed_deq_transmitter);
          dequeue_add_back(OFFSET_VAL, offset_deq_transmitter);
        }

      }
      x++;
    }
  printf("=============OFFSET STACK==============\n");
  dequeue_print(offset_deq_transmitter);
  printf("=============SPEED STACK==============\n");
  dequeue_print(speed_deq_transmitter);

    /////// NEW CODE STARTS HERE
    int v1;
    int v2;
    int o1;
    int o2;

    // reciever dummy
    struct target_array *ta = target_create();

    struct dequeue *offset_backup = dequeue_create();
    struct dequeue *speed_backup = dequeue_create();



    while(true){
        if(o1 == o2 && o1 == 8192){
            break;
        }
        // offset horizon yeterli oldugunda breakle

        o1 = dequeue_remove_front(offset_deq_transmitter);

        v1 = dequeue_remove_front(speed_deq_transmitter);

        if(o1 != 8192){
            dequeue_add_back(o1, offset_backup);
            dequeue_add_back(v1, speed_backup);
        }

        o2 = dequeue_remove_front(offset_deq_transmitter);

        v2 = dequeue_remove_front(speed_deq_transmitter);


        if(o2 != 8192){
            dequeue_add_back(o2, offset_backup);
            dequeue_add_back(v2, speed_backup);
        }

        // control mechanism
        struct package *p = package_create(v1,v2,o1,o2);
        add_back(p, ta);

        package_destroy(p);

    }

    // car request et geri kalani sil yeni horizona ekle son datadan itibaren

    ta_print(ta);

    dequeue_print(offset_backup);
    dequeue_print(speed_backup);

    ta_destroy(ta);
    dequeue_destroy(offset_deq_transmitter);
    dequeue_destroy(speed_deq_transmitter);
    dequeue_destroy(offset_backup);
    dequeue_destroy(speed_backup);

    return 0;
}
