#include "include/main.h"

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
  Player player2 = {93, 118, 5, 5, 100, 100};

  Player oldPlayer = player;
  Player oldPlayer2 = player2;

  Shield shield = {93, 118, 9, SHIELD_UP, 1};
  int bulletCounter = 0;
  int switcher = 0;
  int switcher2 = 0;
  int switcher3 = 0;
  int switcher4 = 0;

  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  ShieldDirection oldShieldDirection = SHIELD_UP;

  gba_state state = START;

  while (1)
  {
    waitForVBlank();

    currentButtons = BUTTONS;
    oldPlayer = player;
    oldPlayer2 = player2;

    switch (state)
    {
    case START:

      player.health = 100;
      bulletCounter = 0;
      shield.active = 1;
      bulletCounter = 0;

      if (switcher2 == 0)
      {
        player2.col = 118;
        player2.row = 93;

        switcher2 = 1;
      }
      drawCenteredString(50, 72, 100, 10, "Press Start to Start", WHITE);
      drawCenteredString(70, 72, 100, 10, "Survive 20 seconds ", WHITE);
      drawCenteredString(90, 72, 100, 10, "To win", WHITE);

      clearPlayer(&oldPlayer2);
      updatePlayer(&player2, currentButtons);
      drawImageDMA(player2.row, player2.col, player2.width, player2.height, character);

      if (KEY_DOWN(BUTTON_START, currentButtons))
      {
        state = PLAY;
        if (switcher)
        {
          switcher = 0;
          switcher2 = 0;
          switcher3 = 0;
          switcher4 = 0;
        }
      }

      break;

    case PLAY:
      currentButtons = BUTTONS;
      oldPlayer = player;

      if (switcher == 0)
      {
        drawFullScreenImageDMA(background2);
        drawBox();
        drawUI();
        drawButtons();
        drawHealthBarBorder();
        initBullets();
        switcher = 1;
      }

      drawEnemy();

      if (shield.active)
      {
        updateShield(&shield, currentButtons);
      }
      else
      {
        updatePlayer(&player, currentButtons);
      }

      updateBullets(&shield);

      updateHealth(&player, &shield);

      drawBox();

      if (shield.active)
      {
        oldShieldDirection = drawShield(&shield, &player, oldShieldDirection);
      }

      bulletCounter++;

      clearPlayer(&oldPlayer);
      drawHealthBar(&player);
      drawBullets();
      drawImageDMA(player.row, player.col, player.width, player.height, character);

      if (bulletCounter >= 1000)
      {
        shield.active = 0;
        state = WIN;
        fillScreenDMA(BLACK);
      }

      if (rand() % 50 == 0)
      {
        Direction dir = rand() % 4;
        addBullet(dir, rand() % 2 + 1);
      }

      if (player.health <= 0)
      {
        state = LOSE;
      }

      break;
    case WIN:

      if (switcher4 == 0)
      {
        drawFullScreenImageDMA(background1);
        drawCenteredString(70, 72, 100, 10, "Congratulations!", WHITE);
        drawCenteredString(90, 72, 100, 10, "Press Start to Retry", WHITE);
        switcher4 = 1;
      }
      if (KEY_DOWN(BUTTON_START, currentButtons))
      {
        state = START;
        fillScreenDMA(BLACK);
      }
      break;
    case LOSE:

      if (switcher3 == 0)
      {
        drawFullScreenImageDMA(background1);
        drawCenteredString(70, 72, 100, 10, "Game Over!", WHITE);
        drawCenteredString(90, 72, 100, 10, "Press Start to Retry", WHITE);
        switcher3 = 1;
      }
      if (KEY_DOWN(BUTTON_START, currentButtons))
      {
        state = START;
        fillScreenDMA(BLACK);
      }
      break;
    }

    if (KEY_DOWN(BUTTON_SELECT, currentButtons))
    {
      state = START;
      fillScreenDMA(BLACK);
      continue;
    }

    previousButtons = currentButtons;
  }

  UNUSED(previousButtons);

  return 0;
}
