// startScene.c
#include "../../include/gameStateHandlers.h"

/**
 * The function `handleStartState` initializes game state variables and displays instructions for the
 * player to start the game.
 *
 * @param player The `player` parameter is a pointer to a `Player` struct, which likely contains
 * information about the player character in a game, such as health, position, and other relevant data.
 * In the `handleStartState` function, the health of the player is set to 100.
 * @param shield The `shield` parameter in the `handleStartState` function is a pointer to a `Shield`
 * struct. This struct likely contains information about a shield in a game, such as its position,
 * size, and whether it is currently active or not. In this function, the `active` field
 * @param bulletCounter The `bulletCounter` parameter is a pointer to an integer that keeps track of
 * the number of bullets fired by the player. In the `handleStartState` function, it is initialized to
 * 0 when the game starts.
 * @param switcher The `switcher` parameter in the `handleStartState` function is a pointer to an
 * integer. It is used to control certain states or actions within the function. In this specific
 * function, it is being set to 0 when the player presses the Start button to transition to the PLAY
 * state.
 * @param switcher2 The `switcher2` parameter is a pointer to an integer that is used as a switcher in
 * the `handleStartState` function. It is initially checked to be equal to 0, and if it is, it is set
 * to 1 within the function. This switcher is likely
 * @param switcher3 The `switcher3` parameter in the `handleStartState` function is a pointer to an
 * integer. It is used to keep track of a certain state or condition within the function. In the
 * provided code snippet, `switcher3` is set to 0 when the `handleStartState`
 * @param switcher4 The `switcher4` parameter in the `handleStartState` function is a pointer to an
 * integer. It is used to keep track of a certain state or condition within the function. In the
 * provided code snippet, `switcher4` is set to 0 when transitioning from the start state
 * @param state The `state` parameter is a pointer to a variable of type `gba_state`. It is used to
 * keep track of the current state of the game. In this function, if the Start button is pressed, the
 * state is updated to `PLAY`, indicating that the game should transition to the playing state
 * @param currentButtons The `currentButtons` parameter is a pointer to an unsigned 32-bit integer
 * (`u32`) variable. It is likely used to store the current state of the buttons being pressed on the
 * input device, such as a game controller. The function `KEY_DOWN` is likely a macro or function that
 */
void handleStartState(Player *player, Shield *shield, int *bulletCounter, int *switcher, int *switcher2, int *switcher3, int *switcher4, gba_state *state, u32 *currentButtons)
{
    player->health = 100;
    *bulletCounter = 0;
    shield->active = 1;

    if (*switcher2 == 0)
    {
        *switcher2 = 1;
    }

    drawCenteredString(50, 72, 100, 10, "Press Start to Start", WHITE);
    drawCenteredString(70, 72, 100, 10, "Survive 20 seconds ", WHITE);
    drawCenteredString(90, 72, 100, 10, "To win", WHITE);

    if (KEY_DOWN(BUTTON_START, *currentButtons))
    {
        *state = PLAY;
        *switcher = 0;
        *switcher2 = 0;
        *switcher3 = 0;
        *switcher4 = 0;
    }
}