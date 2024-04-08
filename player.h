#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

void updatePlayer(Player *p, unsigned int currentButtons);
int checkCollision(Player *p, Bullet *b);
void clearPlayer(Player *p);

#endif