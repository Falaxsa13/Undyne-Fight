#include "../../include/gameStateHandlers.h"

/**
 * @brief Handles the play state of the game.
 *
 * Manages the gameplay mechanics during the play state, including updating player
 * and shield movements, bullet interactions, health updates, and game state transitions.
 *
 * @param player Pointer to the player object.
 * @param shield Pointer to the shield object.
 * @param bulletCounter Pointer to the bullet counter.
 * @param state Pointer to the current game state.
 * @param currentButtons Pointer to the current button input state.
 * @param switcher Pointer to the switcher variable controlling game initialization.
 * @param oldShieldDirection Pointer to the previous shield direction.
 */
void handlePlayState(Player *player, Shield *shield, int *bulletCounter, gba_state *state, u32 *currentButtons, int *switcher, ShieldDirection *oldShieldDirection)
{
    if (*switcher == 0)
    {
        drawFullScreenImageDMA(background2);
        drawBox();
        drawUI();
        drawButtons();
        drawHealthBarBorder();
        initBullets();
        *switcher = 1;
    }

    drawEnemy();

    if (shield->active)
    {
        updateShield(shield, *currentButtons);
    }
    else
    {
        updatePlayer(player, *currentButtons);
    }

    updateBullets(shield);
    updateHealth(player, shield);
    drawBox();

    if (shield->active)
    {
        *oldShieldDirection = drawShield(shield, player, *oldShieldDirection);
    }

    (*bulletCounter)++;

    clearPlayer(player);
    drawHealthBar(player);
    drawBullets();
    drawImageDMA(player->row, player->col, player->width, player->height, character);

    if (*bulletCounter >= 1000)
    {
        shield->active = 0;
        *state = WIN;
        fillScreenDMA(BLACK);
    }

    if (rand() % 50 == 0)
    {
        Direction dir = rand() % 4;
        addBullet(dir, rand() % 2 + 1);
    }

    if (player->health <= 0)
    {
        *state = LOSE;
    }
}