#include "../../include/gameStateHandlers.h"

/**
 * The function `handleWinState` displays a congratulatory message and prompts the user to press Start
 * to retry when a certain condition is met.
 *
 * @param state The `state` parameter is a pointer to a `gba_state` struct, which likely contains
 * information about the current state of the Game Boy Advance system or game.
 * @param currentButtons The `currentButtons` parameter is a pointer to an unsigned 32-bit integer. It
 * likely holds the current state of the buttons being pressed on the Game Boy Advance.
 * @param switcher4 The `switcher4` parameter is a pointer to an integer that is used as a flag to
 * determine if the win state has been handled already. It is initially set to 0 and is later updated
 * to 1 once the win state is displayed to prevent it from being displayed multiple times.
 */
void handleWinState(gba_state *state, u32 *currentButtons, int *switcher4)
{
    if (*switcher4 == 0)
    {
        drawFullScreenImageDMA(background1);
        drawCenteredString(70, 72, 100, 10, "Congratulations!", WHITE);
        drawCenteredString(90, 72, 100, 10, "Press Start to Retry", WHITE);
        *switcher4 = 1;
    }
    if (KEY_DOWN(BUTTON_START, *currentButtons))
    {
        *state = START;
        fillScreenDMA(BLACK);
    }
}