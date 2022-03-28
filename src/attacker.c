// Standard headers
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"
#include "utils.h"

// Main header
#include "attacker.h"

// Macros
#define SPY_DEFENDER_AT 5
#define TOP_MODE 1
#define BOTTOM_MODE 2

/*----------------------------------------------------------------------------*/
/*                                PRIVATE STRUCTS                             */
/*----------------------------------------------------------------------------*/

struct direction_vector
{
    direction_t *array;
    int length;
};
typedef struct direction_vector direction_vector_t;

/*----------------------------------------------------------------------------*/
/*                          PRIVATE FUNCTIONS HEADERS                         */
/*----------------------------------------------------------------------------*/

static direction_t choose_direction(position_t attacker_position, char **known_matrix, int mode);
static direction_vector_t create_direction_vector(int mode);
static void dispose_direction_vector(direction_vector_t v);

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy)
{
    // Static variables declaration
    static int amount_rounds = 0;
    static direction_t last_direction = DIR_STAY;
    static position_t last_position = INVALID_POSITION;
    static char **known_matrix = NULL;
    static int mode = BOTTOM_MODE;

    // Update static variables
    amount_rounds++;
    if (known_matrix == NULL)
        known_matrix = initialize_known_matrix();
    update_known_matrix(known_matrix, last_direction, last_position, attacker_position);

    // TODO: REMOVE
    print_known_matrix(known_matrix);

    // Spy
    if (amount_rounds == SPY_DEFENDER_AT)
    {
        position_t defender_position = get_spy_position(defender_spy);
        if (defender_position.i > attacker_position.i)
            mode = TOP_MODE;
    }

    // Walk
    direction_t dir = choose_direction(attacker_position, known_matrix, mode);

    last_direction = dir;
    last_position = attacker_position;
    return dir;
}

static direction_t choose_direction(position_t attacker_position, char **known_matrix, int mode)
{
    direction_vector_t direction_vector = create_direction_vector(mode);
    for (int i = 0; i < direction_vector.length; i++)
    {
        direction_t cur_dir = direction_vector.array[i];
        int can_walk = can_walk_to(known_matrix, move_position(attacker_position, cur_dir));
        if (can_walk)
        {
            dispose_direction_vector(direction_vector);
            return cur_dir;
        }
    }

    dispose_direction_vector(direction_vector);
    return (direction_t)DIR_STAY;
}

/*----------------------------------------------------------------------------*/
/*                             PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

static direction_vector_t create_direction_vector(int mode)
{
    switch (mode)
    {
    case TOP_MODE:
    {
        direction_t *directions = (direction_t *)malloc(3 * sizeof(direction_t));
        directions[0] = (direction_t)DIR_UP;
        directions[1] = (direction_t)DIR_RIGHT;
        directions[2] = (direction_t)DIR_DOWN;
        return (direction_vector_t){directions, 3};
    }
    default:
    case BOTTOM_MODE:
    {
        direction_t *directions = (direction_t *)malloc(3 * sizeof(direction_t));
        directions[0] = (direction_t)DIR_DOWN;
        directions[1] = (direction_t)DIR_RIGHT;
        directions[2] = (direction_t)DIR_UP;
        return (direction_vector_t){directions, 3};
    }
    }
}

static void dispose_direction_vector(direction_vector_t v)
{
    // TODO: UNUSED(v);
    free(v.array);
}

/*----------------------------------------------------------------------------*/
