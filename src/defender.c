// Standard headers
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "direction.h"
#include "spy.h"
#include "utils.h"

// Main header
#include "defender.h"

// Macros
#define SPY_ATTACKER_AT 5
#define CENTER_MODE 0
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

static direction_t choose_direction(position_t defender_position, char **known_matrix, int mode);
static direction_vector_t create_direction_vector(int mode);
static void dispose_direction_vector(direction_vector_t v);

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy)
{
    // Static variables declaration
    static int amount_rounds = 0;
    static position_t initial_position = INVALID_POSITION;
    static direction_t last_direction = DIR_STAY;
    static position_t last_position = INVALID_POSITION;
    static char **known_matrix = NULL;
    static int mode = CENTER_MODE;

    // Update static variables
    amount_rounds++;
    if (known_matrix == NULL)
        known_matrix = initialize_known_matrix();
    update_known_matrix(known_matrix, last_direction, last_position, defender_position);
    if (equal_positions(initial_position, (position_t)INVALID_POSITION))
        initial_position = defender_position;

    // Spy
    if (amount_rounds == SPY_ATTACKER_AT)
    {
        position_t attacker_position = get_spy_position(attacker_spy);
        unsigned long difference = abs((int)attacker_position.i - (int)defender_position.i);
        if (difference > 2)
            mode = (attacker_position.i > defender_position.i) ? BOTTOM_MODE : TOP_MODE;
    }

    // Walk
    direction_t dir = choose_direction(defender_position, known_matrix, mode);

    last_direction = dir;
    last_position = defender_position;
    return dir;
}

static direction_t choose_direction(position_t defender_position, char **known_matrix, int mode)
{
    direction_vector_t direction_vector = create_direction_vector(mode);
    for (int i = 0; i < direction_vector.length; i++)
    {
        direction_t cur_dir = direction_vector.array[i];
        if (can_walk_to(known_matrix, move_position(defender_position, cur_dir)))
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
    case CENTER_MODE:
        return (direction_vector_t){NULL, 0};
    case TOP_MODE:
    {
        direction_t *directions = (direction_t *)malloc(1 * sizeof(direction_t));
        directions[0] = (direction_t)DIR_UP;
        return (direction_vector_t){directions, 1};
    }
    default:
    case BOTTOM_MODE:
    {
        direction_t *directions = (direction_t *)malloc(1 * sizeof(direction_t));
        directions[0] = (direction_t)DIR_DOWN;
        return (direction_vector_t){directions, 1};
    }
    }
}

static void dispose_direction_vector(direction_vector_t v)
{
    UNUSED(v);
}

/*----------------------------------------------------------------------------*/
