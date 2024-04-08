// gameStateHandlers.h

#ifndef GAME_STATE_HANDLERS_H
#define GAME_STATE_HANDLERS_H

#include "main.h"

void handleStartState(Player *player, Shield *shield, int *bulletCounter, int *switcher, int *switcher2, int *switcher3, int *switcher4, gba_state *state, u32 *currentButtons);
void handlePlayState(Player *player, Shield *shield, int *bulletCounter, gba_state *state, u32 *currentButtons, int *switcher, ShieldDirection *oldShieldDirection);
void handleWinState(gba_state *state, u32 *currentButtons, int *switcher4);
void handleLoseState(gba_state *state, u32 *currentButtons, int *switcher3);

#endif