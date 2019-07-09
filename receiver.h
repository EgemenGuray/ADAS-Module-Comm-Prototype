#include <stdbool.h>

// DO NOT MODIFY THIS STRUCTURE
struct package;

// DO NOT MODIFY THIS STRUCTURE
struct target_array;


struct target_array *target_create(void);

struct package *package_create(int val1, int val2, int off1, int off2);


void package_destroy(struct package *p);


void add_back(struct package *p, struct target_array *ta);

void add_back_help(int value, int offset, struct target_array *ta);

// returns the front offset value
int front_offset(struct target_array *ta);

// removes from front
void remove_front_help(struct target_array *ta);

// destroys the structure
void ta_destroy(struct target_array *ta);


// prints the structure
void ta_print(const struct target_array *ta);

//finds the index
int find_index(int offset, struct target_array *ta);
