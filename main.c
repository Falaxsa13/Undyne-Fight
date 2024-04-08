#include "include/main.h"
#include "include/gameStateHandlers.h"

/**
 * @brief Updates the player's health based on collisions with bullets.
 *
 * This function iterates through the bullets array and checks for collisions between
 * the player and each bullet. If a collision is detected and the bullet does not collide
 * with the shield, the player's health is reduced by 10 and the bullet is removed from
 * the screen.
 *
 * @param p Pointer to the player object.
 * @param shield Pointer to the shield object.
 */
void updateHealth(Player *p, Shield *shield)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (checkCollision(p, &bullets[i]) && !checkShieldCollision(shield, &bullets[i]))
    {
      play_sound();
      p->health -= 10;

      clearBullet(&bullets[i]);
      bullets[i].active = 0;
    }
  }
}

/**
 * @brief Draws the health bar on the screen representing the player's current health.
 *
 * This function draws a health bar on the screen to visually represent the player's current
 * health. If the player's health is greater than 0, the function calculates the width of the
 * health bar based on the player's current health compared to their maximum health. The health
 * bar is then drawn using two rectangles: one for the background (representing the total health)
 * and another for the current health (filled in a different color).
 *
 * @param p Pointer to the player object.
 */
void drawHealthBar(Player *p)
{
  if (p->health > 0)
  {
    drawRectDMA(HEALTH_BAR_Y, HEALTH_BAR_X, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, HEALTH_BAR_LOW_COLOR);
    int currentHealthWidth = (p->health * HEALTH_BAR_WIDTH) / p->maxHealth;
    drawRectDMA(HEALTH_BAR_Y, HEALTH_BAR_X, currentHealthWidth, HEALTH_BAR_HEIGHT, HEALTH_BAR_FULL_COLOR);
  }
}

int main(void)
{

  REG_DISPCNT = MODE3 | BG2_ENABLE;
  sound_init();

  Player player = {93, 118, 5, 5, 100, 100};
  Shield shield = {93, 118, 9, SHIELD_UP, 1};

  int bulletCounter = 0;
  int switcher = 0;
  int switcher2 = 0;
  int switcher3 = 0;
  int switcher4 = 0;

  u32 currentButtons = BUTTONS;

  ShieldDirection oldShieldDirection = SHIELD_UP;
  gba_state state = START;

  while (1)
  {
    waitForVBlank();
    currentButtons = BUTTONS;

    switch (state)
    {
    case START:
      handleStartState(&player, &shield, &bulletCounter, &switcher, &switcher2, &switcher3, &switcher4, &state, &currentButtons);
      break;

    case PLAY:
      handlePlayState(&player, &shield, &bulletCounter, &state, &currentButtons, &switcher, &oldShieldDirection);
      break;

    case WIN:
      handleWinState(&state, &currentButtons, &switcher4);
      break;

    case LOSE:
      handleLoseState(&state, &currentButtons, &switcher3);
      break;
    }

    if (KEY_DOWN(BUTTON_SELECT, currentButtons))
    {
      state = START;
      fillScreenDMA(BLACK);
      switcher = switcher2 = switcher3 = switcher4 = 0;
    }
  }

  return 0;
}
