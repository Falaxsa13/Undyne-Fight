#ifndef MAIN_H
#define MAIN_H

// System includes
#include <stdio.h>
#include <stdlib.h>

// Local includes
#include "sound.h"
#include "player.h"
#include "../images/character.h"
#include "../images/background1.h"
#include "../images/background2.h"

typedef enum
{
    START,
    PLAY,
    WIN,
    LOSE,
} gba_state;

// Function prototypes
void drawHealthBar(Player *p);
void updateHealth(Player *p, Shield *shield);

#endif
