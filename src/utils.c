// Standard headers
#include <stdlib.h>
#include <stdio.h>

// Main header
#include "utils.h"
#include "direction.h"

// Macros
#define POSITION_UNKNOWN '?'
#define POSITION_BARRIER '#'
#define POSITION_EMPTY ' '

/*----------------------------------------------------------------------------*/
/*                          PRIVATE FUNCTIONS HEADERS                         */
/*----------------------------------------------------------------------------*/

char *get_position_cell(char **known_matrix, position_t pos);

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

#define MAX_MATRIX 10 // TODO: INCREASE THIS

char **initialize_known_matrix()
{
    char **known_matrix = (char **)malloc(MAX_MATRIX * sizeof(char *));
    for (int i = 0; i < MAX_MATRIX; i++)
    {
        known_matrix[i] = (char *)malloc(MAX_MATRIX * sizeof(char));
        for (int j = 0; j < MAX_MATRIX; j++)
            known_matrix[i][j] = POSITION_UNKNOWN;
    }
    return known_matrix;
}

/*----------------------------------------------------------------------------*/

void dispose_known_matrix(char **known_matrix)
{
    for (int i = 0; i < MAX_MATRIX; i++)
        free(known_matrix[i]);
    free(known_matrix);
}

/*----------------------------------------------------------------------------*/

void update_known_matrix(char **known_matrix, direction_t last_direction, position_t last_position, position_t cur_position)
{
    if (!(last_direction.i == 0 && last_direction.j == 0) && equal_positions(last_position, cur_position))
        *get_position_cell(known_matrix, move_position(last_position, last_direction)) = POSITION_BARRIER;
    else
    {
        if (!equal_positions(last_position, (position_t)INVALID_POSITION))
            *get_position_cell(known_matrix, last_position) = POSITION_EMPTY;
        *get_position_cell(known_matrix, cur_position) = POSITION_EMPTY;
    }
}

/*----------------------------------------------------------------------------*/

int can_walk_to(char **known_matrix, position_t pos)
{
    if (pos.i >= MAX_MATRIX || pos.j >= MAX_MATRIX)
        return 0;

    // TODO: REMOVE
    printf("can walk to (%d,%d): %d (%c)\n", (int)pos.i, (int)pos.j, *get_position_cell(known_matrix, pos) == POSITION_UNKNOWN, *get_position_cell(known_matrix, pos));

    return *get_position_cell(known_matrix, pos) == POSITION_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                             PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

char *get_position_cell(char **known_matrix, position_t pos)
{
    return &known_matrix[pos.i][pos.j];
}

void print_known_matrix(char **known_matrix)
{
    for (size_t i = 0; i < MAX_MATRIX; i++)
    {
        for (size_t j = 0; j < MAX_MATRIX; j++)
        {
            putchar('|');
            putchar(known_matrix[i][j]);
        }
        putchar('|');
        putchar('\n');
    }
}

/*----------------------------------------------------------------------------*/
