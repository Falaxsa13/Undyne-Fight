#include "../../include/gameStateHandlers.h"

/**
 * The function `handleLoseState` displays a game over screen and allows the player to retry by
 * pressing the Start button.
 *
 * @param state The `state` parameter is a pointer to a `gba_state` struct, which likely contains
 * information about the current state of the Game Boy Advance system or game.
 * @param currentButtons The `currentButtons` parameter is a pointer to an unsigned 32-bit integer
 * (`u32`) that likely represents the current state of the buttons being pressed on a Game Boy Advance
 * device. This parameter is used in the `handleLoseState` function to check if the Start button is
 * pressed
 * @param switcher3 The `switcher3` parameter is a pointer to an integer that is used as a flag to
 * determine if the lose state screen has already been displayed. It is initially set to 0 and then
 * updated to 1 once the game over screen is shown to prevent it from being displayed multiple times.
 */
void handleLoseState(gba_state *state, u32 *currentButtons, int *switcher3)
{
    if (*switcher3 == 0)
    {
        drawFullScreenImageDMA(background1);
        drawCenteredString(70, 72, 100, 10, "Game Over!", WHITE);
        drawCenteredString(90, 72, 100, 10, "Press Start to Retry", WHITE);
        *switcher3 = 1;
    }
    if (KEY_DOWN(BUTTON_START, *currentButtons))
    {
        *state = START;
        fillScreenDMA(BLACK);
    }
}