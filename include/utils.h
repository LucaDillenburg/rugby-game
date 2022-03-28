#ifndef UTILS_H
#define UTILS_H

// Internal headers
#include "position.h"
#include "direction.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

// Known Matrix Functions
char **initialize_known_matrix();
void dispose_known_matrix(char **known_matrix);
void print_known_matrix(char **known_matrix);
void update_known_matrix(char **known_matrix, direction_t last_direction, position_t last_position, position_t cur_position);
int can_walk_to(char **known_matrix, position_t pos);

#endif // UTILS_H
